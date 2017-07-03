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

#include "remoteApiLink.h"
#include <iostream>
#include <sstream>

extern "C" {
    #include "extApi.h"
/*  #include "extApiCustom.h" if you wanna use custom remote API functions! */
}

#ifndef _WIN32
    #define Sleep(x) usleep(1000*x)
#endif

CRemoteApiLink::CRemoteApiLink(const char* ip,int portNb,bool waitUntilConnected,bool doNotReconnect,int timeOut,int cycleTime)
{
    _ipAddress=std::string(ip);
    _portNb=portNb;
    _waitUntilConnected=waitUntilConnected;
    _doNotReconnect=doNotReconnect;
    _timeOut=timeOut;
    _cycleTime=cycleTime;
    _threadLaunched=false;
    _threadEnded=true;
    _destroyAtSimulationEnd=false;
    _clientId=-2;
//  _lockLevel=0;

#ifdef _WIN32
    _mutex=CreateMutex(0,FALSE,0);
//  _mutexAux=CreateMutex(0,FALSE,0);
#elif defined (__linux) || defined (__APPLE__)
    pthread_mutex_init(&_mutex, NULL);
//  pthread_mutex_init(&_mutexAux, NULL);
#endif /* __linux || __APPLE__ */
}

CRemoteApiLink::~CRemoteApiLink()
{
    _stop();
#ifdef _WIN32
//  CloseHandle(_mutexAux);
    CloseHandle(_mutex);
#elif defined (__linux) || defined (__APPLE__)
//  pthread_mutex_destroy(&_mutexAux);
    pthread_mutex_destroy(&_mutex);
#endif /* __linux || __APPLE__ */
}

int CRemoteApiLink::getPortNb()
{
    return(_portNb);
}

int CRemoteApiLink::getClientId()
{
    return(_clientId);
}

void CRemoteApiLink::setDestroyAtSimulationEnd(bool destroy)
{
    _destroyAtSimulationEnd=destroy;
}

bool CRemoteApiLink::getDestroyAtSimulationEnd()
{
    return(_destroyAtSimulationEnd);
}


#ifdef _WIN32
DWORD WINAPI CRemoteApiLink::_staticThreadProc(LPVOID arg)
{
    reinterpret_cast<CRemoteApiLink*>(arg)->_run();
    return(0);
}
#endif /* _WIN32 */

#if defined (__linux) || defined (__APPLE__)
void* CRemoteApiLink::_staticThreadProc(void *arg)
{
    return(reinterpret_cast<CRemoteApiLink*>(arg)->_run());
}
#endif /* __linux || __APPLE__ */

void CRemoteApiLink::start()
{
    if (!_threadLaunched)
    {
        _threadEnded=false;
#ifdef _WIN32
        CreateThread(NULL,0,&CRemoteApiLink::_staticThreadProc,this,THREAD_PRIORITY_NORMAL,NULL);
#endif /* _WIN32 */
#if defined (__linux) || defined (__APPLE__)
        pthread_create(&_theThread,NULL,&CRemoteApiLink::_staticThreadProc,this);
#endif /* __linux || __APPLE__ */
        while ( (!_threadLaunched)&&(!_threadEnded) )
            Sleep(10);
    }
}

void CRemoteApiLink::_stop()
{
    // Terminate the thread if needed:
    if (_threadLaunched)
    {
        _threadLaunched=false; // signal the thread we wanna end

        // Wait until the thread ended:
        while (true)
        {
            waitXms(1);
            if (_threadEnded)
                break;
        }
        waitXms(10);
    }
}

void CRemoteApiLink::_simpleLock(MUTEX_HANDLE_X mutex)
{
#ifdef _WIN32
    while (WaitForSingleObject(mutex,INFINITE)!=WAIT_OBJECT_0)
        Sleep(1);
#elif defined (__linux) || defined (__APPLE__)
    while (pthread_mutex_lock(mutex)==-1)
        Sleep(1);
#endif
}

void CRemoteApiLink::_simpleUnlock(MUTEX_HANDLE_X mutex)
{
#ifdef _WIN32
    ReleaseMutex(mutex);
#elif defined (__linux) || defined (__APPLE__)
    pthread_mutex_unlock(mutex);
#endif
}

void CRemoteApiLink::_lock()
{
#ifdef _WIN32
    _simpleLock(_mutex);
#elif defined (__linux) || defined (__APPLE__)
    _simpleLock(&_mutex);
#endif
/*
#ifdef _WIN32
    _simpleLock(_mutexAux);
    if ( (GetCurrentThreadId()==_lockThreadId) && (_lockLevel>0) )
    { // Already locked by this thread
        _lockLevel++;
        _simpleUnlock(_mutexAux);
        return;
    }
    // First level lock
    _simpleUnlock(_mutexAux);
    _simpleLock(_mutex);
    _simpleLock(_mutexAux);
    _lockThreadId=GetCurrentThreadId();
    _lockLevel=1;
    _simpleUnlock(_mutexAux);
#elif defined (__linux) || defined (__APPLE__)
    _simpleLock(&_mutexAux);
    if ( (pthread_self()==_lockThreadId) && (_lockLevel>0) )
    { // Already locked by this thread
        _lockLevel++;
        _simpleUnlock(&_mutexAux);
        return;
    }
    // First level lock
    _simpleUnlock(&_mutexAux);
    _simpleLock(&_mutex);
    _simpleLock(&_mutexAux);
    _lockThreadId=pthread_self();
    _lockLevel=1;
    _simpleUnlock(&_mutexAux);
#endif
    */
}

