// This file is part of the REMOTE API
// 
// Copyright 2006-2017 Coppelia Robotics GmbH. All rights reserved. 
// marc@coppeliarobotics.com
// www.coppeliarobotics.com
// 
// The REMOTE API is licensed under the terms of GNU GPL:
// 
// -------------------------------------------------------------------
// The REMOTE API is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// THE REMOTE API IS DISTRIBUTED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
// WARRANTY. THE USER WILL USE IT AT HIS/HER OWN RISK. THE ORIGINAL
// AUTHORS AND COPPELIA ROBOTICS GMBH WILL NOT BE LIABLE FOR DATA LOSS,
// DAMAGES, LOSS OF PROFITS OR ANY OTHER KIND OF LOSS WHILE USING OR
// MISUSING THIS SOFTWARE.
// 
// See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with the REMOTE API.  If not, see <http://www.gnu.org/licenses/>.
// -------------------------------------------------------------------
//
// This file was automatically created for V-REP release V3.4.0 rev. 1 on April 5th 2017

#include "v_repExtLuaRemoteApiClient.h"
#include "scriptFunctionData.h"
#include "remoteApiLink.h"
#include <boost/lexical_cast.hpp>
#include "v_repLib.h"
#include <iostream>
#include <sstream>

#ifdef _WIN32
    #ifdef QT_COMPIL
        #include <direct.h>
    #else
        #include <shlwapi.h> // required for PathRemoveFileSpec function
        #pragma comment(lib, "Shlwapi.lib")
    #endif
#endif /* _WIN32 */
#if defined (__linux) || defined (__APPLE__)
    #include <unistd.h>
    #define WIN_AFX_MANAGE_STATE
#endif /* __linux || __APPLE__ */

#define CONCAT(x,y,z) x y z
#define strConCat(x,y,z)    CONCAT(x,y,z)

LIBRARY vrepLib;
std::vector<CRemoteApiLink*> allRemoteApiClients;

int getIndexFromPortNb(int portNb)
{
    for (unsigned int i=0;i<allRemoteApiClients.size();i++)
    {
        if (allRemoteApiClients[i]->getPortNb()==portNb)
            return(i);
    }
    return(-1);
}

int getIndexFromClientId(int clientId)
{
    for (unsigned int i=0;i<allRemoteApiClients.size();i++)
    {
        if (allRemoteApiClients[i]->getClientId()==clientId)
            return(i);
    }
    return(-1);
}

bool isScriptThreaded(int scriptID)
{
    int prop,obj;
    if (-1!=simGetScriptProperty(scriptID,&prop,&obj))
        return((prop&sim_scripttype_threaded)!=0);
    return(false);
}

// --------------------------------------------------------------------------------------
// simxStart
// --------------------------------------------------------------------------------------
#define LUA_START_COMMAND "simxStart"

const int inArgs_START[]={
    6,
    sim_script_arg_string,0,
    sim_script_arg_int32,0,
    sim_script_arg_bool,0,
    sim_script_arg_bool,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0, // optional argument
};

void LUA_START_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    int clientId=-1;
    if (D.readDataFromStack(p->stackID,inArgs_START,inArgs_START[0]-1,LUA_START_COMMAND)) // -1 because the last arg is optional
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        std::string ipAddress(inData->at(0).stringData[0]);
        if (ipAddress.length()==0)
            ipAddress="127.0.0.1";
        int port=inData->at(1).int32Data[0];
        bool waitUntilConnected=inData->at(2).boolData[0];
        bool doNotReconnect=inData->at(3).boolData[0];
        int timeOut=inData->at(4).int32Data[0];
        int cycle=5;
        if (inData->size()>=6)
            cycle=inData->at(5).int32Data[0];
        CRemoteApiLink* link=new CRemoteApiLink(ipAddress.c_str(),port,waitUntilConnected,doNotReconnect,timeOut,cycle);
        link->start();
        while (link->getClientId()==-2) // -2 means: we don't know yet
            CRemoteApiLink::waitXms(1);
        clientId=link->getClientId();
        if (clientId<0)
        { // we failed at starting the client
            delete link;
            clientId=-1;
        }
        else
        { // we succeeded at starting the client
            allRemoteApiClients.push_back(link);
            int prop,obj;
            if (-1!=simGetScriptProperty(p->scriptID,&prop,&obj))
            {
                int scriptType=((prop|sim_scripttype_threaded)-sim_scripttype_threaded);
                bool destroyAtSimulationEnd=( (scriptType==sim_scripttype_mainscript)||(scriptType==sim_scripttype_childscript)||(scriptType==sim_scripttype_jointctrlcallback)||(scriptType==sim_scripttype_contactcallback)||(scriptType==sim_scripttype_generalcallback) );
                link->setDestroyAtSimulationEnd(destroyAtSimulationEnd);
            }
        }
    }
    D.pushOutData(CScriptFunctionDataItem(clientId));
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxFinish
// --------------------------------------------------------------------------------------
#define LUA_FINISH_COMMAND "simxFinish"

