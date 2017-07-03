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

#include "v_repExtMtb.h"
#include "v_repLib.h"
#include "scriptFunctionData.h"
#include "socketOutConnection.h"
#include <iostream>
#include <boost/lexical_cast.hpp>

#ifdef _WIN32
#ifdef QT_COMPIL
    #include <direct.h>
#else
    #include <Shellapi.h>
    #include <shlwapi.h>
    #pragma comment(lib, "Shlwapi.lib")
    #pragma comment(lib, "Shell32.lib")
#endif
#endif

#ifdef QT_COMPIL
    #include <QString>
    #include <QProcess>
#endif

#if defined (__linux) || defined (__APPLE__)
    #include <unistd.h>
    #define _stricmp(x,y) strcasecmp(x,y)
#endif

#define PLUGIN_VERSION 8
                // version 1 was for V-REP 2.5.11 or earlier
                // version 2 was for V-REP versions before V-REP 2.6.0
                // version 3 was for V-REP versions before V-REP 2.6.7
                // version 4 is the Qt version
                // version 5 is from 10/1/2014
                // version 6 is after V-REP 3.1.3
                // version 7 is after V-REP 3.2.0. Completely rewritten.
                // version 8 is after V-REP 3.3.0. Using stacks to exchange data with scripts.

#define CONCAT(x,y,z) x y z
#define strConCat(x,y,z)    CONCAT(x,y,z)

LIBRARY vrepLib;

struct sMtbServer
{
    CSocketOutConnection* connection;
    int mtbServerHandle;
    float jointPositions[4];
    unsigned char robotInput[4];
    unsigned char robotOutput[4];
    std::vector<int> inputOutputConnections; // indicates which input buffer bit is linked with what output buffer bit:
                                            // value[0+4*n] = input bit number (1-32) of this robot 
                                            // value[1+4*n] = handle of the server robot on which we want to connect an output
                                            // value[2+4*n] = output bit number (1-32) of the other robot (see above) 
                                            // value[3+4*n] = connection type (0=same, 1=inverted) 
};

std::vector<sMtbServer> allMtbServers;
int nextMtbServerHandle=0;
std::string currentDirAndPath;

int getServerIndexFromServerHandle(int serverHandle)
{
    for (unsigned int i=0;i<allMtbServers.size();i++)
    {
        if (allMtbServers[i].mtbServerHandle==serverHandle)
            return(i);
    }
    return(-1);
}

void removeOutputInputConnection(int inputIndex,int inputBit)
{
    if ((inputIndex>=0)&&(inputIndex<int(allMtbServers.size())))
    {
        if (inputBit==0)
            allMtbServers[inputIndex].inputOutputConnections.clear();
        else
        {
            for (unsigned int i=0;i<allMtbServers[inputIndex].inputOutputConnections.size()/4;i++)
            {
                if (allMtbServers[inputIndex].inputOutputConnections[4*i+0]==inputBit)
                {
                    allMtbServers[inputIndex].inputOutputConnections.erase(allMtbServers[inputIndex].inputOutputConnections.begin()+4*i+0,allMtbServers[inputIndex].inputOutputConnections.begin()+4*i+4);
                    break;
                }
            }
        }
    }
}

