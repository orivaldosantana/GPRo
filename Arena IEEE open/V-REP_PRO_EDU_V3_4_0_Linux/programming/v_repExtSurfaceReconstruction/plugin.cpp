// Copyright 2016 Coppelia Robotics GmbH. All rights reserved. 
// marc@coppeliarobotics.com
// www.coppeliarobotics.com
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// -------------------------------------------------------------------
// Authors:
// Federico Ferri <federico.ferri.it at gmail dot com>
// -------------------------------------------------------------------

#ifdef _WIN32
	#define VREP_DLLEXPORT extern "C" __declspec(dllexport)
#endif /* _WIN32 */
#if defined (__linux) || defined (__APPLE__)
	#define VREP_DLLEXPORT extern "C"
#endif /* __linux || __APPLE__ */

// The 3 required entry points of the V-REP plugin:
VREP_DLLEXPORT unsigned char v_repStart(void* reservedPointer,int reservedInt);
VREP_DLLEXPORT void v_repEnd();
VREP_DLLEXPORT void* v_repMessage(int message,int* auxiliaryData,void* customData,int* replyData);

#include "plugin.h"
//#include "debug.h"
#include "v_repLib.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <boost/algorithm/string/predicate.hpp>

#ifdef _WIN32
    #ifdef QT_COMPIL
        #include <direct.h>
    #else
        #include <shlwapi.h>
        #pragma comment(lib, "Shlwapi.lib")
    #endif
#endif /* _WIN32 */
#if defined (__linux) || defined (__APPLE__)
    #include <unistd.h>
#define _stricmp strcasecmp
#endif /* __linux || __APPLE__ */

LIBRARY vrepLib; // the V-REP library that we will dynamically load and bind

#include "stubs.h"

#include <CGAL/Scale_space_surface_reconstruction_3.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Timer.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel     Kernel;
typedef CGAL::Scale_space_surface_reconstruction_3<Kernel>      Reconstruction;
typedef Reconstruction::Point                                   Point;
typedef std::vector<Point>                                      Point_collection;
typedef Reconstruction::Triple_const_iterator                   Triple_iterator;
typedef Reconstruction::Triple                                  Triple;

using std::string;

#include <fstream>
void reconstruct(SScriptCallBack *p, const char *cmd, reconstruct_in *in, reconstruct_out *out)
{
 //   DBG << "[enter]" << std::endl;

    int ptCnt = -1;
    const float *ptArray = simGetPointCloudPoints(in->pointCloudHandle, &ptCnt, 0);
    if(!ptArray)
        throw string("call to simGetPointCloudPoints failed");
    float m[12];
    simGetObjectMatrix(in->pointCloudHandle,-1,m);
    std::vector<float> _ptArray(ptArray,ptArray+ptCnt*3);
    Algorithm algorithm = (Algorithm)in->algorithm;
    switch(algorithm)
    {
    case sim_surfacereconstruction_algorithm_scalespace:
        {
            Point_collection points;
            for(int i = 0; i < ptCnt * 3; i += 3)
            {
                simTransformVector(m,&_ptArray[i]);
                points.push_back(Point(_ptArray[i+0], _ptArray[i+1], _ptArray[i+2]));
            }
            Reconstruction reconstruct(10, 200);
            reconstruct.reconstruct_surface(points.begin(), points.end(), 4);
            out->neighborhoodSquaredRadius = reconstruct.neighborhood_squared_radius();
            int triCount = reconstruct.number_of_triangles();
            simInt *idxArray = new simInt[triCount*3];
            int idxCnt = 0;
            for(std::size_t shell = 0; shell < reconstruct.number_of_shells(); ++shell) {
                for(Triple_iterator it = reconstruct.shell_begin(shell); it != reconstruct.shell_end(shell); ++it)
                {
                    const Triple &triple = *it;
                    idxArray[idxCnt++] = triple.at(0);
                    idxArray[idxCnt++] = triple.at(1);
                    idxArray[idxCnt++] = triple.at(2);
                }
            }
#if DEBUG_OUTPUT_RECONSTRUCTED_PLY
            std::ofstream f;
            f.open("reconstructed.ply");
            f << "ply\nformat ascii 1.0\nelement vertex " << ptCnt << "\nproperty float x\nproperty float y\nproperty float z\nelement face " << triCount << "\nproperty list uchar int vertex_index\nend_header\n";
            for(int i = 0; i < ptCnt; i++)
                f << _ptArray[3 * i] << " " << _ptArray[3 * i + 1] << " " << _ptArray[3 * i + 2] << std::endl;
            for(int i = 0; i < triCount; i++)
                f << "3 " << idxArray[3 * i] << " " << idxArray[3 * i + 1] << " " << idxArray[3 * i + 2] << std::endl;
            f.close();
#endif
            out->meshHandle = simCreateMeshShape(0, 1.2, &_ptArray[0], 3 * ptCnt, idxArray, idxCnt, 0);
            if(out->meshHandle == -1)
                throw string("call to simCreateMeshShape failed");
        }
        break;
    default:
        string err = "algorithm not implemented: ";
        err += algorithm_string(algorithm);
        throw err;
    }

//    DBG << "[leave]" << std::endl;
}