const int inArgs_FINISH[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_FINISH_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_FINISH,inArgs_FINISH[0],LUA_FINISH_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        if (clientId==-1)
        { // we want to end all clients!
            for (unsigned int i=0;i<allRemoteApiClients.size();i++)
                delete allRemoteApiClients[i]; // this will call simxFinish appropriately
            allRemoteApiClients.clear();
        }
        else
        {
            int index=getIndexFromClientId(clientId);
            if (index!=-1)
            {
                delete allRemoteApiClients[index]; // this will call simxFinish appropriately
                allRemoteApiClients.erase(allRemoteApiClients.begin()+index);
            }
            else
                simSetLastError(LUA_FINISH_COMMAND,"Invalid client ID."); // output an error
        }
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxAddStatusbarMessage
// --------------------------------------------------------------------------------------
#define LUA_ADDSTATUSBARMESSAGE_COMMAND "simxAddStatusbarMessage"

const int inArgs_ADDSTATUSBARMESSAGE[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_string,0,
    sim_script_arg_int32,0,
};

void LUA_ADDSTATUSBARMESSAGE_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_ADDSTATUSBARMESSAGE,inArgs_ADDSTATUSBARMESSAGE[0],LUA_ADDSTATUSBARMESSAGE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(0,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_ADDSTATUSBARMESSAGE_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxAuxiliaryConsoleClose
// --------------------------------------------------------------------------------------
#define LUA_AUXILIARYCONSOLECLOSE_COMMAND "simxAuxiliaryConsoleClose"

const int inArgs_AUXILIARYCONSOLECLOSE[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_AUXILIARYCONSOLECLOSE_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_AUXILIARYCONSOLECLOSE,inArgs_AUXILIARYCONSOLECLOSE[0],LUA_AUXILIARYCONSOLECLOSE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(1,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_AUXILIARYCONSOLECLOSE_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxAuxiliaryConsoleOpen
// --------------------------------------------------------------------------------------
#define LUA_AUXILIARYCONSOLEOPEN_COMMAND "simxAuxiliaryConsoleOpen"

const int inArgs_AUXILIARYCONSOLEOPEN[]={
    9,
    sim_script_arg_int32,0,
    sim_script_arg_string,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32|sim_script_arg_table|SIM_SCRIPT_ARG_NULL_ALLOWED,2,
    sim_script_arg_int32|sim_script_arg_table|SIM_SCRIPT_ARG_NULL_ALLOWED,2,
    sim_script_arg_float|sim_script_arg_table|SIM_SCRIPT_ARG_NULL_ALLOWED,3,
    sim_script_arg_float|sim_script_arg_table|SIM_SCRIPT_ARG_NULL_ALLOWED,3,
    sim_script_arg_int32,0,
};

void LUA_AUXILIARYCONSOLEOPEN_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_AUXILIARYCONSOLEOPEN,inArgs_AUXILIARYCONSOLEOPEN[0],LUA_AUXILIARYCONSOLEOPEN_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(8).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(2,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_AUXILIARYCONSOLEOPEN_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxAuxiliaryConsolePrint
// --------------------------------------------------------------------------------------
#define LUA_AUXILIARYCONSOLEPRINT_COMMAND "simxAuxiliaryConsolePrint"

const int inArgs_AUXILIARYCONSOLEPRINT[]={
    4,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_string|SIM_SCRIPT_ARG_NULL_ALLOWED,0,
    sim_script_arg_int32,0,
};

void LUA_AUXILIARYCONSOLEPRINT_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_AUXILIARYCONSOLEPRINT,inArgs_AUXILIARYCONSOLEPRINT[0],LUA_AUXILIARYCONSOLEPRINT_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(3).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(3,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_AUXILIARYCONSOLEPRINT_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxAuxiliaryConsoleShow
// --------------------------------------------------------------------------------------
#define LUA_AUXILIARYCONSOLESHOW_COMMAND "simxAuxiliaryConsoleShow"

const int inArgs_AUXILIARYCONSOLESHOW[]={
    4,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_bool,0,
    sim_script_arg_int32,0,
};

void LUA_AUXILIARYCONSOLESHOW_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_AUXILIARYCONSOLESHOW,inArgs_AUXILIARYCONSOLESHOW[0],LUA_AUXILIARYCONSOLESHOW_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(3).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(4,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_AUXILIARYCONSOLESHOW_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxBreakForceSensor
// --------------------------------------------------------------------------------------
#define LUA_BREAKFORCESENSOR_COMMAND "simxBreakForceSensor"

const int inArgs_BREAKFORCESENSOR[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_BREAKFORCESENSOR_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_BREAKFORCESENSOR,inArgs_BREAKFORCESENSOR[0],LUA_BREAKFORCESENSOR_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(5,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_BREAKFORCESENSOR_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxClearFloatSignal
// --------------------------------------------------------------------------------------
#define LUA_CLEARFLOATSIGNAL_COMMAND "simxClearFloatSignal"

const int inArgs_CLEARFLOATSIGNAL[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_string,0,
    sim_script_arg_int32,0,
};

void LUA_CLEARFLOATSIGNAL_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_CLEARFLOATSIGNAL,inArgs_CLEARFLOATSIGNAL[0],LUA_CLEARFLOATSIGNAL_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(6,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_CLEARFLOATSIGNAL_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxClearIntegerSignal
// --------------------------------------------------------------------------------------
#define LUA_CLEARINTEGERSIGNAL_COMMAND "simxClearIntegerSignal"

const int inArgs_CLEARINTEGERSIGNAL[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_string,0,
    sim_script_arg_int32,0,
};

void LUA_CLEARINTEGERSIGNAL_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_CLEARINTEGERSIGNAL,inArgs_CLEARINTEGERSIGNAL[0],LUA_CLEARINTEGERSIGNAL_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(7,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_CLEARINTEGERSIGNAL_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxClearStringSignal
// --------------------------------------------------------------------------------------
#define LUA_CLEARSTRINGSIGNAL_COMMAND "simxClearStringSignal"

const int inArgs_CLEARSTRINGSIGNAL[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_string,0,
    sim_script_arg_int32,0,
};

void LUA_CLEARSTRINGSIGNAL_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_CLEARSTRINGSIGNAL,inArgs_CLEARSTRINGSIGNAL[0],LUA_CLEARSTRINGSIGNAL_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(8,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_CLEARSTRINGSIGNAL_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetObjectHandle
// --------------------------------------------------------------------------------------
#define LUA_GETOBJECTHANDLE_COMMAND "simxGetObjectHandle"

const int inArgs_GETOBJECTHANDLE[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_string,0,
    sim_script_arg_int32,0,
};

void LUA_GETOBJECTHANDLE_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOBJECTHANDLE,inArgs_GETOBJECTHANDLE[0],LUA_GETOBJECTHANDLE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(9,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETOBJECTHANDLE_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxCloseScene
// --------------------------------------------------------------------------------------
#define LUA_CLOSESCENE_COMMAND "simxCloseScene"

const int inArgs_CLOSESCENE[]={
    2,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_CLOSESCENE_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_CLOSESCENE,inArgs_CLOSESCENE[0],LUA_CLOSESCENE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(1).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(10,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_CLOSESCENE_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxCopyPasteObjects
// --------------------------------------------------------------------------------------
#define LUA_COPYPASTEOBJECTS_COMMAND "simxCopyPasteObjects"

const int inArgs_COPYPASTEOBJECTS[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32|sim_script_arg_table,1, // at least one item in the table
    sim_script_arg_int32,0,
};

void LUA_COPYPASTEOBJECTS_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_COPYPASTEOBJECTS,inArgs_COPYPASTEOBJECTS[0],LUA_COPYPASTEOBJECTS_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(11,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_COPYPASTEOBJECTS_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxLoadScene
// --------------------------------------------------------------------------------------
#define LUA_LOADSCENE_COMMAND "simxLoadScene"

const int inArgs_LOADSCENE[]={
    4,
    sim_script_arg_int32,0,
    sim_script_arg_string,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_LOADSCENE_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_LOADSCENE,inArgs_LOADSCENE[0],LUA_LOADSCENE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(3).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(12,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_LOADSCENE_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxLoadModel
// --------------------------------------------------------------------------------------
#define LUA_LOADMODEL_COMMAND "simxLoadModel"

const int inArgs_LOADMODEL[]={
    4,
    sim_script_arg_int32,0,
    sim_script_arg_string,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_LOADMODEL_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_LOADMODEL,inArgs_LOADMODEL[0],LUA_LOADMODEL_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(3).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(13,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_LOADMODEL_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxLoadUI
// --------------------------------------------------------------------------------------
#define LUA_LOADUI_COMMAND "simxLoadUI"

const int inArgs_LOADUI[]={
    4,
    sim_script_arg_int32,0,
    sim_script_arg_string,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_LOADUI_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_LOADUI,inArgs_LOADUI[0],LUA_LOADUI_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(3).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(14,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_LOADUI_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxSetBooleanParameter
// --------------------------------------------------------------------------------------
#define LUA_SETBOOLEANPARAMETER_COMMAND "simxSetBooleanParameter"

const int inArgs_SETBOOLEANPARAMETER[]={
    4,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_bool,0,
    sim_script_arg_int32,0,
};

void LUA_SETBOOLEANPARAMETER_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_SETBOOLEANPARAMETER,inArgs_SETBOOLEANPARAMETER[0],LUA_SETBOOLEANPARAMETER_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(3).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(15,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_SETBOOLEANPARAMETER_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxStartSimulation
// --------------------------------------------------------------------------------------
#define LUA_STARTSIMULATION_COMMAND "simxStartSimulation"

const int inArgs_STARTSIMULATION[]={
    2,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_STARTSIMULATION_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_STARTSIMULATION,inArgs_STARTSIMULATION[0],LUA_STARTSIMULATION_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(1).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(16,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_STARTSIMULATION_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxStopSimulation
// --------------------------------------------------------------------------------------
#define LUA_STOPSIMULATION_COMMAND "simxStopSimulation"

const int inArgs_STOPSIMULATION[]={
    2,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_STOPSIMULATION_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_STOPSIMULATION,inArgs_STOPSIMULATION[0],LUA_STOPSIMULATION_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(1).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(17,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_STOPSIMULATION_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxSetFloatingParameter
// --------------------------------------------------------------------------------------
#define LUA_SETFLOATINGPARAMETER_COMMAND "simxSetFloatingParameter"

const int inArgs_SETFLOATINGPARAMETER[]={
    4,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_float,0,
    sim_script_arg_int32,0,
};

void LUA_SETFLOATINGPARAMETER_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_SETFLOATINGPARAMETER,inArgs_SETFLOATINGPARAMETER[0],LUA_SETFLOATINGPARAMETER_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(3).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(18,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_SETFLOATINGPARAMETER_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxSetIntegerParameter
// --------------------------------------------------------------------------------------
#define LUA_SETINTEGERPARAMETER_COMMAND "simxSetIntegerParameter"

const int inArgs_SETINTEGERPARAMETER[]={
    4,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_SETINTEGERPARAMETER_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_SETINTEGERPARAMETER,inArgs_SETINTEGERPARAMETER[0],LUA_SETINTEGERPARAMETER_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(3).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(19,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_SETINTEGERPARAMETER_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxSetArrayParameter
// --------------------------------------------------------------------------------------
#define LUA_SETARRAYPARAMETER_COMMAND "simxSetArrayParameter"

const int inArgs_SETARRAYPARAMETER[]={
    4,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_script_arg_table,3,
    sim_script_arg_int32,0,
};

void LUA_SETARRAYPARAMETER_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_SETARRAYPARAMETER,inArgs_SETARRAYPARAMETER[0],LUA_SETARRAYPARAMETER_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(3).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(20,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_SETARRAYPARAMETER_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetBooleanParameter
// --------------------------------------------------------------------------------------
#define LUA_GETBOOLEANPARAMETER_COMMAND "simxGetBooleanParameter"

const int inArgs_GETBOOLEANPARAMETER[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETBOOLEANPARAMETER_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETBOOLEANPARAMETER,inArgs_GETBOOLEANPARAMETER[0],LUA_GETBOOLEANPARAMETER_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(21,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETBOOLEANPARAMETER_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetFloatingParameter
// --------------------------------------------------------------------------------------
#define LUA_GETFLOATINGPARAMETER_COMMAND "simxGetFloatingParameter"

const int inArgs_GETFLOATINGPARAMETER[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETFLOATINGPARAMETER_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETFLOATINGPARAMETER,inArgs_GETFLOATINGPARAMETER[0],LUA_GETFLOATINGPARAMETER_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(22,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETFLOATINGPARAMETER_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetIntegerParameter
// --------------------------------------------------------------------------------------
#define LUA_GETINTEGERPARAMETER_COMMAND "simxGetIntegerParameter"

const int inArgs_GETINTEGERPARAMETER[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETINTEGERPARAMETER_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETINTEGERPARAMETER,inArgs_GETINTEGERPARAMETER[0],LUA_GETINTEGERPARAMETER_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(23,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETINTEGERPARAMETER_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetArrayParameter
// --------------------------------------------------------------------------------------
#define LUA_GETARRAYPARAMETER_COMMAND "simxGetArrayParameter"

const int inArgs_GETARRAYPARAMETER[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETARRAYPARAMETER_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETARRAYPARAMETER,inArgs_GETARRAYPARAMETER[0],LUA_GETARRAYPARAMETER_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(24,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETARRAYPARAMETER_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetStringParameter
// --------------------------------------------------------------------------------------
#define LUA_GETSTRINGPARAMETER_COMMAND "simxGetStringParameter"

const int inArgs_GETSTRINGPARAMETER[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETSTRINGPARAMETER_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETSTRINGPARAMETER,inArgs_GETSTRINGPARAMETER[0],LUA_GETSTRINGPARAMETER_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(25,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETSTRINGPARAMETER_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxSetFloatSignal
// --------------------------------------------------------------------------------------
#define LUA_SETFLOATSIGNAL_COMMAND "simxSetFloatSignal"

const int inArgs_SETFLOATSIGNAL[]={
    4,
    sim_script_arg_int32,0,
    sim_script_arg_string,0,
    sim_script_arg_float,0,
    sim_script_arg_int32,0,
};

void LUA_SETFLOATSIGNAL_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_SETFLOATSIGNAL,inArgs_SETFLOATSIGNAL[0],LUA_SETFLOATSIGNAL_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(3).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(26,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_SETFLOATSIGNAL_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxSetIntegerSignal
// --------------------------------------------------------------------------------------
#define LUA_SETINTEGERSIGNAL_COMMAND "simxSetIntegerSignal"

const int inArgs_SETINTEGERSIGNAL[]={
    4,
    sim_script_arg_int32,0,
    sim_script_arg_string,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_SETINTEGERSIGNAL_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_SETINTEGERSIGNAL,inArgs_SETINTEGERSIGNAL[0],LUA_SETINTEGERSIGNAL_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(3).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(27,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_SETINTEGERSIGNAL_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxSetStringSignal
// --------------------------------------------------------------------------------------
#define LUA_SETSTRINGSIGNAL_COMMAND "simxSetStringSignal"

const int inArgs_SETSTRINGSIGNAL[]={
    4,
    sim_script_arg_int32,0,
    sim_script_arg_string,0,
    sim_script_arg_charbuff,0,
    sim_script_arg_int32,0,
};

void LUA_SETSTRINGSIGNAL_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_SETSTRINGSIGNAL,inArgs_SETSTRINGSIGNAL[0],LUA_SETSTRINGSIGNAL_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(3).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(28,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_SETSTRINGSIGNAL_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetFloatSignal
// --------------------------------------------------------------------------------------
#define LUA_GETFLOATSIGNAL_COMMAND "simxGetFloatSignal"

const int inArgs_GETFLOATSIGNAL[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_string,0,
    sim_script_arg_int32,0,
};

void LUA_GETFLOATSIGNAL_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETFLOATSIGNAL,inArgs_GETFLOATSIGNAL[0],LUA_GETFLOATSIGNAL_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(29,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETFLOATSIGNAL_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetIntegerSignal
// --------------------------------------------------------------------------------------
#define LUA_GETINTEGERSIGNAL_COMMAND "simxGetIntegerSignal"

const int inArgs_GETINTEGERSIGNAL[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_string,0,
    sim_script_arg_int32,0,
};

void LUA_GETINTEGERSIGNAL_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETINTEGERSIGNAL,inArgs_GETINTEGERSIGNAL[0],LUA_GETINTEGERSIGNAL_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(30,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETINTEGERSIGNAL_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetStringSignal
// --------------------------------------------------------------------------------------
#define LUA_GETSTRINGSIGNAL_COMMAND "simxGetStringSignal"

const int inArgs_GETSTRINGSIGNAL[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_string,0,
    sim_script_arg_int32,0,
};

void LUA_GETSTRINGSIGNAL_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETSTRINGSIGNAL,inArgs_GETSTRINGSIGNAL[0],LUA_GETSTRINGSIGNAL_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(31,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETSTRINGSIGNAL_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxCreateDummy
// --------------------------------------------------------------------------------------
#define LUA_CREATEDUMMY_COMMAND "simxCreateDummy"

const int inArgs_CREATEDUMMY[]={
    4,
    sim_script_arg_int32,0,
    sim_script_arg_float,0,
    sim_script_arg_int32|sim_script_arg_table|SIM_SCRIPT_ARG_NULL_ALLOWED,12,
    sim_script_arg_int32,0,
};

void LUA_CREATEDUMMY_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_CREATEDUMMY,inArgs_CREATEDUMMY[0],LUA_CREATEDUMMY_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(3).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(32,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_CREATEDUMMY_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxDisplayDialog
// --------------------------------------------------------------------------------------
#define LUA_DISPLAYDIALOG_COMMAND "simxDisplayDialog"

const int inArgs_DISPLAYDIALOG[]={
    8,
    sim_script_arg_int32,0,
    sim_script_arg_string,0,
    sim_script_arg_string,0,
    sim_script_arg_int32,0,
    sim_script_arg_string,0,
    sim_script_arg_float|sim_script_arg_table|SIM_SCRIPT_ARG_NULL_ALLOWED,6,
    sim_script_arg_float|sim_script_arg_table|SIM_SCRIPT_ARG_NULL_ALLOWED,6,
    sim_script_arg_int32,0,
};

void LUA_DISPLAYDIALOG_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_DISPLAYDIALOG,inArgs_DISPLAYDIALOG[0],LUA_DISPLAYDIALOG_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(7).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(33,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_DISPLAYDIALOG_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxEndDialog
// --------------------------------------------------------------------------------------
#define LUA_ENDDIALOG_COMMAND "simxEndDialog"

const int inArgs_ENDDIALOG[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_ENDDIALOG_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_ENDDIALOG,inArgs_ENDDIALOG[0],LUA_ENDDIALOG_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(34,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_ENDDIALOG_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetDialogInput
// --------------------------------------------------------------------------------------
#define LUA_GETDIALOGINPUT_COMMAND "simxGetDialogInput"

const int inArgs_GETDIALOGINPUT[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETDIALOGINPUT_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETDIALOGINPUT,inArgs_GETDIALOGINPUT[0],LUA_GETDIALOGINPUT_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(35,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETDIALOGINPUT_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetDialogResult
// --------------------------------------------------------------------------------------
#define LUA_GETDIALOGRESULT_COMMAND "simxGetDialogResult"

const int inArgs_GETDIALOGRESULT[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETDIALOGRESULT_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETDIALOGRESULT,inArgs_GETDIALOGRESULT[0],LUA_GETDIALOGRESULT_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(36,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETDIALOGRESULT_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxEraseFile
// --------------------------------------------------------------------------------------
#define LUA_ERASEFILE_COMMAND "simxEraseFile"

const int inArgs_ERASEFILE[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_string,0,
    sim_script_arg_int32,0,
};

void LUA_ERASEFILE_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_ERASEFILE,inArgs_ERASEFILE[0],LUA_ERASEFILE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(37,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_ERASEFILE_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetCollisionHandle
// --------------------------------------------------------------------------------------
#define LUA_GETCOLLISIONHANDLE_COMMAND "simxGetCollisionHandle"

const int inArgs_GETCOLLISIONHANDLE[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_string,0,
    sim_script_arg_int32,0,
};

void LUA_GETCOLLISIONHANDLE_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETCOLLISIONHANDLE,inArgs_GETCOLLISIONHANDLE[0],LUA_GETCOLLISIONHANDLE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(38,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETCOLLISIONHANDLE_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetCollectionHandle
// --------------------------------------------------------------------------------------
#define LUA_GETCOLLECTIONHANDLE_COMMAND "simxGetCollectionHandle"

const int inArgs_GETCOLLECTIONHANDLE[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_string,0,
    sim_script_arg_int32,0,
};

void LUA_GETCOLLECTIONHANDLE_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETCOLLECTIONHANDLE,inArgs_GETCOLLECTIONHANDLE[0],LUA_GETCOLLECTIONHANDLE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(96,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETCOLLECTIONHANDLE_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetConnectionId
// --------------------------------------------------------------------------------------
#define LUA_GETCONNECTIONID_COMMAND "simxGetConnectionId"

const int inArgs_GETCONNECTIONID[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_GETCONNECTIONID_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETCONNECTIONID,inArgs_GETCONNECTIONID[0],LUA_GETCONNECTIONID_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(39,&D,false,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETCONNECTIONID_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetDistanceHandle
// --------------------------------------------------------------------------------------
#define LUA_GETDISTANCEHANDLE_COMMAND "simxGetDistanceHandle"

const int inArgs_GETDISTANCEHANDLE[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_string,0,
    sim_script_arg_int32,0,
};

void LUA_GETDISTANCEHANDLE_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETDISTANCEHANDLE,inArgs_GETDISTANCEHANDLE[0],LUA_GETDISTANCEHANDLE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(40,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETDISTANCEHANDLE_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetInMessageInfo
// --------------------------------------------------------------------------------------
#define LUA_GETINMESSAGEINFO_COMMAND "simxGetInMessageInfo"

const int inArgs_GETINMESSAGEINFO[]={
    2,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETINMESSAGEINFO_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETINMESSAGEINFO,inArgs_GETINMESSAGEINFO[0],LUA_GETINMESSAGEINFO_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(41,&D,false,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETINMESSAGEINFO_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetOutMessageInfo
// --------------------------------------------------------------------------------------
#define LUA_GETOUTMESSAGEINFO_COMMAND "simxGetOutMessageInfo"

const int inArgs_GETOUTMESSAGEINFO[]={
    2,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETOUTMESSAGEINFO_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOUTMESSAGEINFO,inArgs_GETOUTMESSAGEINFO[0],LUA_GETOUTMESSAGEINFO_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(42,&D,false,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETOUTMESSAGEINFO_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetJointForce
// --------------------------------------------------------------------------------------
#define LUA_GETJOINTFORCE_COMMAND "simxGetJointForce"

const int inArgs_GETJOINTFORCE[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETJOINTFORCE_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETJOINTFORCE,inArgs_GETJOINTFORCE[0],LUA_GETJOINTFORCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(43,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETJOINTFORCE_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetJointMatrix
// --------------------------------------------------------------------------------------
#define LUA_GETJOINTMATRIX_COMMAND "simxGetJointMatrix"

const int inArgs_GETJOINTMATRIX[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETJOINTMATRIX_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETJOINTMATRIX,inArgs_GETJOINTMATRIX[0],LUA_GETJOINTMATRIX_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(44,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETJOINTMATRIX_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetJointPosition
// --------------------------------------------------------------------------------------
#define LUA_GETJOINTPOSITION_COMMAND "simxGetJointPosition"

const int inArgs_GETJOINTPOSITION[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETJOINTPOSITION_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETJOINTPOSITION,inArgs_GETJOINTPOSITION[0],LUA_GETJOINTPOSITION_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(45,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETJOINTPOSITION_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetLastCmdTime
// --------------------------------------------------------------------------------------
#define LUA_GETLASTCMDTIME_COMMAND "simxGetLastCmdTime"

const int inArgs_GETLASTCMDTIME[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_GETLASTCMDTIME_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETLASTCMDTIME,inArgs_GETLASTCMDTIME[0],LUA_GETLASTCMDTIME_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(46,&D,false,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETLASTCMDTIME_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetLastErrors
// --------------------------------------------------------------------------------------
#define LUA_GETLASTERRORS_COMMAND "simxGetLastErrors"

const int inArgs_GETLASTERRORS[]={
    2,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETLASTERRORS_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETLASTERRORS,inArgs_GETLASTERRORS[0],LUA_GETLASTERRORS_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(1).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(47,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETLASTERRORS_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetModelProperty
// --------------------------------------------------------------------------------------
#define LUA_GETMODELPROPERTY_COMMAND "simxGetModelProperty"

const int inArgs_GETMODELPROPERTY[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETMODELPROPERTY_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETMODELPROPERTY,inArgs_GETMODELPROPERTY[0],LUA_GETMODELPROPERTY_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(48,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETMODELPROPERTY_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetObjectChild
// --------------------------------------------------------------------------------------
#define LUA_GETOBJECTCHILD_COMMAND "simxGetObjectChild"

const int inArgs_GETOBJECTCHILD[]={
    4,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETOBJECTCHILD_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOBJECTCHILD,inArgs_GETOBJECTCHILD[0],LUA_GETOBJECTCHILD_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(3).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(49,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETOBJECTCHILD_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetObjectFloatParameter
// --------------------------------------------------------------------------------------
#define LUA_GETOBJECTFLOATPARAMETER_COMMAND "simxGetObjectFloatParameter"

const int inArgs_GETOBJECTFLOATPARAMETER[]={
    4,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETOBJECTFLOATPARAMETER_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOBJECTFLOATPARAMETER,inArgs_GETOBJECTFLOATPARAMETER[0],LUA_GETOBJECTFLOATPARAMETER_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(3).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(50,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETOBJECTFLOATPARAMETER_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetObjectIntParameter
// --------------------------------------------------------------------------------------
#define LUA_GETOBJECTINTPARAMETER_COMMAND "simxGetObjectIntParameter"

const int inArgs_GETOBJECTINTPARAMETER[]={
    4,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETOBJECTINTPARAMETER_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOBJECTINTPARAMETER,inArgs_GETOBJECTINTPARAMETER[0],LUA_GETOBJECTINTPARAMETER_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(3).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(51,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETOBJECTINTPARAMETER_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetObjectGroupData
// --------------------------------------------------------------------------------------
#define LUA_GETOBJECTGROUPDATA_COMMAND "simxGetObjectGroupData"

const int inArgs_GETOBJECTGROUPDATA[]={
    4,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETOBJECTGROUPDATA_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOBJECTGROUPDATA,inArgs_GETOBJECTGROUPDATA[0],LUA_GETOBJECTGROUPDATA_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(3).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(52,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETOBJECTGROUPDATA_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetObjectOrientation
// --------------------------------------------------------------------------------------
#define LUA_GETOBJECTORIENTATION_COMMAND "simxGetObjectOrientation"

const int inArgs_GETOBJECTORIENTATION[]={
    4,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETOBJECTORIENTATION_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOBJECTORIENTATION,inArgs_GETOBJECTORIENTATION[0],LUA_GETOBJECTORIENTATION_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(3).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(53,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETOBJECTORIENTATION_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetObjectPosition
// --------------------------------------------------------------------------------------
#define LUA_GETOBJECTPOSITION_COMMAND "simxGetObjectPosition"

const int inArgs_GETOBJECTPOSITION[]={
    4,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETOBJECTPOSITION_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOBJECTPOSITION,inArgs_GETOBJECTPOSITION[0],LUA_GETOBJECTPOSITION_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(3).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(54,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETOBJECTPOSITION_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetObjectParent
// --------------------------------------------------------------------------------------
#define LUA_GETOBJECTPARENT_COMMAND "simxGetObjectParent"

const int inArgs_GETOBJECTPARENT[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETOBJECTPARENT_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOBJECTPARENT,inArgs_GETOBJECTPARENT[0],LUA_GETOBJECTPARENT_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(55,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETOBJECTPARENT_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetObjects
// --------------------------------------------------------------------------------------
#define LUA_GETOBJECTS_COMMAND "simxGetObjects"

const int inArgs_GETOBJECTS[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETOBJECTS_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOBJECTS,inArgs_GETOBJECTS[0],LUA_GETOBJECTS_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(56,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETOBJECTS_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetObjectSelection
// --------------------------------------------------------------------------------------
#define LUA_GETOBJECTSELECTION_COMMAND "simxGetObjectSelection"

const int inArgs_GETOBJECTSELECTION[]={
    2,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETOBJECTSELECTION_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOBJECTSELECTION,inArgs_GETOBJECTSELECTION[0],LUA_GETOBJECTSELECTION_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(1).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(57,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETOBJECTSELECTION_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetObjectVelocity
// --------------------------------------------------------------------------------------
#define LUA_GETOBJECTVELOCITY_COMMAND "simxGetObjectVelocity"

const int inArgs_GETOBJECTVELOCITY[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETOBJECTVELOCITY_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOBJECTVELOCITY,inArgs_GETOBJECTVELOCITY[0],LUA_GETOBJECTVELOCITY_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(58,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETOBJECTVELOCITY_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetPingTime
// --------------------------------------------------------------------------------------
#define LUA_GETPINGTIME_COMMAND "simxGetPingTime"

const int inArgs_GETPINGTIME[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_GETPINGTIME_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETPINGTIME,inArgs_GETPINGTIME[0],LUA_GETPINGTIME_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(59,&D,true,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETPINGTIME_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetUIButtonProperty
// --------------------------------------------------------------------------------------
#define LUA_GETUIBUTTONPROPERTY_COMMAND "simxGetUIButtonProperty"

const int inArgs_GETUIBUTTONPROPERTY[]={
    4,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETUIBUTTONPROPERTY_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETUIBUTTONPROPERTY,inArgs_GETUIBUTTONPROPERTY[0],LUA_GETUIBUTTONPROPERTY_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(3).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(60,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETUIBUTTONPROPERTY_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetUIEventButton
// --------------------------------------------------------------------------------------
#define LUA_GETUIEVENTBUTTON_COMMAND "simxGetUIEventButton"

const int inArgs_GETUIEVENTBUTTON[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETUIEVENTBUTTON_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETUIEVENTBUTTON,inArgs_GETUIEVENTBUTTON[0],LUA_GETUIEVENTBUTTON_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(61,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETUIEVENTBUTTON_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetUIHandle
// --------------------------------------------------------------------------------------
#define LUA_GETUIHANDLE_COMMAND "simxGetUIHandle"

const int inArgs_GETUIHANDLE[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_string,0,
    sim_script_arg_int32,0,
};

void LUA_GETUIHANDLE_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETUIHANDLE,inArgs_GETUIHANDLE[0],LUA_GETUIHANDLE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(62,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETUIHANDLE_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetUISlider
// --------------------------------------------------------------------------------------
#define LUA_GETUISLIDER_COMMAND "simxGetUISlider"

const int inArgs_GETUISLIDER[]={
    4,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETUISLIDER_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETUISLIDER,inArgs_GETUISLIDER[0],LUA_GETUISLIDER_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(3).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(63,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETUISLIDER_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetVisionSensorDepthBuffer
// --------------------------------------------------------------------------------------
#define LUA_GETVISIONSENSORDEPTHBUFFER_COMMAND "simxGetVisionSensorDepthBuffer"

const int inArgs_GETVISIONSENSORDEPTHBUFFER[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETVISIONSENSORDEPTHBUFFER_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETVISIONSENSORDEPTHBUFFER,inArgs_GETVISIONSENSORDEPTHBUFFER[0],LUA_GETVISIONSENSORDEPTHBUFFER_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(64,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETVISIONSENSORDEPTHBUFFER_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxGetVisionSensorImage
// --------------------------------------------------------------------------------------
#define LUA_GETVISIONSENSORIMAGE_COMMAND "simxGetVisionSensorImage"

const int inArgs_GETVISIONSENSORIMAGE[]={
    4,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_GETVISIONSENSORIMAGE_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETVISIONSENSORIMAGE,inArgs_GETVISIONSENSORIMAGE[0],LUA_GETVISIONSENSORIMAGE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(3).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(65,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_GETVISIONSENSORIMAGE_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxSetVisionSensorImage
// --------------------------------------------------------------------------------------
#define LUA_SETVISIONSENSORIMAGE_COMMAND "simxSetVisionSensorImage"

const int inArgs_SETVISIONSENSORIMAGE[]={
    5,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_charbuff,1,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_SETVISIONSENSORIMAGE_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_SETVISIONSENSORIMAGE,inArgs_SETVISIONSENSORIMAGE[0],LUA_SETVISIONSENSORIMAGE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(4).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(66,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_SETVISIONSENSORIMAGE_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxPauseCommunication
// --------------------------------------------------------------------------------------
#define LUA_PAUSECOMMUNICATION_COMMAND "simxPauseCommunication"

const int inArgs_PAUSECOMMUNICATION[]={
    2,
    sim_script_arg_int32,0,
    sim_script_arg_bool,0,
};

void LUA_PAUSECOMMUNICATION_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_PAUSECOMMUNICATION,inArgs_PAUSECOMMUNICATION[0],LUA_PAUSECOMMUNICATION_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(67,&D,false,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_PAUSECOMMUNICATION_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxPauseSimulation
// --------------------------------------------------------------------------------------
#define LUA_PAUSESIMULATION_COMMAND "simxPauseSimulation"

const int inArgs_PAUSESIMULATION[]={
    2,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_PAUSESIMULATION_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_PAUSESIMULATION,inArgs_PAUSESIMULATION[0],LUA_PAUSESIMULATION_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(1).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(68,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_PAUSESIMULATION_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxQuery
// --------------------------------------------------------------------------------------
#define LUA_QUERY_COMMAND "simxQuery"

const int inArgs_QUERY[]={
    5,
    sim_script_arg_int32,0,
    sim_script_arg_string,0,
    sim_script_arg_charbuff,0,
    sim_script_arg_string,0,
    sim_script_arg_int32,0,
};

void LUA_QUERY_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_QUERY,inArgs_QUERY[0],LUA_QUERY_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(69,&D,true,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_QUERY_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxReadCollision
// --------------------------------------------------------------------------------------
#define LUA_READCOLLISION_COMMAND "simxReadCollision"

const int inArgs_READCOLLISION[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_READCOLLISION_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_READCOLLISION,inArgs_READCOLLISION[0],LUA_READCOLLISION_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(70,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_READCOLLISION_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxReadDistance
// --------------------------------------------------------------------------------------
#define LUA_READDISTANCE_COMMAND "simxReadDistance"

const int inArgs_READDISTANCE[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_READDISTANCE_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_READDISTANCE,inArgs_READDISTANCE[0],LUA_READDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(71,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_READDISTANCE_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxReadForceSensor
// --------------------------------------------------------------------------------------
#define LUA_READFORCESENSOR_COMMAND "simxReadForceSensor"

const int inArgs_READFORCESENSOR[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_READFORCESENSOR_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_READFORCESENSOR,inArgs_READFORCESENSOR[0],LUA_READFORCESENSOR_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(72,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_READFORCESENSOR_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxReadProximitySensor
// --------------------------------------------------------------------------------------
#define LUA_READPROXIMITYSENSOR_COMMAND "simxReadProximitySensor"

const int inArgs_READPROXIMITYSENSOR[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_READPROXIMITYSENSOR_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_READPROXIMITYSENSOR,inArgs_READPROXIMITYSENSOR[0],LUA_READPROXIMITYSENSOR_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(73,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_READPROXIMITYSENSOR_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxReadStringStream
// --------------------------------------------------------------------------------------
#define LUA_READSTRINGSTREAM_COMMAND "simxReadStringStream"

const int inArgs_READSTRINGSTREAM[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_string,0,
    sim_script_arg_int32,0,
};

void LUA_READSTRINGSTREAM_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_READSTRINGSTREAM,inArgs_READSTRINGSTREAM[0],LUA_READSTRINGSTREAM_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(74,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_READSTRINGSTREAM_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxWriteStringStream
// --------------------------------------------------------------------------------------
#define LUA_WRITESTRINGSTREAM_COMMAND "simxWriteStringStream"

const int inArgs_WRITESTRINGSTREAM[]={
    4,
    sim_script_arg_int32,0,
    sim_script_arg_string,0,
    sim_script_arg_charbuff,0,
    sim_script_arg_int32,0,
};

void LUA_WRITESTRINGSTREAM_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_WRITESTRINGSTREAM,inArgs_WRITESTRINGSTREAM[0],LUA_WRITESTRINGSTREAM_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(3).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(75,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_WRITESTRINGSTREAM_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxReadVisionSensor
// --------------------------------------------------------------------------------------
#define LUA_READVISIONSENSOR_COMMAND "simxReadVisionSensor"

const int inArgs_READVISIONSENSOR[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_READVISIONSENSOR_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_READVISIONSENSOR,inArgs_READVISIONSENSOR[0],LUA_READVISIONSENSOR_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(76,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_READVISIONSENSOR_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxRemoveModel
// --------------------------------------------------------------------------------------
#define LUA_REMOVEMODEL_COMMAND "simxRemoveModel"

const int inArgs_REMOVEMODEL[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_REMOVEMODEL_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_REMOVEMODEL,inArgs_REMOVEMODEL[0],LUA_REMOVEMODEL_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(77,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_REMOVEMODEL_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxRemoveObject
// --------------------------------------------------------------------------------------
#define LUA_REMOVEOBJECT_COMMAND "simxRemoveObject"

const int inArgs_REMOVEOBJECT[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_REMOVEOBJECT_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_REMOVEOBJECT,inArgs_REMOVEOBJECT[0],LUA_REMOVEOBJECT_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(78,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_REMOVEOBJECT_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxRemoveUI
// --------------------------------------------------------------------------------------
#define LUA_REMOVEUI_COMMAND "simxRemoveUI"

const int inArgs_REMOVEUI[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_REMOVEUI_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_REMOVEUI,inArgs_REMOVEUI[0],LUA_REMOVEUI_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(79,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_REMOVEUI_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxSetJointForce
// --------------------------------------------------------------------------------------
#define LUA_SETJOINTFORCE_COMMAND "simxSetJointForce"

const int inArgs_SETJOINTFORCE[]={
    4,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_float,0,
    sim_script_arg_int32,0,
};

void LUA_SETJOINTFORCE_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_SETJOINTFORCE,inArgs_SETJOINTFORCE[0],LUA_SETJOINTFORCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(3).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(80,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_SETJOINTFORCE_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxSetJointPosition
// --------------------------------------------------------------------------------------
#define LUA_SETJOINTPOSITION_COMMAND "simxSetJointPosition"

const int inArgs_SETJOINTPOSITION[]={
    4,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_float,0,
    sim_script_arg_int32,0,
};

void LUA_SETJOINTPOSITION_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_SETJOINTPOSITION,inArgs_SETJOINTPOSITION[0],LUA_SETJOINTPOSITION_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(3).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(81,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_SETJOINTPOSITION_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxSetJointTargetPosition
// --------------------------------------------------------------------------------------
#define LUA_SETJOINTTARGETPOSITION_COMMAND "simxSetJointTargetPosition"

const int inArgs_SETJOINTTARGETPOSITION[]={
    4,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_float,0,
    sim_script_arg_int32,0,
};

void LUA_SETJOINTTARGETPOSITION_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_SETJOINTTARGETPOSITION,inArgs_SETJOINTTARGETPOSITION[0],LUA_SETJOINTTARGETPOSITION_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(3).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(82,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_SETJOINTTARGETPOSITION_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxSetJointTargetVelocity
// --------------------------------------------------------------------------------------
#define LUA_SETJOINTTARGETVELOCITY_COMMAND "simxSetJointTargetVelocity"

const int inArgs_SETJOINTTARGETVELOCITY[]={
    4,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_float,0,
    sim_script_arg_int32,0,
};

void LUA_SETJOINTTARGETVELOCITY_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_SETJOINTTARGETVELOCITY,inArgs_SETJOINTTARGETVELOCITY[0],LUA_SETJOINTTARGETVELOCITY_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(3).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(83,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_SETJOINTTARGETVELOCITY_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxSetModelProperty
// --------------------------------------------------------------------------------------
#define LUA_SETMODELPROPERTY_COMMAND "simxSetModelProperty"

const int inArgs_SETMODELPROPERTY[]={
    4,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_SETMODELPROPERTY_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_SETMODELPROPERTY,inArgs_SETMODELPROPERTY[0],LUA_SETMODELPROPERTY_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(3).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(84,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_SETMODELPROPERTY_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxSetObjectOrientation
// --------------------------------------------------------------------------------------
#define LUA_SETOBJECTORIENTATION_COMMAND "simxSetObjectOrientation"

const int inArgs_SETOBJECTORIENTATION[]={
    5,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_script_arg_table,3,
    sim_script_arg_int32,0,
};

void LUA_SETOBJECTORIENTATION_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_SETOBJECTORIENTATION,inArgs_SETOBJECTORIENTATION[0],LUA_SETOBJECTORIENTATION_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(4).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(85,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_SETOBJECTORIENTATION_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxSetObjectPosition
// --------------------------------------------------------------------------------------
#define LUA_SETOBJECTPOSITION_COMMAND "simxSetObjectPosition"

const int inArgs_SETOBJECTPOSITION[]={
    5,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_script_arg_table,3,
    sim_script_arg_int32,0,
};

void LUA_SETOBJECTPOSITION_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_SETOBJECTPOSITION,inArgs_SETOBJECTPOSITION[0],LUA_SETOBJECTPOSITION_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(4).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(86,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_SETOBJECTPOSITION_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxSetObjectParent
// --------------------------------------------------------------------------------------
#define LUA_SETOBJECTPARENT_COMMAND "simxSetObjectParent"

const int inArgs_SETOBJECTPARENT[]={
    5,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_bool,0,
    sim_script_arg_int32,0,
};

void LUA_SETOBJECTPARENT_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_SETOBJECTPARENT,inArgs_SETOBJECTPARENT[0],LUA_SETOBJECTPARENT_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(4).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(87,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_SETOBJECTPARENT_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxSetObjectSelection
// --------------------------------------------------------------------------------------
#define LUA_SETOBJECTSELECTION_COMMAND "simxSetObjectSelection"

const int inArgs_SETOBJECTSELECTION[]={
    3,
    sim_script_arg_int32,0,
    sim_script_arg_int32|sim_script_arg_table,0,
    sim_script_arg_int32,0,
};

void LUA_SETOBJECTSELECTION_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_SETOBJECTSELECTION,inArgs_SETOBJECTSELECTION[0],LUA_SETOBJECTSELECTION_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(2).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(88,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_SETOBJECTSELECTION_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxSetSphericalJointMatrix
// --------------------------------------------------------------------------------------
#define LUA_SETSPHERICALJOINTMATRIX_COMMAND "simxSetSphericalJointMatrix"

const int inArgs_SETSPHERICALJOINTMATRIX[]={
    4,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_script_arg_table,12,
    sim_script_arg_int32,0,
};

void LUA_SETSPHERICALJOINTMATRIX_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_SETSPHERICALJOINTMATRIX,inArgs_SETSPHERICALJOINTMATRIX[0],LUA_SETSPHERICALJOINTMATRIX_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(3).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(89,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_SETSPHERICALJOINTMATRIX_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxSetUIButtonLabel
// --------------------------------------------------------------------------------------
#define LUA_SETUIBUTTONLABEL_COMMAND "simxSetUIButtonLabel"

const int inArgs_SETUIBUTTONLABEL[]={
    6,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_string,0,
    sim_script_arg_string,0,
    sim_script_arg_int32,0,
};

void LUA_SETUIBUTTONLABEL_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_SETUIBUTTONLABEL,inArgs_SETUIBUTTONLABEL[0],LUA_SETUIBUTTONLABEL_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(5).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(90,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_SETUIBUTTONLABEL_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxSetUIButtonProperty
// --------------------------------------------------------------------------------------
#define LUA_SETUIBUTTONPROPERTY_COMMAND "simxSetUIButtonProperty"

const int inArgs_SETUIBUTTONPROPERTY[]={
    5,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_SETUIBUTTONPROPERTY_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_SETUIBUTTONPROPERTY,inArgs_SETUIBUTTONPROPERTY[0],LUA_SETUIBUTTONPROPERTY_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(4).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(91,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_SETUIBUTTONPROPERTY_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxSetUISlider
// --------------------------------------------------------------------------------------
#define LUA_SETUISLIDER_COMMAND "simxSetUISlider"

const int inArgs_SETUISLIDER[]={
    5,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_SETUISLIDER_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_SETUISLIDER,inArgs_SETUISLIDER[0],LUA_SETUISLIDER_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(4).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(92,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_SETUISLIDER_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxSynchronous
// --------------------------------------------------------------------------------------
#define LUA_SYNCHRONOUS_COMMAND "simxSynchronous"

const int inArgs_SYNCHRONOUS[]={
    2,
    sim_script_arg_int32,0,
    sim_script_arg_bool,0,
};

void LUA_SYNCHRONOUS_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_SYNCHRONOUS,inArgs_SYNCHRONOUS[0],LUA_SYNCHRONOUS_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(93,&D,true,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_SYNCHRONOUS_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxSynchronousTrigger
// --------------------------------------------------------------------------------------
#define LUA_SYNCHRONOUSTRIGGER_COMMAND "simxSynchronousTrigger"

const int inArgs_SYNCHRONOUSTRIGGER[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_SYNCHRONOUSTRIGGER_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_SYNCHRONOUSTRIGGER,inArgs_SYNCHRONOUSTRIGGER[0],LUA_SYNCHRONOUSTRIGGER_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(94,&D,true,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_SYNCHRONOUSTRIGGER_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxTransferFile
// --------------------------------------------------------------------------------------
#define LUA_TRANSFERFILE_COMMAND "simxTransferFile"

const int inArgs_TRANSFERFILE[]={
    5,
    sim_script_arg_int32,0,
    sim_script_arg_string,0,
    sim_script_arg_string,0,
    sim_script_arg_int32,0,
    sim_script_arg_int32,0,
};

void LUA_TRANSFERFILE_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_TRANSFERFILE,inArgs_TRANSFERFILE[0],LUA_TRANSFERFILE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(4).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(95,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_TRANSFERFILE_COMMAND,"Invalid client ID."); // output an error
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simxCallScriptFunction
// --------------------------------------------------------------------------------------
#define LUA_CALLSCRIPTFUNCTION_COMMAND "simxCallScriptFunction"

const int inArgs_CALLSCRIPTFUNCTION[]={
    9,
    sim_script_arg_int32,0,
    sim_script_arg_string,0,
    sim_script_arg_int32,0,
    sim_script_arg_string,0,
    sim_script_arg_int32|sim_script_arg_table,0,
    sim_script_arg_float|sim_script_arg_table,0,
    sim_script_arg_string|sim_script_arg_table,0,
    sim_script_arg_charbuff,0,
    sim_script_arg_int32,0,
};

void LUA_CALLSCRIPTFUNCTION_CALLBACK(SScriptCallBack* p)
{

    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_CALLSCRIPTFUNCTION,inArgs_CALLSCRIPTFUNCTION[0],LUA_CALLSCRIPTFUNCTION_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int clientId=inData->at(0).int32Data[0];
        int operationMode=inData->at(8).int32Data[0];
        int index=getIndexFromClientId(clientId);
        if (index!=-1)
            allRemoteApiClients[index]->handleCommand(97,&D,operationMode==simx_opmode_blocking,isScriptThreaded(p->scriptID)); // after calling this, index might not be valid anymore!!
        else
            simSetLastError(LUA_CALLSCRIPTFUNCTION_COMMAND,"Invalid client ID."); // output an error
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
        std::cout << "Error, could not find or correctly load the V-REP library. Cannot start 'LuaRemoteApiClient' plugin.\n";
        return(0); // Means error, V-REP will unload this plugin
    }
    if (getVrepProcAddresses(vrepLib)==0)
    {
        std::cout << "Error, could not find all required functions in the V-REP library. Cannot start 'LuaRemoteApiClient' plugin.\n";
        unloadVrepLibrary(vrepLib);
        return(0); // Means error, V-REP will unload this plugin
    }

    int vrepVer;
    simGetIntegerParameter(sim_intparam_program_version,&vrepVer);
    if (vrepVer<30200) // if V-REP version is smaller than 3.02.00
    {
        std::cout << "Sorry, your V-REP copy is somewhat old. Cannot start 'LuaRemoteApiClient' plugin.\n";
        unloadVrepLibrary(vrepLib);
        return(0); // Means error, V-REP will unload this plugin
    }

    // Register the new Lua commands:

    simRegisterScriptCallbackFunction(strConCat(LUA_START_COMMAND,"@","LuaRemoteApiClient"),strConCat("number clientId=",LUA_START_COMMAND,"(string connectionAddress,number connectionPort,boolean waitUntilConnected,boolean doNotReconnectOnceDisconnected,number timeOutInMs,number commThreadCycleInMs=5)"),LUA_START_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_FINISH_COMMAND,"@","LuaRemoteApiClient"),strConCat("",LUA_FINISH_COMMAND,"(number clientId)"),LUA_FINISH_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_ADDSTATUSBARMESSAGE_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_ADDSTATUSBARMESSAGE_COMMAND,"(number clientId,string message,number operationMode)"),LUA_ADDSTATUSBARMESSAGE_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_AUXILIARYCONSOLECLOSE_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_AUXILIARYCONSOLECLOSE_COMMAND,"(number clientId,number consoleHandle,number operationMode)"),LUA_AUXILIARYCONSOLECLOSE_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_AUXILIARYCONSOLEOPEN_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,number consoleHandle=",LUA_AUXILIARYCONSOLEOPEN_COMMAND,"(number clientId,string title,number maxLines,number mode,table_2 position,table_2 size,table_3 textColor,table_3 backgroundColor,number operationMode)"),LUA_AUXILIARYCONSOLEOPEN_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_AUXILIARYCONSOLEPRINT_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_AUXILIARYCONSOLEPRINT_COMMAND,"(number clientId,number consoleHandle,string txt,number operationMode)"),LUA_AUXILIARYCONSOLEPRINT_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_AUXILIARYCONSOLESHOW_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_AUXILIARYCONSOLESHOW_COMMAND,"(number clientId,number consoleHandle,boolean showState,number operationMode)"),LUA_AUXILIARYCONSOLESHOW_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_BREAKFORCESENSOR_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_BREAKFORCESENSOR_COMMAND,"(number clientId,number forceSensorHandle,number operationMode)"),LUA_BREAKFORCESENSOR_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_CLEARFLOATSIGNAL_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_CLEARFLOATSIGNAL_COMMAND,"(number clientId,string signalName,number operationMode)"),LUA_CLEARFLOATSIGNAL_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_CLEARINTEGERSIGNAL_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_CLEARINTEGERSIGNAL_COMMAND,"(number clientId,string signalName,number operationMode)"),LUA_CLEARINTEGERSIGNAL_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_CLEARSTRINGSIGNAL_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_CLEARSTRINGSIGNAL_COMMAND,"(number clientId,string signalName,number operationMode)"),LUA_CLEARSTRINGSIGNAL_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETOBJECTHANDLE_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,number objectHandle=",LUA_GETOBJECTHANDLE_COMMAND,"(number clientId,string objectName,number operationMode)"),LUA_GETOBJECTHANDLE_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_CLOSESCENE_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_CLOSESCENE_COMMAND,"(number clientId,number operationMode)"),LUA_CLOSESCENE_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_COPYPASTEOBJECTS_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,table newObjectHandles=",LUA_COPYPASTEOBJECTS_COMMAND,"(number clientId,table objectHandles,number operationMode)"),LUA_COPYPASTEOBJECTS_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_LOADSCENE_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_LOADSCENE_COMMAND,"(number clientId,string scenePathAndName,number options,number operationMode)"),LUA_LOADSCENE_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_LOADMODEL_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,number baseHandle=",LUA_LOADMODEL_COMMAND,"(number clientId,string modelPathAndName,number options,number operationMode)"),LUA_LOADMODEL_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_LOADUI_COMMAND,"@","LuaRemoteApiClient"),"DEPRECATED. Use the Qt-based custom UIs instead.",LUA_LOADUI_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_SETBOOLEANPARAMETER_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_SETBOOLEANPARAMETER_COMMAND,"(number clientId,number paramIdentifier,boolean paramValue,number operationMode)"),LUA_SETBOOLEANPARAMETER_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_STARTSIMULATION_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_STARTSIMULATION_COMMAND,"(number clientId,number operationMode)"),LUA_STARTSIMULATION_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_STOPSIMULATION_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_STOPSIMULATION_COMMAND,"(number clientId,number operationMode)"),LUA_STOPSIMULATION_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_SETFLOATINGPARAMETER_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_SETFLOATINGPARAMETER_COMMAND,"(number clientId,number paramIdentifier,number paramValue,number operationMode)"),LUA_SETFLOATINGPARAMETER_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_SETINTEGERPARAMETER_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_SETINTEGERPARAMETER_COMMAND,"(number clientId,number paramIdentifier,number paramValue,number operationMode)"),LUA_SETINTEGERPARAMETER_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_SETARRAYPARAMETER_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_SETARRAYPARAMETER_COMMAND,"(number clientId,number paramIdentifier,table_3 paramValues,number operationMode)"),LUA_SETARRAYPARAMETER_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETBOOLEANPARAMETER_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,boolean paramValue=",LUA_GETBOOLEANPARAMETER_COMMAND,"(number clientId,number paramIdentifier,number operationMode)"),LUA_GETBOOLEANPARAMETER_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETFLOATINGPARAMETER_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,number paramValue=",LUA_GETFLOATINGPARAMETER_COMMAND,"(number clientId,number paramIdentifier,number operationMode)"),LUA_GETFLOATINGPARAMETER_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETINTEGERPARAMETER_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,number paramValue=",LUA_GETINTEGERPARAMETER_COMMAND,"(number clientId,number paramIdentifier,number operationMode)"),LUA_GETINTEGERPARAMETER_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETARRAYPARAMETER_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,table_3 paramValues=",LUA_GETARRAYPARAMETER_COMMAND,"(number clientId,number paramIdentifier,number operationMode)"),LUA_GETARRAYPARAMETER_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETSTRINGPARAMETER_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,string paramValue=",LUA_GETSTRINGPARAMETER_COMMAND,"(number clientId,number paramIdentifier,number operationMode)"),LUA_GETSTRINGPARAMETER_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_SETFLOATSIGNAL_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_SETFLOATSIGNAL_COMMAND,"(number clientId,string signalName,number signalValue,number operationMode)"),LUA_SETFLOATSIGNAL_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_SETINTEGERSIGNAL_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_SETINTEGERSIGNAL_COMMAND,"(number clientId,string signalName,number signalValue,number operationMode)"),LUA_SETINTEGERSIGNAL_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_SETSTRINGSIGNAL_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_SETSTRINGSIGNAL_COMMAND,"(number clientId,string signalName,string signalValue,number operationMode)"),LUA_SETSTRINGSIGNAL_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETFLOATSIGNAL_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,number signalValue=",LUA_GETFLOATSIGNAL_COMMAND,"(number clientId,string signalName,number operationMode)"),LUA_GETFLOATSIGNAL_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETINTEGERSIGNAL_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,number signalValue=",LUA_GETINTEGERSIGNAL_COMMAND,"(number clientId,string signalName,number operationMode)"),LUA_GETINTEGERSIGNAL_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETSTRINGSIGNAL_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,string signalValue=",LUA_GETSTRINGSIGNAL_COMMAND,"(number clientId,string signalName,number operationMode)"),LUA_GETSTRINGSIGNAL_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_CREATEDUMMY_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,number dummyHandle=",LUA_CREATEDUMMY_COMMAND,"(number clientId,number size,table_12 colors,number operationMode)"),LUA_CREATEDUMMY_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_DISPLAYDIALOG_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,number dialogHandle,number UiHandle=",LUA_DISPLAYDIALOG_COMMAND,"(number clientId,string titleText,string mainText,\nnumber dialogType,string initialText,table_6 titleColors,table_6 dialogColors,number operationMode)"),LUA_DISPLAYDIALOG_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_ENDDIALOG_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_ENDDIALOG_COMMAND,"(number clientId,number dialogHandle,number operationMode)"),LUA_ENDDIALOG_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETDIALOGINPUT_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,string inputText=",LUA_GETDIALOGINPUT_COMMAND,"(number clientId,number dialogHandle,number operationMode)"),LUA_GETDIALOGINPUT_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETDIALOGRESULT_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,number result=",LUA_GETDIALOGRESULT_COMMAND,"(number clientId,number dialogHandle,number operationMode)"),LUA_GETDIALOGRESULT_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_ERASEFILE_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_ERASEFILE_COMMAND,"(number clientId,string fileName_serverSide,number operationMode)"),LUA_ERASEFILE_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETCOLLISIONHANDLE_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,number collisionHandle=",LUA_GETCOLLISIONHANDLE_COMMAND,"(number clientId,string collisionObjectName,number operationMode)"),LUA_GETCOLLISIONHANDLE_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETCOLLECTIONHANDLE_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,number collectionHandle=",LUA_GETCOLLECTIONHANDLE_COMMAND,"(number clientId,string collectionName,number operationMode)"),LUA_GETCOLLECTIONHANDLE_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETCONNECTIONID_COMMAND,"@","LuaRemoteApiClient"),strConCat("number connectionId=",LUA_GETCONNECTIONID_COMMAND,"(number clientId)"),LUA_GETCONNECTIONID_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETDISTANCEHANDLE_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,number distanceHandle=",LUA_GETDISTANCEHANDLE_COMMAND,"(number clientId,string distanceObjectName,number operationMode)"),LUA_GETDISTANCEHANDLE_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETINMESSAGEINFO_COMMAND,"@","LuaRemoteApiClient"),strConCat("number result,number info=",LUA_GETINMESSAGEINFO_COMMAND,"(number clientId,number infoType)"),LUA_GETINMESSAGEINFO_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETOUTMESSAGEINFO_COMMAND,"@","LuaRemoteApiClient"),strConCat("number result,number info=",LUA_GETOUTMESSAGEINFO_COMMAND,"(number clientId,number infoType)"),LUA_GETOUTMESSAGEINFO_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETJOINTFORCE_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,number force=",LUA_GETJOINTFORCE_COMMAND,"(number clientId,number jointHandle,number operationMode)"),LUA_GETJOINTFORCE_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETJOINTMATRIX_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,table_12 matrix=",LUA_GETJOINTMATRIX_COMMAND,"(number clientId,number jointHandle,number operationMode)"),LUA_GETJOINTMATRIX_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETJOINTPOSITION_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,number position=",LUA_GETJOINTPOSITION_COMMAND,"(number clientId,number jointHandle,number operationMode)"),LUA_GETJOINTPOSITION_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETLASTCMDTIME_COMMAND,"@","LuaRemoteApiClient"),strConCat("number simulationTimeInMs=",LUA_GETLASTCMDTIME_COMMAND,"(number clientId)"),LUA_GETLASTCMDTIME_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETLASTERRORS_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,table errorStrings=",LUA_GETLASTERRORS_COMMAND,"(number clientId,number operationMode)"),LUA_GETLASTERRORS_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETMODELPROPERTY_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,number property=",LUA_GETMODELPROPERTY_COMMAND,"(number clientId,number objectHandle,number operationMode)"),LUA_GETMODELPROPERTY_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETOBJECTCHILD_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,number childObjectHandle=",LUA_GETOBJECTCHILD_COMMAND,"(number clientId,number parentObjectHandle,number childIndex,number operationMode)"),LUA_GETOBJECTCHILD_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETOBJECTFLOATPARAMETER_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,number parameterValue=",LUA_GETOBJECTFLOATPARAMETER_COMMAND,"(number clientId,number objectHandle,number parameterId,number operationMode)"),LUA_GETOBJECTFLOATPARAMETER_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETOBJECTINTPARAMETER_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,number parameterValue=",LUA_GETOBJECTINTPARAMETER_COMMAND,"(number clientId,number objectHandle,number parameterId,number operationMode)"),LUA_GETOBJECTINTPARAMETER_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETOBJECTGROUPDATA_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,table handles,table intData,table floatData,table stringData=",LUA_GETOBJECTGROUPDATA_COMMAND,"(number clientId,number objectType,number dataType,number operationMode)"),LUA_GETOBJECTGROUPDATA_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETOBJECTORIENTATION_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,table_3 eulerAngles=",LUA_GETOBJECTORIENTATION_COMMAND,"(number clientId,number objectHandle,number relativeToObjectHandle,number operationMode)"),LUA_GETOBJECTORIENTATION_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETOBJECTPOSITION_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,table_3 position=",LUA_GETOBJECTPOSITION_COMMAND,"(number clientId,number objectHandle,number relativeToObjectHandle,number operationMode)"),LUA_GETOBJECTPOSITION_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETOBJECTPARENT_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,number parentObjectHandle=",LUA_GETOBJECTPARENT_COMMAND,"(number clientId,number objectHandle,number operationMode)"),LUA_GETOBJECTPARENT_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETOBJECTS_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,table objectHandles=",LUA_GETOBJECTS_COMMAND,"(number clientId,number objectType,number operationMode)"),LUA_GETOBJECTS_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETOBJECTSELECTION_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,table objectHandles=",LUA_GETOBJECTSELECTION_COMMAND,"(number clientId,number operationMode)"),LUA_GETOBJECTSELECTION_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETOBJECTVELOCITY_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,table_3 linearVelocity,table_3 angularVelocity=",LUA_GETOBJECTVELOCITY_COMMAND,"(number clientId,number objectHandle,number operationMode)"),LUA_GETOBJECTVELOCITY_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETPINGTIME_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,number pingTime=",LUA_GETPINGTIME_COMMAND,"(number clientId)"),LUA_GETPINGTIME_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETUIBUTTONPROPERTY_COMMAND,"@","LuaRemoteApiClient"),"DEPRECATED. Use the Qt-based custom UIs instead.",LUA_GETUIBUTTONPROPERTY_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETUIEVENTBUTTON_COMMAND,"@","LuaRemoteApiClient"),"DEPRECATED. Use the Qt-based custom UIs instead.",LUA_GETUIEVENTBUTTON_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETUIHANDLE_COMMAND,"@","LuaRemoteApiClient"),"DEPRECATED. Use the Qt-based custom UIs instead.",LUA_GETUIHANDLE_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETUISLIDER_COMMAND,"@","LuaRemoteApiClient"),"DEPRECATED. Use the Qt-based custom UIs instead.",LUA_GETUISLIDER_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETVISIONSENSORDEPTHBUFFER_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,table buffer,table_2 resolution=",LUA_GETVISIONSENSORDEPTHBUFFER_COMMAND,"(number clientId,number sensorHandle,number operationMode)"),LUA_GETVISIONSENSORDEPTHBUFFER_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_GETVISIONSENSORIMAGE_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,string image,table_2 resolution=",LUA_GETVISIONSENSORIMAGE_COMMAND,"(number clientId,number sensorHandle,number options,number operationMode)"),LUA_GETVISIONSENSORIMAGE_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_SETVISIONSENSORIMAGE_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_SETVISIONSENSORIMAGE_COMMAND,"(number clientId,number sensorHandle,string image,number options,number operationMode)"),LUA_SETVISIONSENSORIMAGE_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_PAUSECOMMUNICATION_COMMAND,"@","LuaRemoteApiClient"),strConCat("number result=",LUA_PAUSECOMMUNICATION_COMMAND,"(number clientId,boolean pause)"),LUA_PAUSECOMMUNICATION_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_PAUSESIMULATION_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_PAUSESIMULATION_COMMAND,"(number clientId,number operationMode)"),LUA_PAUSESIMULATION_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_QUERY_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,string retSignalValue=",LUA_QUERY_COMMAND,"(number clientId,string signalName,string signalValue,string retSignalName,number timeOutInMs)"),LUA_QUERY_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_READCOLLISION_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,boolean collisionState=",LUA_READCOLLISION_COMMAND,"(number clientId,number collisionObjectHandle,number operationMode)"),LUA_READCOLLISION_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_READDISTANCE_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,number minimumDistance=",LUA_READDISTANCE_COMMAND,"(number clientId,number distanceObjectHandle,number operationMode)"),LUA_READDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_READFORCESENSOR_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,number state,table_3 force,table_3 torque=",LUA_READFORCESENSOR_COMMAND,"(number clientId,number forceSensorHandle,number operationMode)"),LUA_READFORCESENSOR_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_READPROXIMITYSENSOR_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,boolean detectionState,table_3 detectedPoint,number objectHandle,table_3 normalVector=",LUA_READPROXIMITYSENSOR_COMMAND,"(number clientId,number proximitySensorHandle,number operationMode)"),LUA_READPROXIMITYSENSOR_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_READSTRINGSTREAM_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,string signalValue=",LUA_READSTRINGSTREAM_COMMAND,"(number clientId,string signalName,number operationMode)"),LUA_READSTRINGSTREAM_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_WRITESTRINGSTREAM_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_WRITESTRINGSTREAM_COMMAND,"(number clientId,string signalName,string signalValueToAppend,number operationMode)"),LUA_WRITESTRINGSTREAM_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_READVISIONSENSOR_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,table packet1,table packet2,.. =",LUA_READVISIONSENSOR_COMMAND,"(number clientId,number visionSensorHandle,number operationMode)"),LUA_READVISIONSENSOR_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_REMOVEMODEL_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_REMOVEMODEL_COMMAND,"(number clientId,number objectHandle,number operationMode)"),LUA_REMOVEMODEL_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_REMOVEOBJECT_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_REMOVEOBJECT_COMMAND,"(number clientId,number objectHandle,number operationMode)"),LUA_REMOVEOBJECT_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_REMOVEUI_COMMAND,"@","LuaRemoteApiClient"),"DEPRECATED. Use the Qt-based custom UIs instead.",LUA_REMOVEUI_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_SETJOINTFORCE_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_SETJOINTFORCE_COMMAND,"(number clientId,number jointHandle,number force,number operationMode)"),LUA_SETJOINTFORCE_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_SETJOINTPOSITION_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_SETJOINTPOSITION_COMMAND,"(number clientId,number jointHandle,number position,number operationMode)"),LUA_SETJOINTPOSITION_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_SETJOINTTARGETPOSITION_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_SETJOINTTARGETPOSITION_COMMAND,"(number clientId,number jointHandle,number targetPosition,number operationMode)"),LUA_SETJOINTTARGETPOSITION_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_SETJOINTTARGETVELOCITY_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_SETJOINTTARGETVELOCITY_COMMAND,"(number clientId,number jointHandle,number targetVelocity,number operationMode)"),LUA_SETJOINTTARGETVELOCITY_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_SETMODELPROPERTY_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_SETMODELPROPERTY_COMMAND,"(number clientId,number objectHandle,number modelProperty,number operationMode)"),LUA_SETMODELPROPERTY_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_SETOBJECTORIENTATION_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_SETOBJECTORIENTATION_COMMAND,"(number clientId,number objectHandle,number relativeToObjectHandle,table_3 eulerAngles,number operationMode)"),LUA_SETOBJECTORIENTATION_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_SETOBJECTPOSITION_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_SETOBJECTPOSITION_COMMAND,"(number clientId,number objectHandle,number relativeToObjectHandle,table_3 eulerAngles,number operationMode)"),LUA_SETOBJECTPOSITION_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_SETOBJECTPARENT_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_SETOBJECTPARENT_COMMAND,"(number clientId,number objectHandle,number parentObject,boolean keepInPlace,number operationMode)"),LUA_SETOBJECTPARENT_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_SETOBJECTSELECTION_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_SETOBJECTSELECTION_COMMAND,"(number clientId,table objectHandles,number operationMode)"),LUA_SETOBJECTSELECTION_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_SETSPHERICALJOINTMATRIX_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_SETSPHERICALJOINTMATRIX_COMMAND,"(number clientId,number jointHandle,table_12 matrix,number operationMode)"),LUA_SETSPHERICALJOINTMATRIX_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_SETUIBUTTONLABEL_COMMAND,"@","LuaRemoteApiClient"),"DEPRECATED. Use the Qt-based custom UIs instead.",LUA_SETUIBUTTONLABEL_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_SETUIBUTTONPROPERTY_COMMAND,"@","LuaRemoteApiClient"),"DEPRECATED. Use the Qt-based custom UIs instead.",LUA_SETUIBUTTONPROPERTY_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_SETUISLIDER_COMMAND,"@","LuaRemoteApiClient"),"DEPRECATED. Use the Qt-based custom UIs instead.",LUA_SETUISLIDER_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_SYNCHRONOUS_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_SYNCHRONOUS_COMMAND,"(number clientId,boolean enable)"),LUA_SYNCHRONOUS_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_SYNCHRONOUSTRIGGER_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_SYNCHRONOUSTRIGGER_COMMAND,"(number clientId)"),LUA_SYNCHRONOUSTRIGGER_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_TRANSFERFILE_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode=",LUA_TRANSFERFILE_COMMAND,"(number clientId,string filePathAndName,string filename_serverSide,number timeOut,number operationMode)"),LUA_TRANSFERFILE_CALLBACK);
    simRegisterScriptCallbackFunction(strConCat(LUA_CALLSCRIPTFUNCTION_COMMAND,"@","LuaRemoteApiClient"),strConCat("number returnCode,table retInts,table retFloats,table retStrings,string retBuffer=",LUA_CALLSCRIPTFUNCTION_COMMAND,"(number clientId,string scriptDescription,number scriptHandleOrType,\nstring functionName,table inInts,table inFloats,table inStrings,string inBuffer,number operationMode)"),LUA_CALLSCRIPTFUNCTION_CALLBACK);

    simRegisterScriptVariable("simx_return_ok",(boost::lexical_cast<std::string>(int(simx_return_ok))).c_str(),0);
    simRegisterScriptVariable("simx_return_novalue_flag",(boost::lexical_cast<std::string>(int(simx_return_novalue_flag))).c_str(),0);  
    simRegisterScriptVariable("simx_return_timeout_flag",(boost::lexical_cast<std::string>(int(simx_return_timeout_flag))).c_str(),0);  
    simRegisterScriptVariable("simx_return_illegal_opmode_flag",(boost::lexical_cast<std::string>(int(simx_return_illegal_opmode_flag))).c_str(),0);    
    simRegisterScriptVariable("simx_return_remote_error_flag",(boost::lexical_cast<std::string>(int(simx_return_remote_error_flag))).c_str(),0);    
    simRegisterScriptVariable("simx_return_split_progress_flag",(boost::lexical_cast<std::string>(int(simx_return_split_progress_flag))).c_str(),0);    
    simRegisterScriptVariable("simx_return_local_error_flag",(boost::lexical_cast<std::string>(int(simx_return_local_error_flag))).c_str(),0);  
    simRegisterScriptVariable("simx_return_initialize_error_flag",(boost::lexical_cast<std::string>(int(simx_return_initialize_error_flag))).c_str(),0);    

    simRegisterScriptVariable("simx_opmode_oneshot",(boost::lexical_cast<std::string>(int(simx_opmode_oneshot))).c_str(),0);    
    simRegisterScriptVariable("simx_opmode_blocking",(boost::lexical_cast<std::string>(int(simx_opmode_blocking))).c_str(),0);  
    simRegisterScriptVariable("simx_opmode_oneshot_wait",(boost::lexical_cast<std::string>(int(simx_opmode_blocking))).c_str(),0);  
    simRegisterScriptVariable("simx_opmode_streaming",(boost::lexical_cast<std::string>(int(simx_opmode_streaming))).c_str(),0);    
    simRegisterScriptVariable("simx_opmode_oneshot_split",(boost::lexical_cast<std::string>(int(simx_opmode_oneshot_split))).c_str(),0);    
    simRegisterScriptVariable("simx_opmode_streaming_split",(boost::lexical_cast<std::string>(int(simx_opmode_streaming_split))).c_str(),0);    
    simRegisterScriptVariable("simx_opmode_discontinue",(boost::lexical_cast<std::string>(int(simx_opmode_discontinue))).c_str(),0);    
    simRegisterScriptVariable("simx_opmode_buffer",(boost::lexical_cast<std::string>(int(simx_opmode_buffer))).c_str(),0);  
    simRegisterScriptVariable("simx_opmode_remove",(boost::lexical_cast<std::string>(int(simx_opmode_remove))).c_str(),0);  

    simRegisterScriptVariable("simx_headeroffset_version",(boost::lexical_cast<std::string>(int(simx_headeroffset_version))).c_str(),0);    
    simRegisterScriptVariable("simx_headeroffset_message_id",(boost::lexical_cast<std::string>(int(simx_headeroffset_message_id))).c_str(),0);  
    simRegisterScriptVariable("simx_headeroffset_client_time",(boost::lexical_cast<std::string>(int(simx_headeroffset_client_time))).c_str(),0);    
    simRegisterScriptVariable("simx_headeroffset_server_time",(boost::lexical_cast<std::string>(int(simx_headeroffset_server_time))).c_str(),0);    
    simRegisterScriptVariable("simx_headeroffset_scene_id",(boost::lexical_cast<std::string>(int(simx_headeroffset_scene_id))).c_str(),0);  
    simRegisterScriptVariable("simx_headeroffset_server_state",(boost::lexical_cast<std::string>(int(simx_headeroffset_server_state))).c_str(),0);  

    simRegisterScriptVariable("simx_cmdheaderoffset_mem_size",(boost::lexical_cast<std::string>(int(simx_cmdheaderoffset_mem_size))).c_str(),0);    
    simRegisterScriptVariable("simx_cmdheaderoffset_full_mem_size",(boost::lexical_cast<std::string>(int(simx_cmdheaderoffset_full_mem_size))).c_str(),0);  
    simRegisterScriptVariable("simx_cmdheaderoffset_cmd",(boost::lexical_cast<std::string>(int(simx_cmdheaderoffset_cmd))).c_str(),0);  
    simRegisterScriptVariable("simx_cmdheaderoffset_sim_time",(boost::lexical_cast<std::string>(int(simx_cmdheaderoffset_sim_time))).c_str(),0);    
    simRegisterScriptVariable("simx_cmdheaderoffset_status",(boost::lexical_cast<std::string>(int(simx_cmdheaderoffset_status))).c_str(),0);    

    return(SIMX_VERSION); // initialization went fine, we return the version number of this extension module (can be queried with simGetModuleName)
}

// This is the plugin end routine:
VREP_DLLEXPORT void v_repEnd()
{ // This is called just once, at the end of V-REP

    for (unsigned int i=0;i<allRemoteApiClients.size();i++)
        delete allRemoteApiClients[i];
    
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
    { // This message is sent each time the scene was rendered (well, shortly after) (very often)

    }

    if (message==sim_message_eventcallback_mainscriptabouttobecalled)
    { // Main script is about to be run

    }

    if (message==sim_message_eventcallback_simulationended)
    { // Simulation just ended
        for (unsigned int i=0;i<allRemoteApiClients.size();i++)
        {
            if (allRemoteApiClients[i]->getDestroyAtSimulationEnd())
            {
                delete allRemoteApiClients[i];
                allRemoteApiClients.erase(allRemoteApiClients.begin()+i);
                i--; // reprocess this position
            }
        }
    }

    simSetIntegerParameter(sim_intparam_error_report_mode,errorModeSaved); // restore previous settings
    return(retVal);
}