void updateInputsFromConnections(int inputIndex)
{
    if ((inputIndex>=0)&&(inputIndex<int(allMtbServers.size())))
    {
        for (unsigned int i=0;i<allMtbServers[inputIndex].inputOutputConnections.size()/4;i++)
        {
            int outputIndex=allMtbServers[inputIndex].inputOutputConnections[4*i+1];
            if ((outputIndex>=0)&&(outputIndex<int(allMtbServers.size())))
            {
                unsigned char otherRobotOutputData[4];
                for (int j=0;j<4;j++)
                    otherRobotOutputData[j]=allMtbServers[outputIndex].robotOutput[j];

                int inBytePos=(allMtbServers[inputIndex].inputOutputConnections[4*i+0]-1)/8; // 0-3
                int inBitPos=(allMtbServers[inputIndex].inputOutputConnections[4*i+0]-1)%8; //0-7

                int outBytePos=(allMtbServers[inputIndex].inputOutputConnections[4*i+2]-1)/8; // 0-3
                int outBitPos=(allMtbServers[inputIndex].inputOutputConnections[4*i+2]-1)%8; //0-7

                bool setInputBit=false;
                if (otherRobotOutputData[outBytePos]&(1<<outBitPos))
                    setInputBit=((1-allMtbServers[inputIndex].inputOutputConnections[4*i+3])!=0);
                else
                    setInputBit=(allMtbServers[inputIndex].inputOutputConnections[4*i+3]!=0);

                if (setInputBit)
                    allMtbServers[inputIndex].robotInput[inBytePos]|=(1<<inBitPos);
                else
                    allMtbServers[inputIndex].robotInput[inBytePos]&=255-(1<<inBitPos);
            }
            else
            { // the other server robot was removed. We have to remove this output-input connection:
                allMtbServers[inputIndex].inputOutputConnections.erase(allMtbServers[inputIndex].inputOutputConnections.begin()+4*i+0,allMtbServers[inputIndex].inputOutputConnections.begin()+4*i+4);
                i--; // we have to reprocess this position in the for-loop!
            }
        }
    }
}

void updateAllInputs()
{ // Since each output can influence each input, and this cascaded, we call the update n*n times, where n is the number of servers
    for (unsigned int j=0;j<allMtbServers.size();j++)
    {
        for (unsigned int i=0;i<allMtbServers.size();i++)
            updateInputsFromConnections(i);
    }
}

bool addOutputInputConnection(int inputIndex,int inputBit,int outputIndex,int outputBit,int connectionType)
{
    if ((inputIndex<0)||(inputIndex>=int(allMtbServers.size())))
        return(false);
    if ((outputIndex<0)||(outputIndex>=int(allMtbServers.size())))
        return(false);
    if ( (inputBit<1)||(inputBit>32)||(outputBit<1)||(outputBit>32)||(connectionType<0)||(connectionType>1) )
        return(false);
    removeOutputInputConnection(inputIndex,inputBit);
    //4. Add the connection:
    allMtbServers[inputIndex].inputOutputConnections.push_back(inputBit);
    allMtbServers[inputIndex].inputOutputConnections.push_back(outputIndex);
    allMtbServers[inputIndex].inputOutputConnections.push_back(outputBit);
    allMtbServers[inputIndex].inputOutputConnections.push_back(connectionType);
    return(true);
}



// --------------------------------------------------------------------------------------
// simExtMtb_startServer
// --------------------------------------------------------------------------------------
#define LUA_START_SERVER_COMMAND "simExtMtb_startServer"

const int inArgs_START_SERVER[]={
    5,
    sim_script_arg_string,0,
    sim_script_arg_int32,0,
    sim_script_arg_charbuff,0,
    sim_script_arg_float|sim_script_arg_table,4,
    sim_script_arg_float|sim_script_arg_table,2,
};