void CRemoteApiLink::_unlock()
{
#ifdef _WIN32
    _simpleUnlock(_mutex);
#elif defined (__linux) || defined (__APPLE__)
    _simpleUnlock(&_mutex);
#endif
/*
#ifdef _WIN32
    _simpleLock(_mutexAux);
    _lockLevel--;
    if (_lockLevel==0)
    {
        _simpleUnlock(_mutexAux);
        _simpleUnlock(_mutex);
    }
    else
        _simpleUnlock(_mutexAux);
#elif defined (__linux) || defined (__APPLE__)
    _simpleLock(&_mutexAux);
    _lockLevel--;
    if (_lockLevel==0)
    {
        _simpleUnlock(&_mutexAux);
        _simpleUnlock(&_mutex);
    }
    else
        _simpleUnlock(&_mutexAux);
#endif
        */
}

void* CRemoteApiLink::_run()
{
    _threadLaunched=true;

    _clientId=simxStart(_ipAddress.c_str(),_portNb,_waitUntilConnected,_doNotReconnect,_timeOut,_cycleTime);

    if (_clientId>=0)
    {
        while (_threadLaunched)
        { // we start in the loop and handle the lua commands that come in:
            bool foundACommand=true;
            while (foundACommand)
            {
                _lock();
                foundACommand=false;
                for (unsigned int i=0;i<_commandsToHandle.size();i++)
                {
                    if ( (_commandsToHandle[i]!=NULL)&&(_executionStatus[i][0]==0) )
                    {
                        int cmdId=_commandIds[i];
                        CScriptFunctionData* inOutData=_commandsToHandle[i];
                        int* status=_executionStatus[i];
                        _unlock();
                        _handleCommand(cmdId,inOutData);
                        status[0]=1; // to signal that the command was handled
                        foundACommand=true;
                        _lock();
                    }
                }
                _unlock();
            }
            #ifdef _WIN32
                Sleep(1);
            #else
                usleep(1000);
            #endif
        }
    }
    else
    { // just wait until this thread gets terminated
        while (_threadLaunched)
            #ifdef _WIN32
                Sleep(1);
            #else
                usleep(1000);
            #endif
    }

    _lock();
    bool waitABit=false;
    for (unsigned int i=0;i<_executionStatus.size();i++)
    {
        if (_commandsToHandle[i]!=NULL)
        {
            _executionStatus[i][0]=1;
            waitABit=true;
        }
    }
    if (waitABit)
    {
        #ifdef _WIN32
            Sleep(100);
        #else
            usleep(100000);
        #endif
    }

    if (_clientId>=0)
    { // make sure to call simxFinish on that client, otherwise the remote API will still have running threads
        simxFinish(_clientId);
    }
    _unlock();

    _threadLaunched=false;
    _threadEnded=true; // confirmation that is needed
    return(NULL);
}

#ifdef _WIN32
unsigned int CRemoteApiLink::getTimeInMs(void)
{
    return(timeGetTime()&0x03ffffff);
}
#endif /* _WIN32 */

#if defined (__linux) || defined (__APPLE__)
unsigned int CRemoteApiLink::getTimeInMs(void)
{
    struct timeval tv;
    unsigned int result=0;
    if (gettimeofday(&tv,NULL)==0)
        result=(tv.tv_sec*1000+tv.tv_usec/1000)&0x03ffffff;
    return(result);
}
#endif /* __linux || __APPLE__ */

unsigned int CRemoteApiLink::getTimeDiffInMs(unsigned int lastTime)
{
    unsigned int currentTime=getTimeInMs();
    if (currentTime<lastTime)
        return(currentTime+0x03ffffff-lastTime);
    return(currentTime-lastTime);
}

void CRemoteApiLink::waitXms(unsigned int xms)
{
    unsigned int startTime=getTimeInMs();
    while (getTimeDiffInMs(startTime)<xms)
#ifdef _WIN32
        Sleep(1);
#else
        usleep(1000);
#endif
}

