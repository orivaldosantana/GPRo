// This file uses OpenMesh, licensed under following conditions:

/*  OpenMesh is free software: you can redistribute it and/or modify         *
 *  it under the terms of the GNU Lesser General Public License as           *
 *  published by the Free Software Foundation, either version 3 of           *
 *  the License, or (at your option) any later version with the              *
 *  following exceptions:                                                    *
 *                                                                           *
 *  If other files instantiate templates or use macros                       *
 *  or inline functions from this file, or you compile this file and         *
 *  link it with other files to produce an executable, this file does        *
 *  not by itself cause the resulting executable to be covered by the        *
 *  GNU Lesser General Public License. This exception does not however       *
 *  invalidate any other reasons why the executable file might be            *
 *  covered by the GNU Lesser General Public License.                        *
 *                                                                           *
 *  OpenMesh is distributed in the hope that it will be useful,              *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU Lesser General Public License for more details.                      *
 *                                                                           *
 *  You should have received a copy of the GNU LesserGeneral Public          *
 *  License along with OpenMesh.  If not,                                    *
 *  see <http://www.gnu.org/licenses/>.                                      *
 */

#define _USE_MATH_DEFINES
#define NOMINMAX

#include "v_repExtOpenMesh.h"
#include "scriptFunctionData.h"
#include "v_repLib.h"
#include "OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh"
#include "OpenMesh/Tools/Decimater/DecimaterT.hh"
#include "OpenMesh/Tools/Decimater/ModQuadricT.hh"
// #include "OpenMesh/Tools/Decimater/ModRoundnessT.hh"
// #include "OpenMesh/Tools/Decimater/ModHausdorffT.hh"
// #include "OpenMesh/Tools/Decimater/ModAspectRatioT.hh"
// #include "OpenMesh/Tools/Decimater/ModNormalDeviationT.hh"
// #include <OpenMesh/Core/Geometry/VectorT.hh>

typedef OpenMesh::TriMesh_ArrayKernelT<> Mesh;
typedef OpenMesh::Decimater::DecimaterT< Mesh > Decimater;
typedef OpenMesh::Decimater::ModQuadricT< Mesh >::Handle HModQuadric;
// typedef OpenMesh::Decimater::ModRoundnessT< Mesh >::Handle HModRoundness;
// typedef OpenMesh::Decimater::ModHausdorffT< Mesh >::Handle HModHausdorff;
// typedef OpenMesh::Decimater::ModAspectRatioT< Mesh >::Handle HModAspectRatio;
// typedef OpenMesh::Decimater::ModNormalDeviationT< Mesh >::Handle HModNormalDeviation;

#ifdef _WIN32
    #ifdef QT_COMPIL
        #include <direct.h>
        #include <iostream>
    #else
        #include <shlwapi.h>
        #pragma comment(lib, "Shlwapi.lib")
    #endif
#endif /* _WIN32 */
#if defined (__linux) || defined (__APPLE__)
    #include <unistd.h>
#endif /* __linux || __APPLE__ */

#define CONCAT(x,y,z) x y z
#define strConCat(x,y,z)    CONCAT(x,y,z)
LIBRARY vrepLib;

bool compute(const float* verticesIn,int verticesInLength,const int* indicesIn,int indicesInLength,float decimationPercentage,std::vector<float>& verticesOut,std::vector<int>& indicesOut)
{
        Mesh mesh;
        Decimater   decimater(mesh);

        HModQuadric hModQuadric;
        decimater.add(hModQuadric);
        decimater.module(hModQuadric).unset_max_err();
    

//      HModRoundness hModRoundness; 
//      decimater.add(hModRoundness);
//      decimater.module(hModRoundness).set_binary(false);

//      HModHausdorff hModHausdorff; 
//      decimater.add(hModHausdorff);
//      decimater.module(hModHausdorff).set_binary(false);

//      HModAspectRatio hModAspectRatio; 
//      decimater.add(hModAspectRatio);
//      decimater.module(hModAspectRatio).set_binary(false);

//      HModNormalDeviation hModNormalDeviation; 
//      decimater.add(hModNormalDeviation);
//      decimater.module(hModNormalDeviation).set_binary(false);


        std::vector<Mesh::VertexHandle> vhandles;
        for (int i=0;i<verticesInLength/3;i++)
            vhandles.push_back(mesh.add_vertex(Mesh::Point(verticesIn[3*i+0],verticesIn[3*i+1],verticesIn[3*i+2])));

        std::vector<Mesh::VertexHandle> face_vhandles;
        for (int i=0;i<indicesInLength/3;i++)
        {
            face_vhandles.clear();
            face_vhandles.push_back(vhandles[indicesIn[3*i+0]]);
            face_vhandles.push_back(vhandles[indicesIn[3*i+1]]);
            face_vhandles.push_back(vhandles[indicesIn[3*i+2]]);
            mesh.add_face(face_vhandles);
        }

        decimater.initialize();
        decimater.decimate_to_faces(0,int(decimationPercentage*float(indicesInLength/3)));
        mesh.garbage_collection();

        verticesOut.clear();
        Mesh::VertexHandle vh;
        OpenMesh::Vec3f v;
        for (int i=0;i<int(mesh.n_vertices());i++)
        {
            vh = Mesh::VertexHandle(i);
            v  = mesh.point(vh);
            verticesOut.push_back(v[0]);
            verticesOut.push_back(v[1]);
            verticesOut.push_back(v[2]);
        }

        indicesOut.clear();
        Mesh::FaceHandle fh;
        OpenMesh::ArrayItems::Face f;
        for (int i=0;i<int(mesh.n_faces());i++)
        {
            fh = Mesh::FaceHandle(i);
            mesh.cfv_iter(fh);
            OpenMesh::PolyConnectivity::ConstFaceVertexIter cfv_it=mesh.cfv_iter(fh);
            indicesOut.push_back(cfv_it->idx());
            ++cfv_it;
            indicesOut.push_back(cfv_it->idx());
            ++cfv_it;
            indicesOut.push_back(cfv_it->idx());
        }
        return((verticesOut.size()>=9)&&(indicesOut.size()>=3));
}