void LUA_START_SERVER_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    int handle=-1; // means error
    std::string msg("Problem launching or communicating with the MTB server.");
    if (D.readDataFromStack(p->stackID,inArgs_START_SERVER,inArgs_START_SERVER[0],LUA_START_SERVER_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        std::string mtbServerExecutableName(inData->at(0).stringData[0]);
        int portNumber=inData->at(1).int32Data[0];
        std::string arguments(boost::lexical_cast<std::string>(portNumber));
        std::string program(inData->at(2).stringData[0]);

        #ifdef QT_COMPIL
            QString cmd;
            if (mtbServerExecutableName.find('/')==std::string::npos)
                cmd="./";
            cmd+=QString(mtbServerExecutableName.c_str());
            QStringList strList;
            strList << QString(arguments.c_str());
            QProcess::startDetached(cmd,strList,QString(currentDirAndPath.c_str()),NULL);
            #ifdef _WIN32
                Sleep(1000);
            #else
                sleep(1);
            #endif
        #else
            #ifdef _WIN32
                ShellExecuteA(NULL,"open",mtbServerExecutableName.c_str(),arguments.c_str(),NULL,SW_SHOWDEFAULT);
                Sleep(1000);
            #else
                pid_t pid=fork();
                if (pid==0)
                {
                    execl(mtbServerExecutableName.c_str(),mtbServerExecutableName.c_str(),inData->at(0).stringData[0].c_str(),inData->at(1).stringData[0].c_str(),boost::lexical_cast<std::string>(inData->at(2).int32Data[0]).c_str(), (char*) 0);
                    exit(0);
                }
                sleep(1);
            #endif
        #endif
        
        CSocketOutConnection* connection=new CSocketOutConnection("127.0.0.1",portNumber);
        if (connection->connectToServer()==1)
        { // we could connect!
            // Now send the program and initial values to the server:
            std::string dat(25,0);
            dat[0]=0; // compile command
            for (unsigned int i=0;i<4;i++)
                ((float*)(&dat[1+i*4]))[0]=inData->at(3).floatData[i];
            for (unsigned int i=0;i<2;i++)
                ((float*)(&dat[1+16+i*4]))[0]=inData->at(4).floatData[i];
            dat.insert(dat.end(),program.begin(),program.end());
            if (connection->sendData(&dat[0],int(dat.size())))
            { // send was successful
                // Now wait for the status reply:
                int dataSize;
                char* data=connection->receiveReplyData(dataSize);
                if (dataSize>0)
                { // data reception was ok!
                    if (dataSize==2)
                    { // ok, the program was correctly compiled
                        handle=nextMtbServerHandle++;
                        sMtbServer server;
                        server.connection=connection;
                        server.mtbServerHandle=handle;
                        for (unsigned int i=0;i<4;i++)
                            server.jointPositions[i]=inData->at(3).floatData[i];
                        for (unsigned int i=0;i<4;i++)
                        {
                            server.robotInput[i]=0;
                            server.robotOutput[i]=0;
                        }
                        allMtbServers.push_back(server);
                        updateAllInputs();
                    }
                    else
                    { // there was a problem. Return the error string:
                        msg=std::string(data+1);
                        delete connection;
                    }
                    delete[] data;
                }
                else
                {
                    delete connection;
                    simSetLastError(LUA_START_SERVER_COMMAND,"Failed to receive data from the MTB server.");
                }
            }
            else
            {
                delete connection;
                simSetLastError(LUA_START_SERVER_COMMAND,"Failed to send data to the MTB server.");
            }
        }
        else
        {
            delete connection;
            simSetLastError(LUA_START_SERVER_COMMAND,"Failed to start or connect to MTB server.");
        }
            
    }
    else
        msg="Wrong or missing arguments.";
    D.pushOutData(CScriptFunctionDataItem(handle));
    if (handle<0)
        D.pushOutData(CScriptFunctionDataItem(msg));
    else
        D.pushOutData(CScriptFunctionDataItem(""));
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------
// simExtMtb_stopServer
// --------------------------------------------------------------------------------------
#define LUA_STOP_SERVER_COMMAND "simExtMtb_stopServer"

const int inArgs_STOP_SERVER[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_STOP_SERVER_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    bool success=false;
    if (D.readDataFromStack(p->stackID,inArgs_STOP_SERVER,inArgs_STOP_SERVER[0],LUA_STOP_SERVER_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int serverHandle=inData->at(0).int32Data[0];
        int index=getServerIndexFromServerHandle(serverHandle);
        if (index!=-1)
        {
            delete allMtbServers[index].connection;
            allMtbServers.erase(allMtbServers.begin()+index);
            updateAllInputs();
            success=true;
        }
        else
            simSetLastError(LUA_STOP_SERVER_COMMAND,"Invalid MTB server handle.");
    }
    D.pushOutData(CScriptFunctionDataItem(success));
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simExtMtb_step
// --------------------------------------------------------------------------------------
#define LUA_STEP_COMMAND "simExtMtb_step"

const int inArgs_STEP[]={
    2,
    sim_script_arg_int32,0,
    sim_script_arg_float,0,
};

void LUA_STEP_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    std::string msg("Invalid MTB server handle, or problem communicating with the MTB server.");
    int result=-1;
    if (D.readDataFromStack(p->stackID,inArgs_STEP,inArgs_STEP[0],LUA_STEP_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int serverHandle=inData->at(0).int32Data[0];
        float timeStep=inData->at(1).floatData[0];
        int index=getServerIndexFromServerHandle(serverHandle);
        if (index!=-1)
        {
            // Now send the data to the server:
            std::string dat(9,0);
            dat[0]=1; // step command
            dat[1]=allMtbServers[index].robotInput[0];
            dat[2]=allMtbServers[index].robotInput[1];
            dat[3]=allMtbServers[index].robotInput[2];
            dat[4]=allMtbServers[index].robotInput[3];
            ((float*)&dat[5])[0]=timeStep;
            if (allMtbServers[index].connection->sendData(&dat[0],int(dat.size())))
            { // send was successful
                // Now wait for the status reply:
                int dataSize;
                char* data=allMtbServers[index].connection->receiveReplyData(dataSize);
                if (dataSize>=22)
                { // data reception was ok!
                    for (unsigned int i=0;i<4;i++)
                        allMtbServers[index].jointPositions[i]=((float*)(data+1))[i];
                    for (unsigned int i=0;i<4;i++)
                        allMtbServers[index].robotOutput[i]=data[1+16+i];
                    updateAllInputs();
                    msg=std::string(data+1+16+4);
                    if (msg.length()!=0)
                        result=0; // program not yet finished
                    else
                        result=1; // program finished

                    delete[] data;
                }
                else
                    simSetLastError(LUA_STEP_COMMAND,"Failed to receive data from the MTB server.");
            }
            else
                simSetLastError(LUA_STEP_COMMAND,"Failed to send data to the MTB server.");
        }
        else
            simSetLastError(LUA_STEP_COMMAND,"Invalid MTB server handle.");
    }
    else
        msg="Wrong or missing arguments.";
    D.pushOutData(CScriptFunctionDataItem(result));
    D.pushOutData(CScriptFunctionDataItem(msg));
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simExtMtb_getJoints
// --------------------------------------------------------------------------------------
#define LUA_GET_JOINTS_COMMAND "simExtMtb_getJoints"

const int inArgs_GET_JOINTS[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_GET_JOINTS_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    std::vector<float> joints;
    if (D.readDataFromStack(p->stackID,inArgs_GET_JOINTS,inArgs_GET_JOINTS[0],LUA_GET_JOINTS_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int serverHandle=inData->at(0).int32Data[0];
        int index=getServerIndexFromServerHandle(serverHandle);
        if (index!=-1)
        {
            for (unsigned int i=0;i<4;i++)
                joints.push_back(allMtbServers[index].jointPositions[i]);
        }
        else
            simSetLastError(LUA_GET_JOINTS_COMMAND,"Invalid MTB server handle.");
    }
    if (joints.size()!=0)
        D.pushOutData(CScriptFunctionDataItem(joints));
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simExtMtb_getOutput
// --------------------------------------------------------------------------------------
#define LUA_GET_OUTPUT_COMMAND "simExtMtb_getOutput"

const int inArgs_GET_OUTPUT[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_GET_OUTPUT_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    std::vector<int> output;
    if (D.readDataFromStack(p->stackID,inArgs_GET_OUTPUT,inArgs_GET_OUTPUT[0],LUA_GET_OUTPUT_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int serverHandle=inData->at(0).int32Data[0];
        int index=getServerIndexFromServerHandle(serverHandle);
        if (index!=-1)
        {
            for (unsigned int i=0;i<4;i++)
                output.push_back(allMtbServers[index].robotOutput[i]);
        }
        else
            simSetLastError(LUA_GET_OUTPUT_COMMAND,"Invalid MTB server handle.");
    }
    if (output.size()!=0)
        D.pushOutData(CScriptFunctionDataItem(output));
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simExtMtb_getInput
// --------------------------------------------------------------------------------------
#define LUA_GET_INPUT_COMMAND "simExtMtb_getInput"

const int inArgs_GET_INPUT[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_GET_INPUT_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    std::vector<int> input;
    if (D.readDataFromStack(p->stackID,inArgs_GET_INPUT,inArgs_GET_INPUT[0],LUA_GET_INPUT_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int serverHandle=inData->at(0).int32Data[0];
        int index=getServerIndexFromServerHandle(serverHandle);
        if (index!=-1)
        {
            for (unsigned int i=0;i<4;i++)
                input.push_back(allMtbServers[index].robotInput[i]);
        }
        else
            simSetLastError(LUA_GET_INPUT_COMMAND,"Invalid MTB server handle.");
    }
    if (input.size()!=0)
        D.pushOutData(CScriptFunctionDataItem(input));
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simExtMtb_setInput
// --------------------------------------------------------------------------------------
#define LUA_SET_INPUT_COMMAND "simExtMtb_setInput"

const int inArgs_SET_INPUT[]={
    2,
    sim_script_arg_int32,0,
    sim_script_arg_int32|sim_script_arg_table,4,
};

void LUA_SET_INPUT_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    bool result=false;
    if (D.readDataFromStack(p->stackID,inArgs_SET_INPUT,inArgs_SET_INPUT[0],LUA_SET_INPUT_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int serverHandle=inData->at(0).int32Data[0];
        int index=getServerIndexFromServerHandle(serverHandle);
        if (index!=-1)
        {
            for (unsigned int i=0;i<4;i++)
                allMtbServers[index].robotInput[i]=(unsigned char)inData->at(1).int32Data[i];
            updateAllInputs();
            result=true;
        }
        else
            simSetLastError(LUA_SET_INPUT_COMMAND,"Invalid MTB server handle.");
    }
    D.pushOutData(CScriptFunctionDataItem(result));
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simExtMtb_connectInput
// --------------------------------------------------------------------------------------
#define LUA_CONNECT_INPUT_COMMAND "simExtMtb_connectInput"

const int inArgs_CONNECT_INPUT[]={
    5,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_CONNECT_INPUT_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    bool result=false;
    if (D.readDataFromStack(p->stackID,inArgs_CONNECT_INPUT,inArgs_CONNECT_INPUT[0],LUA_CONNECT_INPUT_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int inputServerHandle=inData->at(0).int32Data[0];
        int inputIndex=getServerIndexFromServerHandle(inputServerHandle);
        int inputBits=inData->at(1).int32Data[0];
        int outputServerHandle=inData->at(2).int32Data[0];
        int outputIndex=getServerIndexFromServerHandle(outputServerHandle);
        int outputBits=inData->at(3).int32Data[0];
        int connectionType=inData->at(4).int32Data[0];
        if ((inputIndex!=-1)&&(outputIndex!=-1))
        {
            result=addOutputInputConnection(inputIndex,inputBits,outputIndex,outputBits,connectionType);        
            updateAllInputs();
        }
        else
            simSetLastError(LUA_CONNECT_INPUT_COMMAND,"Invalid MTB server handle.");
    }
    D.pushOutData(CScriptFunctionDataItem(result));
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simExtMtb_disconnectInput
// --------------------------------------------------------------------------------------
#define LUA_DISCONNECT_INPUT_COMMAND "simExtMtb_disconnectInput"

const int inArgs_DISCONNECT_INPUT[]={
    2,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_DISCONNECT_INPUT_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    bool result=false;
    if (D.readDataFromStack(p->stackID,inArgs_DISCONNECT_INPUT,inArgs_DISCONNECT_INPUT[0],LUA_DISCONNECT_INPUT_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int inputServerHandle=inData->at(0).int32Data[0];
        int inputIndex=getServerIndexFromServerHandle(inputServerHandle);
        int inputBits=inData->at(1).int32Data[0];
        if (inputIndex!=-1)
        {
            removeOutputInputConnection(inputIndex,inputBits);
            updateAllInputs();
            result=true;
        }
        else
            simSetLastError(LUA_DISCONNECT_INPUT_COMMAND,"Invalid MTB server handle.");
    }
    D.pushOutData(CScriptFunctionDataItem(result));
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

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
    currentDirAndPath=curDirAndFile;

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
        std::cout << "Error, could not find or correctly load the V-REP library. Cannot start 'MTB' plugin.\n";
        return(0); 
    }
    if (getVrepProcAddresses(vrepLib)==0)
    {
        std::cout << "Error, could not find all required functions in the V-REP library. Cannot start 'MTB' plugin.\n";
        unloadVrepLibrary(vrepLib);
        return(0);
    }

    // Check the version of V-REP:
    int vrepVer;
    simGetIntegerParameter(sim_intparam_program_version,&vrepVer);
    if (vrepVer<30200)
    {
        std::cout << "Sorry, your V-REP copy is somewhat old. Cannot start 'MTB' plugin.\n";
        unloadVrepLibrary(vrepLib);
        return(0);
    }


    simRegisterScriptCallbackFunction(strConCat(LUA_START_SERVER_COMMAND,"@","Mtb"),strConCat("number mtbServerHandle,string message=",LUA_START_SERVER_COMMAND,"(string mtbServerExecutable,number portNumber,charBuffer program,table_4 jointPositions, table_2 velocities)"),LUA_START_SERVER_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_STOP_SERVER_COMMAND,"@","Mtb"),strConCat("boolean result=",LUA_STOP_SERVER_COMMAND,"(number mtbServerHandle)"),LUA_STOP_SERVER_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_STEP_COMMAND,"@","Mtb"),strConCat("number result,string message=",LUA_STEP_COMMAND,"(number mtbServerHandle,number timeStep)"),LUA_STEP_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GET_JOINTS_COMMAND,"@","Mtb"),strConCat("table_4 jointValues=",LUA_GET_JOINTS_COMMAND,"(number mtbServerHandle)"),LUA_GET_JOINTS_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GET_OUTPUT_COMMAND,"@","Mtb"),strConCat("table_4 outputValues=",LUA_GET_OUTPUT_COMMAND,"(number mtbServerHandle)"),LUA_GET_OUTPUT_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GET_INPUT_COMMAND,"@","Mtb"),strConCat("table_4 inputValues=",LUA_GET_INPUT_COMMAND,"(number mtbServerHandle)"),LUA_GET_INPUT_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_SET_INPUT_COMMAND,"@","Mtb"),strConCat("boolean result=",LUA_SET_INPUT_COMMAND,"(number mtbServerHandle,table_4 inputValues)"),LUA_SET_INPUT_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_CONNECT_INPUT_COMMAND,"@","Mtb"),strConCat("boolean result=",LUA_CONNECT_INPUT_COMMAND,"(number inputMtbServerHandle,number inputBitNumber,number outputMtbServerHandle,number outputBitNumber,number connectionType)"),LUA_CONNECT_INPUT_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_DISCONNECT_INPUT_COMMAND,"@","Mtb"),strConCat("boolean result=",LUA_DISCONNECT_INPUT_COMMAND,"(number inputMtbServerHandle,number inputBitNumber)"),LUA_DISCONNECT_INPUT_CALLBACK);

    return(PLUGIN_VERSION);
}

VREP_DLLEXPORT void v_repEnd()
{
    unloadVrepLibrary(vrepLib);
}

VREP_DLLEXPORT void* v_repMessage(int message,int* auxiliaryData,void* customData,int* replyData)
{ // This is called quite often. Just watch out for messages/events you want to handle
    // Keep following 5 lines at the beginning and unchanged:
    int errorModeSaved;
    simGetIntegerParameter(sim_intparam_error_report_mode,&errorModeSaved);
    simSetIntegerParameter(sim_intparam_error_report_mode,sim_api_errormessage_ignore);
    void* retVal=NULL;

    if (message==sim_message_eventcallback_simulationended)
    { // simulation ended. End all started MTB servers:
        for (unsigned int i=0;i<allMtbServers.size();i++)
            delete allMtbServers[i].connection;
        allMtbServers.clear();
    }


    // Keep following unchanged:
    simSetIntegerParameter(sim_intparam_error_report_mode,errorModeSaved);
    return(retVal);
}