void CRemoteApiLink::handleCommand(int cmdId,CScriptFunctionData* inOutData,bool blocking,bool threaded)
{
    // Insert the command:
    _lock();
    int index=-1;
    for (unsigned int i=0;i<_commandsToHandle.size();i++)
    {
        if (_commandsToHandle[i]==NULL)
        {
            index=i;
            break;
        }
    }
    int status=0;
    if (index!=-1)
    {
        _commandIds[index]=cmdId;
        _commandsToHandle[index]=inOutData;
        _executionStatus[index]=&status;
    }
    else
    {
        index=(int)_commandsToHandle.size();
        _commandIds.push_back(cmdId);
        _commandsToHandle.push_back(inOutData);
        _executionStatus.push_back(&status);
    }
    _unlock();

    // Now wait here until the command was executed:
    while (true)
    {
        if (blocking)
        {
            if (threaded)
            { // we have a threaded script calling. We wait while switching it. V-REP is NOT blocking during that time:
                if (simSwitchThread()<1)
                { // the switch failed (e.g. simSetThreadAutomaticSwitch(false) was maybe called)
                    #ifdef _WIN32
                    Sleep(0);
                    #else
                    usleep(10);
                    #endif
                }
            }
            else
            { // we have a non-threaded script calling. We wait a bit. V-REP is blocking during that time: 
                #ifdef _WIN32
                    Sleep(1);
                #else
                    usleep(1000);
                #endif
            }
        }
        else
        { // simply do a thread switch (very short wait). V-REP is blocking during that time:
            #ifdef _WIN32
                Sleep(0);
            #else
                usleep(10);
            #endif
        }
        if (status!=0)
            break;
    }
    _lock();
    _commandsToHandle[index]=NULL; // free that slot
    _unlock();
}

