// This file is part of the ConvexDecompose interface for V-REP
// 
// -------------------------------------------------------------------
// Copyright (c) 2011 Khaled Mamou (kmamou at gmail dot com)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
// 
// 3. The names of the contributors may not be used to endorse or promote products derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// -------------------------------------------------------------------

//
// This file was automatically created for V-REP release V3.4.0 rev. 1 on April 5th 2017

#include "v_repExtConvexDecompose.h"
#include "v_repLib.h"
#include <hacdHACD.h>
#include <hacdMicroAllocator.h>
#include "VHACD.h"
#include <iostream>

#ifdef _WIN32
    #ifdef QT_COMPIL
        #include <direct.h>
    #else
        #include <shlwapi.h>
        #pragma comment(lib, "Shlwapi.lib")
    #endif
#endif
#if defined (__linux) || defined (__APPLE__)
    #include <unistd.h>
#endif

#define PLUGIN_VERSION 1

LIBRARY vrepLib;

int computeHACD(const float* vertices,int verticesLength,const int* indices,int indicesLength,std::vector<std::vector<float>*>& verticesList,std::vector<std::vector<int>*>& indicesList,size_t nClusters,double concavity,bool addExtraDistPoints,bool addFacesPoints,double ccConnectDist,size_t targetNTrianglesDecimatedMesh,size_t maxHullVertices,double smallestClusterThreshold)
{
    int retVal=0;

    printf("Computing the convex decomposition (HACD)...\n");
    std::vector< HACD::Vec3<HACD::Real> > points;
    std::vector< HACD::Vec3<long> > triangles;

    for (int i=0;i<verticesLength/3;i++)
    {
        HACD::Vec3<HACD::Real> v(vertices[3*i+0],vertices[3*i+1],vertices[3*i+2]);
        points.push_back(v);
    }
    for (int i=0;i<indicesLength/3;i++)
    {
        HACD::Vec3<long> t(indices[3*i+0],indices[3*i+1],indices[3*i+2]);
        triangles.push_back(t);
    }

    HACD::HeapManager * heapManager = HACD::createHeapManager(65536*(1000));

    HACD::HACD * const myHACD = HACD::CreateHACD(heapManager);
    myHACD->SetPoints(&points[0]);
    myHACD->SetNPoints(points.size());
    myHACD->SetTriangles(&triangles[0]);
    myHACD->SetNTriangles(triangles.size());
    myHACD->SetCompacityWeight(0.0001);
    myHACD->SetVolumeWeight(0.0);
    myHACD->SetConnectDist(ccConnectDist);               // if two connected components are seperated by distance < ccConnectDist
                                                        // then create a virtual edge between them so the can be merged during
                                                        // the simplification process

    myHACD->SetNClusters(nClusters);                     // minimum number of clusters
    myHACD->SetNVerticesPerCH(maxHullVertices);                      // max of 100 vertices per convex-hull
    myHACD->SetConcavity(concavity);                     // maximum concavity
    myHACD->SetSmallClusterThreshold(smallestClusterThreshold);              // threshold to detect small clusters
    myHACD->SetNTargetTrianglesDecimatedMesh(targetNTrianglesDecimatedMesh); // # triangles in the decimated mesh
//  myHACD->SetCallBack(&CallBack);
    myHACD->SetAddExtraDistPoints(addExtraDistPoints);
    myHACD->SetAddFacesPoints(addFacesPoints);

    {
        myHACD->Compute();
    }

    nClusters = myHACD->GetNClusters();
    printf("Done (%i clusters generated).\n",nClusters);
    retVal=nClusters;

    for(size_t c = 0; c < nClusters; ++c)
    {
        size_t nPoints = myHACD->GetNPointsCH(c);
        size_t nTriangles = myHACD->GetNTrianglesCH(c);
        HACD::Vec3<HACD::Real> * pointsCH = new HACD::Vec3<HACD::Real>[nPoints];
        HACD::Vec3<long> * trianglesCH = new HACD::Vec3<long>[nTriangles];
        myHACD->GetCH(c, pointsCH, trianglesCH);
        std::vector<float>* _vert=new std::vector<float>;
        std::vector<int>* _ind=new std::vector<int>;
        for (int i=0;i<int(nPoints);i++)
        {
            _vert->push_back(float(pointsCH[i].X()));
            _vert->push_back(float(pointsCH[i].Y()));
            _vert->push_back(float(pointsCH[i].Z()));
        }
        for (int i=0;i<int(nTriangles);i++)
        {
            _ind->push_back(trianglesCH[i].X());
            _ind->push_back(trianglesCH[i].Y());
            _ind->push_back(trianglesCH[i].Z());
        }
        verticesList.push_back(_vert);
        indicesList.push_back(_ind);

        delete [] pointsCH;
        delete [] trianglesCH;
    }

    HACD::DestroyHACD(myHACD);
    HACD::releaseHeapManager(heapManager);

    return(retVal);
}

