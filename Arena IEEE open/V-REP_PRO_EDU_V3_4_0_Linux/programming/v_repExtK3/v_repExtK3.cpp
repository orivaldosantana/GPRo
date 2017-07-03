// Copyright 2006-2017 Coppelia Robotics GmbH. All rights reserved. 
// marc@coppeliarobotics.com
// www.coppeliarobotics.com
// 
// -------------------------------------------------------------------
// THIS FILE IS DISTRIBUTED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
// WARRANTY. THE USER WILL USE IT AT HIS/HER OWN RISK. THE ORIGINAL
// AUTHORS AND COPPELIA ROBOTICS GMBH WILL NOT BE LIABLE FOR DATA LOSS,
// DAMAGES, LOSS OF PROFITS OR ANY OTHER KIND OF LOSS WHILE USING OR
// MISUSING THIS SOFTWARE.
// 
// You are free to use/modify/distribute this file for whatever purpose!
// -------------------------------------------------------------------
//
// This file was automatically created for V-REP release V3.4.0 rev. 1 on April 5th 2017

#include "v_repExtK3.h"
#include "scriptFunctionData.h"
#include <iostream>
#include "v_repLib.h"
#include <math.h>
#include <sstream>
#include <iomanip>

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
#endif /* __linux || __APPLE__ */

#define CONCAT(x,y,z) x y z
#define strConCat(x,y,z)    CONCAT(x,y,z)

LIBRARY vrepLib;

struct sK3
{
    int k3BaseHandle;
    int handle;
    int wheelMotorHandles[2];
    int colorSensorHandles[2];
    int irSensorHandles[9];
    int usSensorHandles[5];
    int armMotorHandles[6];
    int fingerMotorHandles[3];
    int gripperDistanceSensorHandles[2];
    int gripperColorSensorHandles[2];
    int uiHandle;
    char* waitUntilZero;

    float maxVelocity;
    float maxAcceleration;
    float maxArmAcceleration;
    float targetVelocities[2];
    float currentVelocities[2];
    float cumulativeMotorAngles[2];
    float previousMotorAngles[2];
    float targetArmPosition;
    float currentArmPosition;
    float currentArmVelocity;
    float targetGripperGap;
    float currentGripperGap;
};

std::vector<sK3> allK3s;
int nextK3Handle=0;

int getK3IndexFromHandle(int k3Handle)
{
    for (unsigned int i=0;i<allK3s.size();i++)
    {
        if (allK3s[i].handle==k3Handle)
            return(i);
    }
    return(-1);
}

// --------------------------------------------------------------------------------------
// simExtK3_create
// --------------------------------------------------------------------------------------
#define LUA_CREATE_COMMAND "simExtK3_create"

const int inArgs_CREATE[]={
    9,
    sim_script_arg_int32|sim_script_arg_table,2, // wheel motor handles
    sim_script_arg_int32|sim_script_arg_table,2, // color sensor handles
    sim_script_arg_int32|sim_script_arg_table,9, // IR sensor handles
    sim_script_arg_int32|sim_script_arg_table,5, // US sensor handles
    sim_script_arg_int32|sim_script_arg_table,6, // arm motor handles
    sim_script_arg_int32|sim_script_arg_table,3, // finger motor handles
    sim_script_arg_int32|sim_script_arg_table,2, // gripper distance sensor handles
    sim_script_arg_int32|sim_script_arg_table,2, // gripper color sensor handles
    sim_script_arg_int32,0, // UI handle
};