void CRemoteApiLink::_handleCommand(int cmdId,CScriptFunctionData* inOutData)
{ // simxStart and simxFinish are handled elsewhere
    std::vector<CScriptFunctionDataItem>* inData=inOutData->getInDataPtr();
    if (cmdId==0)
    { // simxAddStatusbarMessage
        int res=simxAddStatusbarMessage(_clientId,inData->at(1).stringData[0].c_str(),inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==1)
    { // simxAuxiliaryConsoleClose
        int res=simxAuxiliaryConsoleClose(_clientId,inData->at(1).int32Data[0],inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==2)
    { // simxAuxiliaryConsoleOpen
        int* pos=NULL;
        int* size=NULL;
        float* frgb=NULL;
        float* brgb=NULL;
        if (inData->at(4).int32Data.size()>=2)
            pos=&inData->at(4).int32Data[0];
        if (inData->at(5).int32Data.size()>=2)
            size=&inData->at(5).int32Data[0];
        if (inData->at(6).floatData.size()>=3)
            frgb=&inData->at(6).floatData[0];
        if (inData->at(7).floatData.size()>=3)
            brgb=&inData->at(7).floatData[0];
        int consoleHandle;
        int res=simxAuxiliaryConsoleOpen(_clientId,inData->at(1).stringData[0].c_str(),inData->at(2).int32Data[0],inData->at(3).int32Data[0],pos,size,frgb,brgb,&consoleHandle,inData->at(8).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(consoleHandle));
    }
    if (cmdId==3)
    { // simxAuxiliaryConsolePrint
        char* txt=NULL;
        if (inData->at(2).stringData.size()==1)
            txt=(char*)inData->at(2).stringData[0].c_str();
        int res=simxAuxiliaryConsolePrint(_clientId,inData->at(1).int32Data[0],txt,inData->at(3).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==4)
    { // simxAuxiliaryConsoleShow
        int res=simxAuxiliaryConsoleShow(_clientId,inData->at(1).int32Data[0],inData->at(2).boolData[0],inData->at(3).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==5)
    { // simxBreakForceSensor
        int res=simxBreakForceSensor(_clientId,inData->at(1).int32Data[0],inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==6)
    { // simxClearFloatSignal
        int res=simxClearFloatSignal(_clientId,inData->at(1).stringData[0].c_str(),inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==7)
    { // simxClearIntegerSignal
        int res=simxClearIntegerSignal(_clientId,inData->at(1).stringData[0].c_str(),inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==8)
    { // simxClearStringSignal
        int res=simxClearStringSignal(_clientId,inData->at(1).stringData[0].c_str(),inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==9)
    { // simxGetObjectHandle
        int handle;
        int res=simxGetObjectHandle(_clientId,inData->at(1).stringData[0].c_str(),&handle,inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(handle));
    }
    if (cmdId==10)
    { // simxCloseScene
        int res=simxCloseScene(_clientId,inData->at(1).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==11)
    { // simxCopyPasteObjects
        int cnt;
        int* newHandles;
        int res=simxCopyPasteObjects(_clientId,&inData->at(1).int32Data[0],(int)inData->at(1).int32Data.size(),&newHandles,&cnt,inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
        {
            std::vector<int> nobj;
            for (int i=0;i<cnt;i++)
                nobj.push_back(newHandles[i]);
            inOutData->pushOutData(CScriptFunctionDataItem(nobj));
        }
    }
    if (cmdId==12)
    { // simxLoadScene
        std::string fileAndPath(inData->at(1).stringData[0]);
        int res=simxLoadScene(_clientId,fileAndPath.c_str(),inData->at(2).int32Data[0],inData->at(3).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==13)
    { // simxLoadModel
        std::string fileAndPath(inData->at(1).stringData[0]);
        int baseHandle;
        int res=simxLoadModel(_clientId,fileAndPath.c_str(),inData->at(2).int32Data[0],&baseHandle,inData->at(3).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(baseHandle));
    }
    if (cmdId==14)
    { // simxLoadUI
        std::string fileAndPath(inData->at(1).stringData[0]);
        int cnt;
        int* handles;
        int res=simxLoadUI(_clientId,fileAndPath.c_str(),inData->at(2).int32Data[0],&cnt,&handles,inData->at(3).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
        {
            std::vector<int> handlesR;
            for (int i=0;i<cnt;i++)
                handlesR.push_back(handles[i]);
            inOutData->pushOutData(CScriptFunctionDataItem(handlesR));
        }
    }
    if (cmdId==15)
    { // simxSetBooleanParameter
        int res=simxSetBooleanParameter(_clientId,inData->at(1).int32Data[0],inData->at(2).boolData[0],inData->at(3).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==16)
    { // simxStartSimulation
        int res=simxStartSimulation(_clientId,inData->at(1).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==17)
    { // simxStopSimulation
        int res=simxStopSimulation(_clientId,inData->at(1).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==18)
    { // simxSetFloatingParameter
        int res=simxSetFloatingParameter(_clientId,inData->at(1).int32Data[0],inData->at(2).floatData[0],inData->at(3).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==19)
    { // simxSetIntegerParameter
        int res=simxSetIntegerParameter(_clientId,inData->at(1).int32Data[0],inData->at(2).int32Data[0],inData->at(3).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==20)
    { // simxSetArrayParameter
        int res=simxSetArrayParameter(_clientId,inData->at(1).int32Data[0],&inData->at(2).floatData[0],inData->at(3).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==21)
    { // simxGetBooleanParameter
        unsigned char p;
        int res=simxGetBooleanParameter(_clientId,inData->at(1).int32Data[0],&p,inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(p!=0));
    }
    if (cmdId==22)
    { // simxGetFloatingParameter
        float p;
        int res=simxGetFloatingParameter(_clientId,inData->at(1).int32Data[0],&p,inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(p));
    }
    if (cmdId==23)
    { // simxGetIntegerParameter
        int p;
        int res=simxGetIntegerParameter(_clientId,inData->at(1).int32Data[0],&p,inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(p));
    }
    if (cmdId==24)
    { // simxGetArrayParameter
        std::vector<float> p(3,0.0f);
        int res=simxGetArrayParameter(_clientId,inData->at(1).int32Data[0],&p[0],inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(p));
    }

    if (cmdId==25)
    { // simxGetStringParameter
        char* p;
        int res=simxGetStringParameter(_clientId,inData->at(1).int32Data[0],&p,inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(std::string(p)));
    }
    if (cmdId==26)
    { // simxSetFloatSignal
        int res=simxSetFloatSignal(_clientId,inData->at(1).stringData[0].c_str(),inData->at(2).floatData[0],inData->at(3).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==27)
    { // simxSetIntegerSignal
        int res=simxSetIntegerSignal(_clientId,inData->at(1).stringData[0].c_str(),inData->at(2).int32Data[0],inData->at(3).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==28)
    { // simxSetStringSignal
        int res=simxSetStringSignal(_clientId,inData->at(1).stringData[0].c_str(),(unsigned char*)inData->at(2).stringData[0].c_str(),(int)inData->at(2).stringData[0].size(),inData->at(3).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==29)
    { // simxGetFloatSignal
        float signalValue;
        int res=simxGetFloatSignal(_clientId,inData->at(1).stringData[0].c_str(),&signalValue,inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(signalValue));
    }
    if (cmdId==30)
    { // simxGetIntegerSignal
        int signalValue;
        int res=simxGetIntegerSignal(_clientId,inData->at(1).stringData[0].c_str(),&signalValue,inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(signalValue));
    }
    if (cmdId==31)
    { // simxGetStringSignal
        int signalLength;
        unsigned char* signalValue;
        int res=simxGetStringSignal(_clientId,inData->at(1).stringData[0].c_str(),&signalValue,&signalLength,inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem((char*)signalValue,signalLength));
    }
    if (cmdId==32)
    { // simxCreateDummy
        int dummyHandle;
        unsigned char* cols=NULL;
        unsigned char colors[12];
        if (inData->at(2).int32Data.size()>=12)
        {
            for (int i=0;i<12;i++)
                colors[i]=(unsigned char)inData->at(2).int32Data[i];
            cols=colors;
        }
        int res=simxCreateDummy(_clientId,inData->at(1).floatData[0],cols,&dummyHandle,inData->at(3).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(dummyHandle));
    }
    if (cmdId==33)
    { // simxDisplayDialog
        int dlgHandle;
        int uiHandle;
        float* tcols=NULL;
        float tcolors[6];
        float* dcols=NULL;
        float dcolors[6];
        if (inData->at(5).floatData.size()>=6)
        {
            for (int i=0;i<6;i++)
                tcolors[i]=inData->at(5).floatData[i];
            tcols=tcolors;
        }
        if (inData->at(6).floatData.size()>=6)
        {
            for (int i=0;i<6;i++)
                dcolors[i]=inData->at(6).floatData[i];
            dcols=dcolors;
        }
        int res=simxDisplayDialog(_clientId,inData->at(1).stringData[0].c_str(),inData->at(2).stringData[0].c_str(),inData->at(3).int32Data[0],inData->at(4).stringData[0].c_str(),tcols,dcols,&dlgHandle,&uiHandle,inData->at(7).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
        {
            inOutData->pushOutData(CScriptFunctionDataItem(dlgHandle));
            inOutData->pushOutData(CScriptFunctionDataItem(uiHandle));
        }
    }
    if (cmdId==34)
    { // simxEndDialog
        int res=simxEndDialog(_clientId,inData->at(1).int32Data[0],inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==35)
    { // simxGetDialogInput
        char* input;
        int res=simxGetDialogInput(_clientId,inData->at(1).int32Data[0],&input,inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(std::string(input)));
    }
    if (cmdId==36)
    { // simxGetDialogResult
        int result;
        int res=simxGetDialogResult(_clientId,inData->at(1).int32Data[0],&result,inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(result));
    }
    if (cmdId==37)
    { // simxEraseFile
        int res=simxEraseFile(_clientId,inData->at(1).stringData[0].c_str(),inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==38)
    { // simxGetCollisionHandle
        int handle;
        int res=simxGetCollisionHandle(_clientId,inData->at(1).stringData[0].c_str(),&handle,inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(handle));
    }
    if (cmdId==39)
    { // simxGetConnectionId
        int res=simxGetConnectionId(_clientId);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==40)
    { // simxGetDistanceHandle
        int handle;
        int res=simxGetDistanceHandle(_clientId,inData->at(1).stringData[0].c_str(),&handle,inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(handle));
    }
    if (cmdId==41)
    { // simxGetInMessageInfo
        int info;
        int res=simxGetInMessageInfo(_clientId,inData->at(1).int32Data[0],&info);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res!=-1)
            inOutData->pushOutData(CScriptFunctionDataItem(info));
    }
    if (cmdId==42)
    { // simxGetOutMessageInfo
        int info;
        int res=simxGetOutMessageInfo(_clientId,inData->at(1).int32Data[0],&info);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res!=-1)
            inOutData->pushOutData(CScriptFunctionDataItem(info));
    }
    if (cmdId==43)
    { // simxGetJointForce
        float force;
        int res=simxGetJointForce(_clientId,inData->at(1).int32Data[0],&force,inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(force));
    }
    if (cmdId==44)
    { // simxGetJointMatrix
        std::vector<float> matrix(12,0.0f);
        int res=simxGetJointMatrix(_clientId,inData->at(1).int32Data[0],&matrix[0],inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(matrix));
    }
    if (cmdId==45)
    { // simxGetJointPosition
        float position;
        int res=simxGetJointPosition(_clientId,inData->at(1).int32Data[0],&position,inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(position));
    }
    if (cmdId==46)
    { // simxGetLastCmdTime
        int timInMs=simxGetLastCmdTime(_clientId);
        inOutData->pushOutData(CScriptFunctionDataItem(timInMs));
    }
    if (cmdId==47)
    { // simxGetLastErrors
        int errorCnt;
        char* errorStrings;
        int res=simxGetLastErrors(_clientId,&errorCnt,&errorStrings,inData->at(1).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
        {
            std::vector<std::string> errs;
            int off=0;
            for (int i=0;i<errorCnt;i++)
            {
                errs.push_back(std::string(errorStrings+off));
                off+=int(strlen(errorStrings+off))+1;
            }
            inOutData->pushOutData(CScriptFunctionDataItem(errs));
        }
    }
    if (cmdId==48)
    { // simxGetModelProperty
        int prop;
        int res=simxGetModelProperty(_clientId,inData->at(1).int32Data[0],&prop,inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(prop));
    }
    if (cmdId==49)
    { // simxGetObjectChild
        int child;
        int res=simxGetObjectChild(_clientId,inData->at(1).int32Data[0],inData->at(2).int32Data[0],&child,inData->at(3).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(child));
    }
    if (cmdId==50)
    { // simxGetObjectFloatParameter
        float param;
        int res=simxGetObjectFloatParameter(_clientId,inData->at(1).int32Data[0],inData->at(2).int32Data[0],&param,inData->at(3).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(param));
    }
    if (cmdId==51)
    { // simxGetObjectIntParameter
        int param;
        int res=simxGetObjectIntParameter(_clientId,inData->at(1).int32Data[0],inData->at(2).int32Data[0],&param,inData->at(3).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(param));
    }
    if (cmdId==52)
    { // simxGetObjectGroupData
        int handlesCount;
        int* handles;
        int intDataCount;
        int* intData;
        int floatDataCount;
        float* floatData;
        int stringDataCount;
        char* stringData;
        int res=simxGetObjectGroupData(_clientId,inData->at(1).int32Data[0],inData->at(2).int32Data[0],&handlesCount,&handles,&intDataCount,&intData,&floatDataCount,&floatData,&stringDataCount,&stringData,inData->at(3).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
        {
            std::vector<int> _handles;
            for (int i=0;i<handlesCount;i++)
                _handles.push_back(handles[i]);
            inOutData->pushOutData(CScriptFunctionDataItem(_handles));

            std::vector<int> _intData;
            for (int i=0;i<intDataCount;i++)
                _intData.push_back(intData[i]);
            inOutData->pushOutData(CScriptFunctionDataItem(_intData));

            std::vector<float> _floatData;
            for (int i=0;i<floatDataCount;i++)
                _floatData.push_back(floatData[i]);
            inOutData->pushOutData(CScriptFunctionDataItem(_floatData));

            std::vector<std::string> str;
            int off=0;
            for (int i=0;i<stringDataCount;i++)
            {
                str.push_back(std::string(stringData+off));
                off+=int(strlen(stringData+off))+1;
            }
            inOutData->pushOutData(CScriptFunctionDataItem(str));
        }
    }
    if (cmdId==53)
    { // simxGetObjectOrientation
        std::vector<float> euler(3,0.0f);
        int res=simxGetObjectOrientation(_clientId,inData->at(1).int32Data[0],inData->at(2).int32Data[0],&euler[0],inData->at(3).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(euler));
    }
    if (cmdId==54)
    { // simxGetObjectPosition
        std::vector<float> pos(3,0.0f);
        int res=simxGetObjectPosition(_clientId,inData->at(1).int32Data[0],inData->at(2).int32Data[0],&pos[0],inData->at(3).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(pos));
    }
    if (cmdId==55)
    { // simxGetObjectParent
        int parent;
        int res=simxGetObjectParent(_clientId,inData->at(1).int32Data[0],&parent,inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(parent));
    }
    if (cmdId==56)
    { // simxGetObjects
        int handlesCount;
        int* handles;
        int res=simxGetObjects(_clientId,inData->at(1).int32Data[0],&handlesCount,&handles,inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
        {
            std::vector<int> _handles;
            for (int i=0;i<handlesCount;i++)
                _handles.push_back(handles[i]);
            inOutData->pushOutData(CScriptFunctionDataItem(_handles));
        }
    }
    if (cmdId==57)
    { // simxGetObjectSelection
        int handlesCount;
        int* handles;
        int res=simxGetObjectSelection(_clientId,&handles,&handlesCount,inData->at(1).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
        {
            std::vector<int> _handles;
            for (int i=0;i<handlesCount;i++)
                _handles.push_back(handles[i]);
            inOutData->pushOutData(CScriptFunctionDataItem(_handles));
        }
    }
    if (cmdId==58)
    { // simxGetObjectVelocity
        std::vector<float> lVel(3,0.0f);
        std::vector<float> aVel(3,0.0f);
        int res=simxGetObjectVelocity(_clientId,inData->at(1).int32Data[0],&lVel[0],&aVel[0],inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
        {
            inOutData->pushOutData(CScriptFunctionDataItem(lVel));
            inOutData->pushOutData(CScriptFunctionDataItem(aVel));
        }
    }
    if (cmdId==59)
    { // simxGetPingTime
        int ptime;
        int res=simxGetPingTime(_clientId,&ptime);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(ptime));
    }
    if (cmdId==60)
    { // simxGetUIButtonProperty
        int prop;
        int res=simxGetUIButtonProperty(_clientId,inData->at(1).int32Data[0],inData->at(2).int32Data[0],&prop,inData->at(3).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(prop));
    }
    if (cmdId==61)
    { // simxGetUIEventButton
        int butt;
        std::vector<int> auxVals(2,0);
        int res=simxGetUIEventButton(_clientId,inData->at(1).int32Data[0],&butt,&auxVals[0],inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
        {
            inOutData->pushOutData(CScriptFunctionDataItem(butt));
            inOutData->pushOutData(CScriptFunctionDataItem(auxVals));
        }
    }
    if (cmdId==62)
    { // simxGetUIHandle
        int handle;
        int res=simxGetUIHandle(_clientId,inData->at(1).stringData[0].c_str(),&handle,inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(handle));
    }
    if (cmdId==63)
    { // simxGetUISlider
        int pos;
        int res=simxGetUISlider(_clientId,inData->at(1).int32Data[0],inData->at(2).int32Data[0],&pos,inData->at(3).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(pos));
    }
    if (cmdId==64)
    { // simxGetVisionSensorDepthBuffer
        std::vector<int> resolution(2,0);
        float* buffer;
        int res=simxGetVisionSensorDepthBuffer(_clientId,inData->at(1).int32Data[0],&resolution[0],&buffer,inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
        {
            inOutData->pushOutData(CScriptFunctionDataItem(std::vector<float>(buffer,buffer+resolution[0]*resolution[1])));
            inOutData->pushOutData(CScriptFunctionDataItem(resolution));
        }
    }
    if (cmdId==65)
    { // simxGetVisionSensorImage
        std::vector<int> resolution(2,0);
        unsigned char* img;
        int res=simxGetVisionSensorImage(_clientId,inData->at(1).int32Data[0],&resolution[0],&img,inData->at(2).int32Data[0],inData->at(3).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
        {
            int byteNb=3;
            if (inData->at(2).int32Data[0]&1)
                byteNb=1;
            inOutData->pushOutData(CScriptFunctionDataItem((char*)img,resolution[0]*resolution[1]*byteNb));
            inOutData->pushOutData(CScriptFunctionDataItem(resolution));
        }
    }
    if (cmdId==66)
    { // simxSetVisionSensorImage
        int options=inData->at(3).int32Data[0];
        int res=simxSetVisionSensorImage(_clientId,inData->at(1).int32Data[0],(unsigned char*)&inData->at(2).stringData[0][0],(int)inData->at(2).stringData[0].size(),options,inData->at(4).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==67)
    { // simxPauseCommunication
        int res=simxPauseCommunication(_clientId,inData->at(1).boolData[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==68)
    { // simxPauseSimulation
        int res=simxPauseSimulation(_clientId,inData->at(1).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==69)
    { // simxQuery
        unsigned char* retSignalValue;
        int retSignalLength;
        int res=simxQuery(_clientId,inData->at(1).stringData[0].c_str(),(unsigned char*)inData->at(2).stringData[0].c_str(),(int)inData->at(2).stringData[0].size(),inData->at(3).stringData[0].c_str(),&retSignalValue,&retSignalLength,inData->at(4).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem((char*)retSignalValue,retSignalLength));
    }
    if (cmdId==70)
    { // simxReadCollision
        unsigned char collState;
        int res=simxReadCollision(_clientId,inData->at(1).int32Data[0],&collState,inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(collState!=0));
    }
    if (cmdId==71)
    { // simxReadDistance
        float dist;
        int res=simxReadDistance(_clientId,inData->at(1).int32Data[0],&dist,inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(dist));
    }
    if (cmdId==72)
    { // simxReadForceSensor
        unsigned char state;
        std::vector<float> force(3,0.0f);
        std::vector<float> torque(3,0.0f);
        int res=simxReadForceSensor(_clientId,inData->at(1).int32Data[0],&state,&force[0],&torque[0],inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
        {
            inOutData->pushOutData(CScriptFunctionDataItem(state));
            if ((state&3)==1)
            {
                inOutData->pushOutData(CScriptFunctionDataItem(force));
                inOutData->pushOutData(CScriptFunctionDataItem(torque));
            }
        }
    }
    if (cmdId==73)
    { // simxReadProximitySensor
        unsigned char state;
        std::vector<float> point(3,0.0f);
        int handle;
        std::vector<float> normal(3,0.0f);
        int res=simxReadProximitySensor(_clientId,inData->at(1).int32Data[0],&state,&point[0],&handle,&normal[0],inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
        {
            inOutData->pushOutData(CScriptFunctionDataItem(state!=0));
            if (state!=0)
            {
                inOutData->pushOutData(CScriptFunctionDataItem(point));
                inOutData->pushOutData(CScriptFunctionDataItem(handle));
                inOutData->pushOutData(CScriptFunctionDataItem(normal));
            }
        }
    }
    if (cmdId==74)
    { // simxReadStringStream
        unsigned char* signalValue;
        int signalLength;
        int res=simxReadStringStream(_clientId,inData->at(1).stringData[0].c_str(),&signalValue,&signalLength,inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem((char*)signalValue,signalLength));
    }
    if (cmdId==75)
    { // simxWriteStringStream
        int res=simxWriteStringStream(_clientId,inData->at(1).stringData[0].c_str(),(unsigned char*)inData->at(2).stringData[0].c_str(),(int)inData->at(2).stringData[0].size(),inData->at(3).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==76)
    { // simxReadVisionSensor
        unsigned char detectionState;
        float* auxValues;
        int* auxValuesCount;
        int res=simxReadVisionSensor(_clientId,inData->at(1).int32Data[0],&detectionState,&auxValues,&auxValuesCount,inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
        {
            inOutData->pushOutData(CScriptFunctionDataItem(detectionState!=0));
            int packets=auxValuesCount[0];
            int off=0;
            for (int i=0;i<packets;i++)
            {
                std::vector<float> packet;
                for (int j=0;j<auxValuesCount[1+i];j++)
                    packet.push_back(auxValues[off++]);
                inOutData->pushOutData(CScriptFunctionDataItem(packet));
            }
        }
    }
    if (cmdId==77)
    { // simxRemoveModel
        int res=simxRemoveModel(_clientId,inData->at(1).int32Data[0],inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==78)
    { // simxRemoveObject
        int res=simxRemoveObject(_clientId,inData->at(1).int32Data[0],inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==79)
    { // simxRemoveUI
        int res=simxRemoveUI(_clientId,inData->at(1).int32Data[0],inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==80)
    { // simxSetJointForce
        int res=simxSetJointForce(_clientId,inData->at(1).int32Data[0],inData->at(2).floatData[0],inData->at(3).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==81)
    { // simxSetJointPosition
        int res=simxSetJointPosition(_clientId,inData->at(1).int32Data[0],inData->at(2).floatData[0],inData->at(3).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==82)
    { // simxSetJointTargetPosition
        int res=simxSetJointTargetPosition(_clientId,inData->at(1).int32Data[0],inData->at(2).floatData[0],inData->at(3).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==83)
    { // simxSetJointTargetVelocity
        int res=simxSetJointTargetVelocity(_clientId,inData->at(1).int32Data[0],inData->at(2).floatData[0],inData->at(3).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==84)
    { // simxSetModelProperty
        int res=simxSetModelProperty(_clientId,inData->at(1).int32Data[0],inData->at(2).int32Data[0],inData->at(3).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==85)
    { // simxSetObjectOrientation
        int res=simxSetObjectOrientation(_clientId,inData->at(1).int32Data[0],inData->at(2).int32Data[0],&inData->at(3).floatData[0],inData->at(4).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==86)
    { // simxSetObjectPosition
        int res=simxSetObjectPosition(_clientId,inData->at(1).int32Data[0],inData->at(2).int32Data[0],&inData->at(3).floatData[0],inData->at(4).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==87)
    { // simxSetObjectParent
        int res=simxSetObjectParent(_clientId,inData->at(1).int32Data[0],inData->at(2).int32Data[0],inData->at(3).boolData[0],inData->at(4).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==88)
    { // simxSetObjectSelection
        int cnt=(int)inData->at(1).int32Data.size();
        int* handlesPtr=NULL;
        if (cnt>0)
            handlesPtr=&inData->at(1).int32Data[0];
        int res=simxSetObjectSelection(_clientId,handlesPtr,cnt,inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==89)
    { // simxSetSphericalJointMatrix
        int res=simxSetSphericalJointMatrix(_clientId,inData->at(1).int32Data[0],&inData->at(2).floatData[0],inData->at(3).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==90)
    { // simxSetUIButtonLabel
        int res=simxSetUIButtonLabel(_clientId,inData->at(1).int32Data[0],inData->at(2).int32Data[0],inData->at(3).stringData[0].c_str(),inData->at(4).stringData[0].c_str(),inData->at(5).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==91)
    { // simxSetUIButtonProperty
        int res=simxSetUIButtonProperty(_clientId,inData->at(1).int32Data[0],inData->at(2).int32Data[0],inData->at(3).int32Data[0],inData->at(4).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==92)
    { // simxSetUISlider
        int res=simxSetUISlider(_clientId,inData->at(1).int32Data[0],inData->at(2).int32Data[0],inData->at(3).int32Data[0],inData->at(4).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==93)
    { // simxSynchronous
        int res=simxSynchronous(_clientId,inData->at(1).boolData[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==94)
    { // simxSynchronousTrigger
        int res=simxSynchronousTrigger(_clientId);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==95)
    { // simxTransferFile
        int res=simxTransferFile(_clientId,inData->at(1).stringData[0].c_str(),inData->at(2).stringData[0].c_str(),inData->at(3).int32Data[0],inData->at(4).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
    }
    if (cmdId==96)
    { // simxGetCollectionHandle
        int handle;
        int res=simxGetCollectionHandle(_clientId,inData->at(1).stringData[0].c_str(),&handle,inData->at(2).int32Data[0]);
        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
            inOutData->pushOutData(CScriptFunctionDataItem(handle));
    }
    if (cmdId==97)
    { // simxCallScriptFunction
        int* _inInt=NULL;
        int _inIntS=(int)inData->at(4).int32Data.size();
        if (_inIntS>0)
            _inInt=&inData->at(4).int32Data[0];

        float* _inFloat=NULL;
        int _inFloatS=(int)inData->at(5).floatData.size();
        if (_inFloatS>0)
            _inFloat=&inData->at(5).floatData[0];

        int _inStringS=(int)inData->at(6).stringData.size();
        std::string _inString;
        for (size_t i=0;i<inData->at(6).stringData.size();i++)
        {
            _inString+=std::string(inData->at(6).stringData[i].c_str());
            _inString+='\0';
        }
        
        int outIntS;
        int* outInt;
        int outFloatS;
        float* outFloat;
        int outStringS;
        char* outString;
        int outBuffS;
        unsigned char* outBuff;

        int res=simxCallScriptFunction(_clientId,inData->at(1).stringData[0].c_str(),inData->at(2).int32Data[0],
                inData->at(3).stringData[0].c_str(),_inIntS,_inInt,_inFloatS,_inFloat,_inStringS,_inString.c_str(),
                (int)inData->at(7).stringData[0].size(),(simxUChar*)inData->at(7).stringData[0].c_str(),
                &outIntS,&outInt,&outFloatS,&outFloat,&outStringS,&outString,&outBuffS,&outBuff,inData->at(8).int32Data[0]);

        inOutData->pushOutData(CScriptFunctionDataItem(res));
        if (res==0)
        {
            std::vector<int> oi(outInt,outInt+outIntS);
            inOutData->pushOutData(CScriptFunctionDataItem(oi));
            std::vector<float> of(outFloat,outFloat+outFloatS);
            inOutData->pushOutData(CScriptFunctionDataItem(of));
            size_t off=0;
            std::vector<std::string> st;
            for (int i=0;i<outStringS;i++)
            {
                st.push_back(std::string(outString+off));
                off+=strlen(outString+off)+1;
            }
            inOutData->pushOutData(CScriptFunctionDataItem(st));
            inOutData->pushOutData(CScriptFunctionDataItem((char*)outBuff,outBuffS));
        }
    }
}