int computeVHACD(const float* vertices,int verticesLength,const int* indices,int indicesLength,std::vector<std::vector<float>*>& verticesList,std::vector<std::vector<int>*>& indicesList,int resolution,int depth,float concavity,int planeDownsampling,int convexHullDownsampling,float alpha,float beta,float gamma,bool pca,bool voxelBased,int maxVerticesPerCH,float minVolumePerCH)
{
    printf("Computing the convex decomposition (VHACD)...\n");
    VHACD::IVHACD::Parameters   params;
    params.m_resolution=resolution;
    params.m_depth=depth;
    params.m_concavity=concavity;
    params.m_planeDownsampling=planeDownsampling;
    params.m_convexhullDownsampling=convexHullDownsampling;
    params.m_alpha=alpha;
    params.m_beta=beta;
    params.m_gamma=gamma;
    params.m_pca=pca;
    params.m_mode=!voxelBased;
    params.m_maxNumVerticesPerCH=maxVerticesPerCH;
    params.m_minVolumePerCH=minVolumePerCH;
    VHACD::IVHACD* interfaceVHACD=VHACD::CreateVHACD();
    interfaceVHACD->Compute(vertices,3,verticesLength/3,indices,3,indicesLength/3,params);

    unsigned int nConvexHulls=interfaceVHACD->GetNConvexHulls();
    printf("Done (%i clusters generated).\n",nConvexHulls);
    VHACD::IVHACD::ConvexHull ch;
    for (unsigned int p=0;p<nConvexHulls;++p)
    {
        interfaceVHACD->GetConvexHull(p,ch);
        std::vector<float>* _vert=new std::vector<float>;
        std::vector<int>* _ind=new std::vector<int>;
        for (unsigned int v=0,idx=0;v<ch.m_nPoints;++v,idx+=3)
        {
            _vert->push_back(ch.m_points[idx+0]);
            _vert->push_back(ch.m_points[idx+1]);
            _vert->push_back(ch.m_points[idx+2]);
        }
        for (unsigned int t=0,idx=0;t<ch.m_nTriangles;++t,idx+=3)
        {
            _ind->push_back(ch.m_triangles[idx+0]);
            _ind->push_back(ch.m_triangles[idx+1]);
            _ind->push_back(ch.m_triangles[idx+2]);
        }
        verticesList.push_back(_vert);
        indicesList.push_back(_ind);
    }

    interfaceVHACD->Clean();
    interfaceVHACD->Release();
    return((int)nConvexHulls);
}

VREP_DLLEXPORT unsigned char v_repStart(void* reservedPointer,int reservedInt)
{
    // 1. Figure out this plugin's directory:
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

    // 2. Append the V-REP library's name:
    std::string temp(currentDirAndPath);
#ifdef _WIN32
    temp+="\\v_rep.dll";
#elif defined (__linux)
    temp+="/libv_rep.so";
#elif defined (__APPLE__)
    temp+="/libv_rep.dylib";
#endif 

    // 3. Load the V-REP library:
    vrepLib=loadVrepLibrary(temp.c_str());
    if (vrepLib==NULL)
    {
        std::cout << "Error, could not find or correctly load the V-REP library. Cannot start 'ConvexDecompose' plugin.\n";
        return(0); 
    }
    if (getVrepProcAddresses(vrepLib)==0)
    {
        std::cout << "Error, could not find all required functions in the V-REP library. Cannot start 'ConvexDecompose' plugin.\n";
        unloadVrepLibrary(vrepLib);
        return(0);
    }

    // Check the version of V-REP:
    int vrepVer;
    simGetIntegerParameter(sim_intparam_program_version,&vrepVer);
    if (vrepVer<30201)
    {
        std::cout << "Sorry, your V-REP copy is somewhat old. Cannot start 'ConvexDecompose' plugin.\n";
        unloadVrepLibrary(vrepLib);
        return(0);
    }

    return(PLUGIN_VERSION);
}

VREP_DLLEXPORT void v_repEnd()
{
    unloadVrepLibrary(vrepLib);
}

VREP_DLLEXPORT void* v_repMessage(int message,int* auxiliaryData,void* customData,int* replyData)
{ // This is called quite often. Just watch out for messages/events you want to handle
    return(NULL);
}