// --------------------------------------------------------------------------------------
// simExtOpenMesh_getDecimated
// --------------------------------------------------------------------------------------
#define LUA_DECIMATE_COMMAND "simExtOpenMesh_getDecimated"

const int inArgs_DECIMATE[]={
    4,
    sim_script_arg_float|sim_script_arg_table,9,
    sim_script_arg_int32|sim_script_arg_table,6,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_DECIMATE_CALLBACK(SScriptCallBack* p)
{ // keep for backward compatibility
    CScriptFunctionData D;
    int result=-1;
    if (D.readDataFromStack(p->stackID,inArgs_DECIMATE,inArgs_DECIMATE[0],LUA_DECIMATE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        float* outV;
        int outVL;
        int* outI;
        int outIL;
        float percent=float(inData->at(3).int32Data[0])/float(inData->at(1).int32Data.size()/3);
        int res=simGetDecimatedMesh(&inData->at(0).floatData[0],inData->at(0).floatData.size(),&inData->at(1).int32Data[0],inData->at(1).int32Data.size(),&outV,&outVL,&outI,&outIL,percent,0,NULL);
        if (res>0)
        {
            std::vector<float> v2(outV,outV+outVL);
            std::vector<int> i2(outI,outI+outIL);
            simReleaseBuffer((simChar*)outV);
            simReleaseBuffer((simChar*)outI);
            D.pushOutData(CScriptFunctionDataItem(v2));
            D.pushOutData(CScriptFunctionDataItem(i2));
        }
/*
        Mesh mesh;
        Decimater   decimater(mesh);

        HModQuadric hModQuadric;
        decimater.add(hModQuadric);
        decimater.module(hModQuadric).unset_max_err();
    

//      HModRoundness hModRoundness; 
//      decimater.add(hModRoundness);
//      decimater.module(hModRoundness).set_binary(false);

//      HModHausdorff hModHausdorff; 
//      decimater.add(hModHausdorff);
//      decimater.module(hModHausdorff).set_binary(false);

//      HModAspectRatio hModAspectRatio; 
//      decimater.add(hModAspectRatio);
//      decimater.module(hModAspectRatio).set_binary(false);

//      HModNormalDeviation hModNormalDeviation; 
//      decimater.add(hModNormalDeviation);
//      decimater.module(hModNormalDeviation).set_binary(false);


        std::vector<Mesh::VertexHandle> vhandles;
        for (int i=0;i<int(inData->at(0).floatData.size()/3);i++)
            vhandles.push_back(mesh.add_vertex(Mesh::Point(inData->at(0).floatData[3*i+0],inData->at(0).floatData[3*i+1],inData->at(0).floatData[3*i+2])));

        std::vector<Mesh::VertexHandle> face_vhandles;
        for (int i=0;i<int(inData->at(1).intData.size()/3);i++)
        {
            face_vhandles.clear();
            face_vhandles.push_back(vhandles[inData->at(1).intData[3*i+0]]);
            face_vhandles.push_back(vhandles[inData->at(1).intData[3*i+1]]);
            face_vhandles.push_back(vhandles[inData->at(1).intData[3*i+2]]);
            mesh.add_face(face_vhandles);
        }

        decimater.initialize();
        decimater.decimate_to_faces(inData->at(2).intData[0],inData->at(3).intData[0]);
        mesh.garbage_collection();


        std::vector<float> newVertices;
        Mesh::VertexHandle vh;
        OpenMesh::Vec3f v;
        for (int i=0;i<int(mesh.n_vertices());i++)
        {
            vh = Mesh::VertexHandle(i);
            v  = mesh.point(vh);
            newVertices.push_back(v[0]);
            newVertices.push_back(v[1]);
            newVertices.push_back(v[2]);
        }

        std::vector<int> newIndices;
        Mesh::FaceHandle fh;
        OpenMesh::ArrayItems::Face f;
        for (int i=0;i<int(mesh.n_faces());i++)
        {
            fh = Mesh::FaceHandle(i);
            mesh.cfv_iter(fh);
            OpenMesh::PolyConnectivity::ConstFaceVertexIter cfv_it=mesh.cfv_iter(fh);
            newIndices.push_back(cfv_it->idx());
            ++cfv_it;
            newIndices.push_back(cfv_it->idx());
            ++cfv_it;
            newIndices.push_back(cfv_it->idx());
        }

        D.pushOutData(CScriptFunctionDataItem(newVertices));
        D.pushOutData(CScriptFunctionDataItem(newIndices));
        */
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------


// This is the plugin start routine:
VREP_DLLEXPORT unsigned char v_repStart(void* reservedPointer,int reservedInt)
{ // This is called just once, at the start of V-REP
    // Dynamically load and bind V-REP functions:
    char curDirAndFile[1024];
#ifdef _WIN32
    #ifdef QT_COMPIL
        _getcwd(curDirAndFile, sizeof(curDirAndFile));
    #else
        GetModuleFileName(NULL,curDirAndFile,1023);
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

    vrepLib=loadVrepLibrary(temp.c_str());
    if (vrepLib==NULL)
    {
        std::cout << "Error, could not find or correctly load the V-REP library. Cannot start 'OpenMesh' plugin.\n";
        return(0); // Means error, V-REP will unload this plugin
    }
    if (getVrepProcAddresses(vrepLib)==0)
    {
        std::cout << "Error, could not find all required functions in the V-REP library. Cannot start 'OpenMesh' plugin.\n";
        unloadVrepLibrary(vrepLib);
        return(0); // Means error, V-REP will unload this plugin
    }

    int vrepVer;
    simGetIntegerParameter(sim_intparam_program_version,&vrepVer);
    if (vrepVer<30200) // if V-REP version is smaller than 3.02.00
    {
        std::cout << "Sorry, your V-REP copy is somewhat old. Cannot start 'OpenMesh' plugin.\n";
        unloadVrepLibrary(vrepLib);
        return(0); // Means error, V-REP will unload this plugin
    }

    // Register the new Lua commands:
    simRegisterScriptCallbackFunction(strConCat(LUA_DECIMATE_COMMAND,"@","OpenMesh"),strConCat("table newVertices,table newIndices=",LUA_DECIMATE_COMMAND,"(table vertices,table indices,number maxVertices,number maxTriangles)"),LUA_DECIMATE_CALLBACK);

    return(3);  // initialization went fine, we return the version number of this extension module (can be queried with simGetModuleName). 3 since V3.3.1
}

// This is the plugin end routine:
VREP_DLLEXPORT void v_repEnd()
{ // This is called just once, at the end of V-REP
    unloadVrepLibrary(vrepLib); // release the library
}

// This is the plugin messaging routine (i.e. V-REP calls this function very often, with various messages):
VREP_DLLEXPORT void* v_repMessage(int message,int* auxiliaryData,void* customData,int* replyData)
{ // This is called quite often. Just watch out for messages/events you want to handle

    // This function should not generate any error messages:
    int errorModeSaved;
    simGetIntegerParameter(sim_intparam_error_report_mode,&errorModeSaved);
    simSetIntegerParameter(sim_intparam_error_report_mode,sim_api_errormessage_ignore);

    void* retVal=NULL;

    if (message==sim_message_eventcallback_instancepass)
    {
    }

    if (message==sim_message_eventcallback_simulationended)
    { // Simulation just ended
    }

    simSetIntegerParameter(sim_intparam_error_report_mode,errorModeSaved); // restore previous settings
    return(retVal);
}

VREP_DLLEXPORT void v_repDecimateMesh(void* data)
{
    // Collect info from V-REP:
    void** valPtr=(void**)data;
    float* verticesIn=((float*)valPtr[0]);
    int verticesInLength=((int*)valPtr[1])[0];
    int* indicesIn=((int*)valPtr[2]);
    int indicesInLength=((int*)valPtr[3])[0];
    float decimationPercentage=((float*)valPtr[4])[0];
    int interfaceVersion=((int*)valPtr[5])[0]; // should be zero for the current version

    std::vector<float> verticesOut;
    std::vector<int> indicesOut;
    bool result=compute(verticesIn,verticesInLength,indicesIn,indicesInLength,decimationPercentage,verticesOut,indicesOut);
    ((bool*)valPtr[6])[0]=result;
    if (result)
    {
        float* v=(float*)simCreateBuffer(verticesOut.size()*sizeof(float));
        for (size_t i=0;i<verticesOut.size();i++)
            v[i]=verticesOut[i];
        ((float**)valPtr[7])[0]=v;
        ((int*)valPtr[8])[0]=verticesOut.size();
        int* ind=(int*)simCreateBuffer(indicesOut.size()*sizeof(int));
        for (size_t i=0;i<indicesOut.size();i++)
            ind[i]=indicesOut[i];
        ((int**)valPtr[9])[0]=ind;
        ((int*)valPtr[10])[0]=indicesOut.size();
    }
}