VREP_DLLEXPORT unsigned char v_repStart(void* reservedPointer, int reservedInt)
{
    char curDirAndFile[1024];
#ifdef _WIN32
    #ifdef QT_COMPIL
        _getcwd(curDirAndFile, sizeof(curDirAndFile));
    #else
        GetModuleFileName(NULL, curDirAndFile, 1023);
        PathRemoveFileSpec(curDirAndFile);
    #endif
#elif defined (__linux) || defined (__APPLE__)
    getcwd(curDirAndFile, sizeof(curDirAndFile));
#endif

    std::string currentDirAndPath(curDirAndFile);
    std::string temp(currentDirAndPath);
#ifdef _WIN32
    temp+="\\v_rep.dll";
#elif defined (__linux)
    temp+="/libv_rep.so";
#elif defined (__APPLE__)
    temp+="/libv_rep.dylib";
#endif /* __linux || __APPLE__ */
    vrepLib = loadVrepLibrary(temp.c_str());
    if(vrepLib == NULL)
    {
        std::cout << "Error, could not find or correctly load the V-REP library. Cannot start '" PLUGIN_NAME "' plugin." << std::endl;
        return 0;
    }
    if(getVrepProcAddresses(vrepLib)==0)
    {
        std::cout << "Error, could not find all required functions in the V-REP library. Cannot start '" PLUGIN_NAME "' plugin." << std::endl;
        unloadVrepLibrary(vrepLib);
        return 0;
    }

    int vrepVer;
    simGetIntegerParameter(sim_intparam_program_version, &vrepVer);
    if(vrepVer < 30203) // if V-REP version is smaller than 3.02.03
    {
        std::cout << "Sorry, your V-REP copy is somewhat old. Cannot start '" PLUGIN_NAME "' plugin." << std::endl;
        unloadVrepLibrary(vrepLib);
        return 0;
    }

    if(!registerScriptStuff())
    {
        std::cout << "Initialization failed." << std::endl;
        unloadVrepLibrary(vrepLib);
        return 0;
    }

    return PLUGIN_VERSION;
}

VREP_DLLEXPORT void v_repEnd()
{
//    DBG << "[enter]" << std::endl;

    unloadVrepLibrary(vrepLib); // release the library

//    DBG << "[leave]" << std::endl;
}

VREP_DLLEXPORT void* v_repMessage(int message, int* auxiliaryData, void* customData, int* replyData)
{
    // Keep following 5 lines at the beginning and unchanged:
    static bool refreshDlgFlag = true;
    int errorModeSaved;
    simGetIntegerParameter(sim_intparam_error_report_mode, &errorModeSaved);
    simSetIntegerParameter(sim_intparam_error_report_mode, sim_api_errormessage_ignore);
    void* retVal=NULL;

    static bool firstInstancePass = true;
    if(firstInstancePass && message == sim_message_eventcallback_instancepass)
    {
        firstInstancePass = false;
        //UIFunctions::getInstance(); // construct UIFunctions here (SIM thread)
    }

    if(message == sim_message_eventcallback_simulationended)
    { // Simulation just ended
        // TODO: move this to sim_message_eventcallback_simulationabouttoend
        // TODO: ASSERT_THREAD(???)
        //Proxy::destroyTransientObjects();
    }

#ifdef VREP_INSTANCE_SWITCH_WORKS
    static int oldSceneID = simGetInt32ParameterE(sim_intparam_scene_unique_id);
    if(message == sim_message_eventcallback_instanceswitch)
    {
        int newSceneID = simGetInt32ParameterE(sim_intparam_scene_unique_id);
        //Proxy::sceneChange(oldSceneID, newSceneID);
        oldSceneID = newSceneID;
    }
#else
    // XXX: currently (3.3.1 beta) it is broken
    if(message == sim_message_eventcallback_instancepass)
    {
        static int oldSceneID = -1;
        if(oldSceneID == -1) oldSceneID = simGetInt32ParameterE(sim_intparam_scene_unique_id);
        int sceneID = simGetInt32ParameterE(sim_intparam_scene_unique_id);
        if(sceneID != oldSceneID)
        {
            //Proxy::sceneChange(oldSceneID, sceneID);
            oldSceneID = sceneID;
        }
    }
#endif

    // Keep following unchanged:
    simSetIntegerParameter(sim_intparam_error_report_mode, errorModeSaved); // restore previous settings
    return(retVal);
}