void LUA_CREATE_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    int handle=-1;
    if (D.readDataFromStack(p->stackID,inArgs_CREATE,inArgs_CREATE[0],LUA_CREATE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        sK3 k3;
        k3.k3BaseHandle=p->objectID;
        handle=nextK3Handle++;
        k3.handle=handle;
        k3.waitUntilZero=NULL;
        for (unsigned int i=0;i<2;i++)
            k3.wheelMotorHandles[i]=inData->at(0).int32Data[i];
        for (unsigned int i=0;i<2;i++)
            k3.colorSensorHandles[i]=inData->at(1).int32Data[i];
        for (unsigned int i=0;i<9;i++)
            k3.irSensorHandles[i]=inData->at(2).int32Data[i];
        for (unsigned int i=0;i<5;i++)
            k3.usSensorHandles[i]=inData->at(3).int32Data[i];
        for (unsigned int i=0;i<6;i++)
            k3.armMotorHandles[i]=inData->at(4).int32Data[i];
        for (unsigned int i=0;i<3;i++)
            k3.fingerMotorHandles[i]=inData->at(5).int32Data[i];
        for (unsigned int i=0;i<2;i++)
            k3.gripperDistanceSensorHandles[i]=inData->at(6).int32Data[i];
        for (unsigned int i=0;i<2;i++)
            k3.gripperColorSensorHandles[i]=inData->at(7).int32Data[i];
        k3.uiHandle=inData->at(8).int32Data[0];

        k3.maxVelocity=6.283f;
        k3.maxAcceleration=25.0f;
        k3.maxArmAcceleration=0.5f;
        k3.targetVelocities[0]=0.0f;
        k3.targetVelocities[1]=0.0f;
        k3.currentVelocities[0]=0.0f;
        k3.currentVelocities[1]=0.0f;
        k3.cumulativeMotorAngles[0]=0.0f;
        k3.cumulativeMotorAngles[1]=0.0f;
        k3.previousMotorAngles[0]=0.0f;
        k3.previousMotorAngles[1]=0.0f;
        k3.targetArmPosition=0.0f;
        k3.currentArmPosition=0.0f;
        k3.currentArmVelocity=0.0f;
        k3.targetGripperGap=0.055f;
        k3.currentGripperGap=0.055f;

        allK3s.push_back(k3);
    }
    D.pushOutData(CScriptFunctionDataItem(handle));
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------
// simExtK3_destroy
// --------------------------------------------------------------------------------------
#define LUA_DESTROY_COMMAND "simExtK3_destroy"

const int inArgs_DESTROY[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_DESTROY_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    bool success=false;
    if (D.readDataFromStack(p->stackID,inArgs_DESTROY,inArgs_DESTROY[0],LUA_DESTROY_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int handle=inData->at(0).int32Data[0];
        int k3Index=getK3IndexFromHandle(handle);
        if (k3Index>=0)
        {
            if (allK3s[k3Index].waitUntilZero!=NULL)
                allK3s[k3Index].waitUntilZero[0]=0; // free the blocked thread
            allK3s.erase(allK3s.begin()+k3Index);
            success=true;
        }
        else
            simSetLastError(LUA_DESTROY_COMMAND,"Invalid Khepera3 handle.");
    }
    D.pushOutData(CScriptFunctionDataItem(success));
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------
// simExtK3_getInfrared
// --------------------------------------------------------------------------------------
#define LUA_GETINFRARED_COMMAND "simExtK3_getInfrared"

const int inArgs_GETINFRARED[]={
    2,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETINFRARED_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    bool success=false;
    float distance=100.0f; // no detection
    if (D.readDataFromStack(p->stackID,inArgs_GETINFRARED,inArgs_GETINFRARED[0],LUA_GETINFRARED_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int k3Index=getK3IndexFromHandle(inData->at(0).int32Data[0]);
        int sensorIndex=inData->at(1).int32Data[0];
        if (k3Index!=-1)
        {
            if ( (sensorIndex>=0)&&(sensorIndex<9) )
            {
                float ptAndDist[4];
                if (((simGetExplicitHandling(allK3s[k3Index].irSensorHandles[sensorIndex])&1)==0)&&(simReadProximitySensor(allK3s[k3Index].irSensorHandles[sensorIndex],ptAndDist,NULL,NULL)>0))
                    distance=ptAndDist[3];
                success=true;
            }
            else
                simSetLastError(LUA_GETINFRARED_COMMAND,"Invalid index.");
        }
        else
            simSetLastError(LUA_GETINFRARED_COMMAND,"Invalid Khepera3 handle.");
    }
    if (success)
        D.pushOutData(CScriptFunctionDataItem(distance));
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simExtK3_getUltrasonic
// --------------------------------------------------------------------------------------
#define LUA_GETULTRASONIC_COMMAND "simExtK3_getUltrasonic"

const int inArgs_GETULTRASONIC[]={
    2,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETULTRASONIC_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    bool success=false;
    float distance=100.0f; // no detection
    if (D.readDataFromStack(p->stackID,inArgs_GETULTRASONIC,inArgs_GETULTRASONIC[0],LUA_GETULTRASONIC_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int k3Index=getK3IndexFromHandle(inData->at(0).int32Data[0]);
        int sensorIndex=inData->at(1).int32Data[0];
        if (k3Index!=-1)
        {
            if ( (sensorIndex>=0)&&(sensorIndex<5) )
            {
                float ptAndDist[4];
                if (((simGetExplicitHandling(allK3s[k3Index].usSensorHandles[sensorIndex])&1)==0)&&(simReadProximitySensor(allK3s[k3Index].usSensorHandles[sensorIndex],ptAndDist,NULL,NULL)>0))
                    distance=ptAndDist[3];
                success=true;
            }
            else
                simSetLastError(LUA_GETULTRASONIC_COMMAND,"Invalid index.");
        }
        else
            simSetLastError(LUA_GETULTRASONIC_COMMAND,"Invalid Khepera3 handle.");
    }
    if (success)
        D.pushOutData(CScriptFunctionDataItem(distance));
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simExtK3_getLineSensor
// --------------------------------------------------------------------------------------
#define LUA_GETLINESENSOR_COMMAND "simExtK3_getLineSensor"

const int inArgs_GETLINESENSOR[]={
    2,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETLINESENSOR_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    bool success=false;
    float intensity=0.0f; // no detection
    if (D.readDataFromStack(p->stackID,inArgs_GETLINESENSOR,inArgs_GETLINESENSOR[0],LUA_GETLINESENSOR_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int k3Index=getK3IndexFromHandle(inData->at(0).int32Data[0]);
        int sensorIndex=inData->at(1).int32Data[0];
        if (k3Index!=-1)
        {
            if ( (sensorIndex>=0)&&(sensorIndex<2) )
            {
                float* auxValues=NULL;
                int* auxValuesCount=NULL;
                if (simReadVisionSensor(allK3s[k3Index].colorSensorHandles[sensorIndex],&auxValues,&auxValuesCount)>=0)
                {
                    if ((auxValuesCount[0]>0)||(auxValuesCount[1]>=15))
                        intensity=auxValues[10];
                    simReleaseBuffer((char*)auxValues);
                    simReleaseBuffer((char*)auxValuesCount);
                }
                success=true;
            }
            else
                simSetLastError(LUA_GETLINESENSOR_COMMAND,"Invalid index.");
        }
        else
            simSetLastError(LUA_GETLINESENSOR_COMMAND,"Invalid Khepera3 handle.");
    }
    if (success)
        D.pushOutData(CScriptFunctionDataItem(intensity));
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simExtK3_getEncoder
// --------------------------------------------------------------------------------------
#define LUA_GETENCODER_COMMAND "simExtK3_getEncoder"

const int inArgs_GETENCODER[]={
    2,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETENCODER_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    bool success=false;
    int encoder=0;
    if (D.readDataFromStack(p->stackID,inArgs_GETENCODER,inArgs_GETENCODER[0],LUA_GETENCODER_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int k3Index=getK3IndexFromHandle(inData->at(0).int32Data[0]);
        int encoderIndex=inData->at(1).int32Data[0];
        if (k3Index!=-1)
        {
            if ( (encoderIndex>=0)&&(encoderIndex<2) )
            {
                encoder=int(float(2764)*allK3s[k3Index].cumulativeMotorAngles[encoderIndex]/(2.0f*3.1415f)); // 2764 are the impulses per turn
                success=true;
            }
            else
                simSetLastError(LUA_GETENCODER_COMMAND,"Invalid index.");
        }
        else
            simSetLastError(LUA_GETENCODER_COMMAND,"Invalid Khepera3 handle.");
    }
    if (success)
        D.pushOutData(CScriptFunctionDataItem(encoder));
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simExtK3_getGripperProxSensor
// --------------------------------------------------------------------------------------
#define LUA_GETGRIPPERPROXSENSOR_COMMAND "simExtK3_getGripperProxSensor"

const int inArgs_GETGRIPPERPROXSENSOR[]={
    2,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETGRIPPERPROXSENSOR_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    bool success=false;
    float distance=100.0f;
    if (D.readDataFromStack(p->stackID,inArgs_GETGRIPPERPROXSENSOR,inArgs_GETGRIPPERPROXSENSOR[0],LUA_GETGRIPPERPROXSENSOR_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int k3Index=getK3IndexFromHandle(inData->at(0).int32Data[0]);
        int sensorIndex=inData->at(1).int32Data[0];
        if (k3Index!=-1)
        {
            if ( (sensorIndex>=0)&&(sensorIndex<2) )
            {
                float ptAndDist[4];
                if (((simGetExplicitHandling(allK3s[k3Index].gripperDistanceSensorHandles[sensorIndex])&1)==0)&&(simReadProximitySensor(allK3s[k3Index].gripperDistanceSensorHandles[sensorIndex],ptAndDist,NULL,NULL)>0))
                    distance=ptAndDist[3];
                success=true;
            }
            else
                simSetLastError(LUA_GETGRIPPERPROXSENSOR_COMMAND,"Invalid index.");
        }
        else
            simSetLastError(LUA_GETGRIPPERPROXSENSOR_COMMAND,"Invalid Khepera3 handle.");
    }
    if (success)
        D.pushOutData(CScriptFunctionDataItem(distance));
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simExtK3_setArmPosition
// --------------------------------------------------------------------------------------
#define LUA_SETARMPOSITION_COMMAND "simExtK3_setArmPosition"

const int inArgs_SETARMPOSITION[]={
    2,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_SETARMPOSITION_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    bool success=false;
    if (D.readDataFromStack(p->stackID,inArgs_SETARMPOSITION,inArgs_SETARMPOSITION[0],LUA_SETARMPOSITION_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int k3Index=getK3IndexFromHandle(inData->at(0).int32Data[0]);
        int position=inData->at(1).int32Data[0];
        if (k3Index!=-1)
        {
            if (position>900)
                position=900;
            if (position<300)
                position=300;
            allK3s[k3Index].targetArmPosition=(1.0f-float(position-300)/600.0f)*195.0f*3.1415f/180.0f;
            success=true;
        }
        else
            simSetLastError(LUA_SETARMPOSITION_COMMAND,"Invalid Khepera3 handle.");
    }
    D.pushOutData(CScriptFunctionDataItem(success));
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simExtK3_setGripperGap
// --------------------------------------------------------------------------------------
#define LUA_SETGRIPPERGAP_COMMAND "simExtK3_setGripperGap"

const int inArgs_SETGRIPPERGAP[]={
    2,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_SETGRIPPERGAP_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    bool success=false;
    if (D.readDataFromStack(p->stackID,inArgs_SETGRIPPERGAP,inArgs_SETGRIPPERGAP[0],LUA_SETGRIPPERGAP_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int k3Index=getK3IndexFromHandle(inData->at(0).int32Data[0]);
        int gap=inData->at(1).int32Data[0];
        if (k3Index!=-1)
        {
            if (gap>170)
                gap=170;
            if (gap<0)
                gap=0;
            allK3s[k3Index].targetGripperGap=0.055f*float(gap)/170.0f;
            success=true;
        }
        else
            simSetLastError(LUA_SETGRIPPERGAP_COMMAND,"Invalid Khepera3 handle.");
    }
    D.pushOutData(CScriptFunctionDataItem(success));
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simExtK3_setVelocity
// --------------------------------------------------------------------------------------
#define LUA_SETVELOCITY_COMMAND "simExtK3_setVelocity"

const int inArgs_SETVELOCITY[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_float,0,
    sim_script_arg_float,0,
};

void LUA_SETVELOCITY_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    bool success=false;
    if (D.readDataFromStack(p->stackID,inArgs_SETVELOCITY,inArgs_SETVELOCITY[0],LUA_SETVELOCITY_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int k3Index=getK3IndexFromHandle(inData->at(0).int32Data[0]);
        float leftVel=inData->at(1).floatData[0];
        float rightVel=inData->at(2).floatData[0];
        if (k3Index!=-1)
        {
            if (leftVel>allK3s[k3Index].maxVelocity)
                leftVel=allK3s[k3Index].maxVelocity;
            if (leftVel<-allK3s[k3Index].maxVelocity)
                leftVel=-allK3s[k3Index].maxVelocity;
            if (rightVel>allK3s[k3Index].maxVelocity)
                rightVel=allK3s[k3Index].maxVelocity;
            if (rightVel<-allK3s[k3Index].maxVelocity)
                rightVel=-allK3s[k3Index].maxVelocity;
            allK3s[k3Index].targetVelocities[0]=leftVel;
            allK3s[k3Index].targetVelocities[1]=rightVel;
            success=true;
        }
        else
            simSetLastError(LUA_SETVELOCITY_COMMAND,"Invalid Khepera3 handle.");
    }
    D.pushOutData(CScriptFunctionDataItem(success));
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simExtK3_setEncoders
// --------------------------------------------------------------------------------------
#define LUA_SETENCODERS_COMMAND "simExtK3_setEncoders"

const int inArgs_SETENCODERS[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_SETENCODERS_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    bool success=false;
    if (D.readDataFromStack(p->stackID,inArgs_SETENCODERS,inArgs_SETENCODERS[0],LUA_SETENCODERS_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int k3Index=getK3IndexFromHandle(inData->at(0).int32Data[0]);
        int leftEncoder=inData->at(1).int32Data[0];
        int rightEncoder=inData->at(2).int32Data[0];
        if (k3Index!=-1)
        {
            allK3s[k3Index].cumulativeMotorAngles[0]=float(leftEncoder)*(2.0f*3.1415f)/float(2764); // 2764 are the impulses per turn
            allK3s[k3Index].cumulativeMotorAngles[1]=float(rightEncoder)*(2.0f*3.1415f)/float(2764);
            success=true;
        }
        else
            simSetLastError(LUA_SETENCODERS_COMMAND,"Invalid Khepera3 handle.");
    }
    D.pushOutData(CScriptFunctionDataItem(success));
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

VREP_DLLEXPORT unsigned char v_repStart(void* reservedPointer,int reservedInt)
{ // This is called just once, at the start of V-REP.
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
        std::cout << "Error, could not find or correctly load v_rep.dll. Cannot start 'K3' plugin.\n";
        return(0); // Means error, V-REP will unload this plugin
    }
    if (getVrepProcAddresses(vrepLib)==0)
    {
        std::cout << "Error, could not find all required functions in v_rep.dll. Cannot start 'K3' plugin.\n";
        unloadVrepLibrary(vrepLib);
        return(0); // Means error, V-REP will unload this plugin
    }

    // Check the V-REP version:
    int vrepVer;
    simGetIntegerParameter(sim_intparam_program_version,&vrepVer);
    if (vrepVer<30200) // if V-REP version is smaller than 3.02.00
    {
        std::cout << "Sorry, your V-REP copy is somewhat old, V-REP 3.2.0 or higher is required. Cannot start 'K3' plugin.\n";
        unloadVrepLibrary(vrepLib);
        return(0); // Means error, V-REP will unload this plugin
    }

    // Register new Lua commands:
    simRegisterScriptCallbackFunction(strConCat(LUA_CREATE_COMMAND,"@","K3"),strConCat("number k3Handle=",LUA_CREATE_COMMAND,"(table_2 wheelMotorHandles,table_2 colorSensorHandles,table_9 IrSensorHandles,table_5 usSensorHandles,table_6 armMotorHandles,table_3 fingerMotorHandles,table_2 gripperDistSensHandles,table_2 gripperColSensHandles,number uiHandle)"),LUA_CREATE_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_DESTROY_COMMAND,"@","K3"),strConCat("boolean result=",LUA_DESTROY_COMMAND,"(number k3Handle)"),LUA_DESTROY_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETINFRARED_COMMAND,"@","K3"),strConCat("number distance=",LUA_GETINFRARED_COMMAND,"(number k3Handle,number index)"),LUA_GETINFRARED_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETULTRASONIC_COMMAND,"@","K3"),strConCat("number distance=",LUA_GETULTRASONIC_COMMAND,"(number k3Handle,number index)"),LUA_GETULTRASONIC_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETLINESENSOR_COMMAND,"@","K3"),strConCat("number intensity=",LUA_GETLINESENSOR_COMMAND,"(number k3Handle,number index)"),LUA_GETLINESENSOR_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETENCODER_COMMAND,"@","K3"),strConCat("number encoderValue=",LUA_GETENCODER_COMMAND,"(number k3Handle,number index)"),LUA_GETENCODER_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETGRIPPERPROXSENSOR_COMMAND,"@","K3"),strConCat("number distance=",LUA_GETGRIPPERPROXSENSOR_COMMAND,"(number k3Handle,number index)"),LUA_GETGRIPPERPROXSENSOR_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_SETARMPOSITION_COMMAND,"@","K3"),strConCat("boolean result=",LUA_SETARMPOSITION_COMMAND,"(number k3Handle,number position)"),LUA_SETARMPOSITION_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_SETGRIPPERGAP_COMMAND,"@","K3"),strConCat("boolean result=",LUA_SETGRIPPERGAP_COMMAND,"(number k3Handle,number gap)"),LUA_SETGRIPPERGAP_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_SETVELOCITY_COMMAND,"@","K3"),strConCat("boolean result=",LUA_SETVELOCITY_COMMAND,"(number k3Handle,number leftVelocity,number rightVelocity)"),LUA_SETVELOCITY_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_SETENCODERS_COMMAND,"@","K3"),strConCat("boolean result=",LUA_SETENCODERS_COMMAND,"(number k3Handle,number leftEncoderValue,number rightEncoderValue)"),LUA_SETENCODERS_CALLBACK);

    return(7); // initialization went fine, we return the version number of this plugin (can be queried with simGetModuleName)
    // version 1 was for V-REP 2.5.11 and below
    // version 2 was for V-REP 2.5.12 and below
    // version 3 was for V-REP 3.0.5 and below
    // version 4 was for V-REP 3.1.3 and below
    // version 6 is for V-REP versions after V-REP 3.2.0 (completely rewritten)
    // version 7 is for V-REP versions after V-REP 3.3.0 (using stacks to exchange data with scripts)
}

VREP_DLLEXPORT void v_repEnd()
{ // This is called just once, at the end of V-REP
    unloadVrepLibrary(vrepLib); // release the library
}

VREP_DLLEXPORT void* v_repMessage(int message,int* auxiliaryData,void* customData,int* replyData)
{ // This is called quite often. Just watch out for messages/events you want to handle
    // This function should not generate any error messages:
    int errorModeSaved;
    simGetIntegerParameter(sim_intparam_error_report_mode,&errorModeSaved);
    simSetIntegerParameter(sim_intparam_error_report_mode,sim_api_errormessage_ignore);

    void* retVal=NULL;

    if (message==sim_message_eventcallback_modulehandle)
    {
        if ( (customData==NULL)||(std::string("K3").compare((char*)customData)==0) ) // is the command also meant for Khepera3?
        {
            float dt=simGetSimulationTimeStep();
            for (unsigned int k3Index=0;k3Index<allK3s.size();k3Index++)
            {
                // 1. Run the robot control:
                for (int i=0;i<2;i++)
                {
                    if (allK3s[k3Index].targetVelocities[i]>allK3s[k3Index].currentVelocities[i])
                    {
                        allK3s[k3Index].currentVelocities[i]=allK3s[k3Index].currentVelocities[i]+allK3s[k3Index].maxAcceleration*dt;
                        if (allK3s[k3Index].currentVelocities[i]>allK3s[k3Index].targetVelocities[i])
                            allK3s[k3Index].currentVelocities[i]=allK3s[k3Index].targetVelocities[i];
                    }
                    else
                    {
                        allK3s[k3Index].currentVelocities[i]=allK3s[k3Index].currentVelocities[i]-allK3s[k3Index].maxAcceleration*dt;
                        if (allK3s[k3Index].currentVelocities[i]<allK3s[k3Index].targetVelocities[i])
                            allK3s[k3Index].currentVelocities[i]=allK3s[k3Index].targetVelocities[i];
                    }
                    simSetJointTargetVelocity(allK3s[k3Index].wheelMotorHandles[i],allK3s[k3Index].currentVelocities[i]);
                    float jp;
                    simGetJointPosition(allK3s[k3Index].wheelMotorHandles[i],&jp);
                    float dp=jp-allK3s[k3Index].previousMotorAngles[i];
                    if (fabs(dp)>3.1415f)
                        dp-=(2.0f*3.1415f*fabs(dp)/dp);
                    allK3s[k3Index].cumulativeMotorAngles[i]+=dp; // corrected on 5/3/2012 thanks to Felix Fischer
                    allK3s[k3Index].previousMotorAngles[i]=jp;
                }


                float adp=allK3s[k3Index].targetArmPosition-allK3s[k3Index].currentArmPosition;
                if (adp!=0.0f)
                {
                    if (adp*allK3s[k3Index].currentArmVelocity>=0.0f)
                    {
                        float decelToZeroTime=(fabs(allK3s[k3Index].currentArmVelocity)+allK3s[k3Index].maxArmAcceleration*dt*1.0f)/allK3s[k3Index].maxArmAcceleration;
                        float distToZero=0.5f*allK3s[k3Index].maxArmAcceleration*decelToZeroTime*decelToZeroTime;
                        float dir=1.0f;
                        if (allK3s[k3Index].currentArmVelocity!=0.0f)
                            dir=allK3s[k3Index].currentArmVelocity/fabs(allK3s[k3Index].currentArmVelocity);
                        else
                            dir=adp/fabs(adp);
                        if (fabs(adp)>distToZero)
                            allK3s[k3Index].currentArmVelocity+=dir*allK3s[k3Index].maxArmAcceleration*dt;
                        else
                            allK3s[k3Index].currentArmVelocity-=dir*allK3s[k3Index].maxArmAcceleration*dt;
                    }
                    else
                        allK3s[k3Index].currentArmVelocity*=(1-allK3s[k3Index].maxArmAcceleration*dt/fabs(allK3s[k3Index].currentArmVelocity));
                }
                else
                {
                    if (allK3s[k3Index].currentArmVelocity!=0.0f)
                    {
                        float dv=-allK3s[k3Index].currentArmVelocity*allK3s[k3Index].maxArmAcceleration*dt/fabs(allK3s[k3Index].currentArmVelocity);
                        if ((allK3s[k3Index].currentArmVelocity+dv)*allK3s[k3Index].currentArmVelocity<0.0f)
                            allK3s[k3Index].currentArmVelocity=0.0f;
                        else
                            allK3s[k3Index].currentArmVelocity+=dv;
                    }
                }

                allK3s[k3Index].currentArmPosition+=allK3s[k3Index].currentArmVelocity*dt;

                simSetJointTargetPosition(allK3s[k3Index].armMotorHandles[0],allK3s[k3Index].currentArmPosition);
                simSetJointTargetPosition(allK3s[k3Index].armMotorHandles[1],-allK3s[k3Index].currentArmPosition);
                simSetJointTargetPosition(allK3s[k3Index].armMotorHandles[2],allK3s[k3Index].currentArmPosition);
                simSetJointTargetPosition(allK3s[k3Index].armMotorHandles[3],allK3s[k3Index].currentArmPosition);
                simSetJointTargetPosition(allK3s[k3Index].armMotorHandles[4],allK3s[k3Index].currentArmPosition);
                simSetJointTargetPosition(allK3s[k3Index].armMotorHandles[5],allK3s[k3Index].currentArmPosition);

                float jp;
                simGetJointPosition(allK3s[k3Index].fingerMotorHandles[0],&jp);
                allK3s[k3Index].currentGripperGap=(jp)+0.04f;
                float dp=allK3s[k3Index].targetGripperGap-allK3s[k3Index].currentGripperGap;
                float velToRegulate=0.0f;
                if (fabs(dp)<0.005f)
                {
                    if (dp!=0.0f)
                        velToRegulate=(fabs(dp)/0.005f)*0.045f*dp/fabs(dp);
                }
                else
                    velToRegulate=0.045f*dp/fabs(dp);
                simSetJointTargetVelocity(allK3s[k3Index].fingerMotorHandles[0],velToRegulate); 
                simSetJointTargetPosition(allK3s[k3Index].fingerMotorHandles[1],-jp*0.5f); 
                simSetJointTargetPosition(allK3s[k3Index].fingerMotorHandles[2],-jp*0.5f);

                // 2. Update the robot's UI:
                // IR sensors on the base:
                float ptAndDist[4];
                std::string s;
                for (int i=0;i<9;i++)
                {
                    if (((simGetExplicitHandling(allK3s[k3Index].irSensorHandles[i])&1)==0)&&(simReadProximitySensor(allK3s[k3Index].irSensorHandles[i],ptAndDist,NULL,NULL)>0))
                    {
                        simSetUIButtonLabel(allK3s[k3Index].uiHandle,110+i,"&&Box",NULL);
                        std::stringstream out;
                        out << int(ptAndDist[3]*1000.0f);
                        s=out.str();
                        simSetUIButtonLabel(allK3s[k3Index].uiHandle,210+i,s.c_str(),NULL);
                    }
                    else
                    {
                        simSetUIButtonLabel(allK3s[k3Index].uiHandle,110+i,"",NULL);
                        simSetUIButtonLabel(allK3s[k3Index].uiHandle,210+i,"",NULL);
                    }
                }
                
                // UI title:
                char* objName=simGetObjectName(allK3s[k3Index].k3BaseHandle);
                if (objName!=NULL)
                {
                    std::string nm(objName);
                    simReleaseBuffer(objName);
                    nm+=" state visualization";
                    simSetUIButtonLabel(allK3s[k3Index].uiHandle,0,nm.c_str(),NULL);
                }

                // US sensors on the base:
                for (int i=0;i<5;i++)
                {
                    if (((simGetExplicitHandling(allK3s[k3Index].usSensorHandles[i])&1)==0)&&(simReadProximitySensor(allK3s[k3Index].usSensorHandles[i],ptAndDist,NULL,NULL)>0))
                    {
                        simSetUIButtonLabel(allK3s[k3Index].uiHandle,100+i,"&&Box",NULL);
                        std::stringstream out;
                        out << int(ptAndDist[3]*1000.0f);
                        s=out.str();
                        simSetUIButtonLabel(allK3s[k3Index].uiHandle,200+i,s.c_str(),NULL);
                    }
                    else
                    {
                        simSetUIButtonLabel(allK3s[k3Index].uiHandle,100+i,"",NULL);
                        simSetUIButtonLabel(allK3s[k3Index].uiHandle,200+i,"",NULL);
                    }
                }
                // Color sensors on base:
                for (int i=0;i<2;i++)
                {
                    float* auxValues=NULL;
                    int* auxValuesCount=NULL;
                    float col[3]={0.0f,0.0f,0.0f};
                    if (simReadVisionSensor(allK3s[k3Index].colorSensorHandles[i],&auxValues,&auxValuesCount)>=0)
                    {
                        if ((auxValuesCount[0]>0)||(auxValuesCount[1]>=15))
                        {
                            col[0]=auxValues[11];
                            col[1]=auxValues[12];
                            col[2]=auxValues[13];
                        }
                        simReleaseBuffer((char*)auxValues);
                        simReleaseBuffer((char*)auxValuesCount);
                    }
                    simSetUIButtonColor(allK3s[k3Index].uiHandle,300+i,col,NULL,NULL);
                }

                // Base motor velocities and encoders:
                for (int i=0;i<2;i++)
                {
                    std::stringstream out1,out2;
                    out1 << std::fixed << std::setprecision(1) << allK3s[k3Index].currentVelocities[i]*180.0f/3.1415f;
                    s=out1.str();
                    simSetUIButtonLabel(allK3s[k3Index].uiHandle,320+i,s.c_str(),NULL);

                    out2 << int(float(2764)*allK3s[k3Index].cumulativeMotorAngles[i]/(2.0f*3.1415f));
                    s=out2.str();
                    simSetUIButtonLabel(allK3s[k3Index].uiHandle,310+i,s.c_str(),NULL);
                }

                // Arm position:
                std::stringstream out;
                out << int((1.0f-(allK3s[k3Index].currentArmPosition*180.0f/(195.0f*3.1415f)))*600.0f+300.0f);
                s=out.str();
                simSetUIButtonLabel(allK3s[k3Index].uiHandle,120,s.c_str(),NULL);
                simSetUISlider(allK3s[k3Index].uiHandle,121,int(1000.0f*allK3s[k3Index].currentArmPosition*180.0f/(195.0f*3.1415f)));

                // Finger motors:
                out.str("");
                out << int(allK3s[k3Index].currentGripperGap*1000.0f);
                s=out.str();
                simSetUIButtonLabel(allK3s[k3Index].uiHandle,130,s.c_str(),NULL);
                simSetUISlider(allK3s[k3Index].uiHandle,131,int(0.5f+(1.0f-allK3s[k3Index].currentGripperGap/0.055f)*1000.0f));
                simSetUISlider(allK3s[k3Index].uiHandle,132,int(0.5f+(allK3s[k3Index].currentGripperGap/0.055f)*1000.0f));


                // Gripper proximity sensors:
                for (int i=0;i<2;i++)
                {
                    if (((simGetExplicitHandling(allK3s[k3Index].gripperDistanceSensorHandles[i])&1)==0)&&(simReadProximitySensor(allK3s[k3Index].gripperDistanceSensorHandles[i],ptAndDist,NULL,NULL)>0))
                    {
                        out.str("");
                        out << int(ptAndDist[3]*1000.0f);
                        s=out.str();
                        simSetUIButtonLabel(allK3s[k3Index].uiHandle,133+i,s.c_str(),NULL);
                    }
                    else
                        simSetUIButtonLabel(allK3s[k3Index].uiHandle,133+i,"",NULL);
                }
                // Gripper color sensors:
                for (int i=0;i<2;i++)
                {
                    float* auxValues=NULL;
                    int* auxValuesCount=NULL;
                    float col[3]={0.0f,0.0f,0.0f};
                    if (simReadVisionSensor(allK3s[k3Index].gripperColorSensorHandles[i],&auxValues,&auxValuesCount)>=0)
                    {
                        if ((auxValuesCount[0]>0)||(auxValuesCount[1]>=15))
                        {
                            col[0]=auxValues[11];
                            col[1]=auxValues[12];
                            col[2]=auxValues[13];
                        }
                        simReleaseBuffer((char*)auxValues);
                        simReleaseBuffer((char*)auxValuesCount);
                    }
                    simSetUIButtonColor(allK3s[k3Index].uiHandle,135+i,col,NULL,NULL);
                }

            }
        }
    }

    if (message==sim_message_eventcallback_simulationended)
    { // simulation ended. Destroy all Khepera3 instances:
        allK3s.clear();
    }

    simSetIntegerParameter(sim_intparam_error_report_mode,errorModeSaved); // restore previous settings
    return(retVal);
}