VREP_DLLEXPORT void v_repHACD(void* data)
{
    // Collect info from V-REP:
    void** valPtr=(void**)data;
    float* vertices=((float*)valPtr[0]);
    int verticesLength=((int*)valPtr[1])[0];
    int* indices=((int*)valPtr[2]);
    int indicesLength=((int*)valPtr[3])[0];
    size_t nClusters=((size_t*)valPtr[4])[0];
    double concavity=((double*)valPtr[5])[0];
    bool addExtraDistPoints=((bool*)valPtr[6])[0];
    bool addFacesPoints=((bool*)valPtr[7])[0];
    double ccConnectDist=((double*)valPtr[8])[0];
    size_t targetNTrianglesDecimatedMesh=((size_t*)valPtr[9])[0];
    size_t maxHullVertices=((size_t*)valPtr[10])[0];
    double smallestClusterThreshold=((double*)valPtr[11])[0];

    std::vector<std::vector<float>*> verticesList;
    std::vector<std::vector<int>*> indicesList;
    computeHACD(vertices,verticesLength,indices,indicesLength,verticesList,indicesList,nClusters,concavity,addExtraDistPoints,addFacesPoints,ccConnectDist,targetNTrianglesDecimatedMesh,maxHullVertices,smallestClusterThreshold);
    int el=int(verticesList.size());
    ((int*)valPtr[12])[0]=el;
    if (el>0)
    {
        float** vertList=(float**)simCreateBuffer(el*sizeof(float*));
        ((float***)valPtr[13])[0]=vertList;
        int** indList=(int**)simCreateBuffer(el*sizeof(int*));
        ((int***)valPtr[14])[0]=indList;
        int* vertCountList=(int*)simCreateBuffer(el*sizeof(int));
        ((int**)valPtr[15])[0]=vertCountList;
        int* indCountList=(int*)simCreateBuffer(el*sizeof(int));
        ((int**)valPtr[16])[0]=indCountList;

        for (int mesh=0;mesh<el;mesh++)
        {
            float* vert=(float*)simCreateBuffer(verticesList[mesh]->size()*sizeof(float));
            for (size_t i=0;i<verticesList[mesh]->size();i++)
                vert[i]=verticesList[mesh]->at(i);
            vertList[mesh]=vert;
            vertCountList[mesh]=verticesList[mesh]->size();
            int* ind=(int*)simCreateBuffer(indicesList[mesh]->size()*sizeof(int));
            for (size_t i=0;i<indicesList[mesh]->size();i++)
                ind[i]=indicesList[mesh]->at(i);
            indList[mesh]=ind;
            indCountList[mesh]=indicesList[mesh]->size();
        }
    }
}

VREP_DLLEXPORT void v_repVHACD(void* data)
{
    // Collect info from V-REP:
    void** valPtr=(void**)data;
    float* vertices=((float*)valPtr[0]);
    int verticesLength=((int*)valPtr[1])[0];
    int* indices=((int*)valPtr[2]);
    int indicesLength=((int*)valPtr[3])[0];

    int resolution=((int*)valPtr[4])[0];
    int depth=((int*)valPtr[5])[0];
    float concavity=((float*)valPtr[6])[0];
    int planeDownsampling=((int*)valPtr[7])[0];
    int convexHullDownsampling=((int*)valPtr[8])[0];
    float alpha=((float*)valPtr[9])[0];
    float beta=((float*)valPtr[10])[0];
    float gamma=((float*)valPtr[11])[0];
    bool pca=((bool*)valPtr[12])[0];
    bool voxelBased=((bool*)valPtr[13])[0];
    int maxVerticesPerCH=((int*)valPtr[14])[0];
    float minVolumePerCH=((float*)valPtr[15])[0];

    std::vector<std::vector<float>*> verticesList;
    std::vector<std::vector<int>*> indicesList;
    computeVHACD(vertices,verticesLength,indices,indicesLength,verticesList,indicesList,resolution,depth,concavity,planeDownsampling,convexHullDownsampling,alpha,beta,gamma,pca,voxelBased,maxVerticesPerCH,minVolumePerCH);
    int el=int(verticesList.size());
    ((int*)valPtr[16])[0]=el;
    if (el>0)
    {
        float** vertList=(float**)simCreateBuffer(el*sizeof(float*));
        ((float***)valPtr[17])[0]=vertList;
        int** indList=(int**)simCreateBuffer(el*sizeof(int*));
        ((int***)valPtr[18])[0]=indList;
        int* vertCountList=(int*)simCreateBuffer(el*sizeof(int));
        ((int**)valPtr[19])[0]=vertCountList;
        int* indCountList=(int*)simCreateBuffer(el*sizeof(int));
        ((int**)valPtr[20])[0]=indCountList;

        for (int mesh=0;mesh<el;mesh++)
        {
            float* vert=(float*)simCreateBuffer(verticesList[mesh]->size()*sizeof(float));
            for (size_t i=0;i<verticesList[mesh]->size();i++)
                vert[i]=verticesList[mesh]->at(i);
            vertList[mesh]=vert;
            vertCountList[mesh]=verticesList[mesh]->size();
            int* ind=(int*)simCreateBuffer(indicesList[mesh]->size()*sizeof(int));
            for (size_t i=0;i<indicesList[mesh]->size();i++)
                ind[i]=indicesList[mesh]->at(i);
            indList[mesh]=ind;
            indCountList[mesh]=indicesList[mesh]->size();
        }
    }
}
