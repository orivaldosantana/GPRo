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

#pragma once

#include <vector>
#include "scriptFunctionData.h"

#ifdef _WIN32
    #include <Windows.h>
    #ifndef QT_COMPIL
        #pragma message("-----------------------> Adding library: Winmm.lib")
        #pragma comment(lib,"Winmm.lib")
    #endif
#endif /* _WIN32 */

#if defined (__linux) || defined (__APPLE__)
    #include <pthread.h>

    #include <sys/types.h>
    #include <sys/socket.h>
    #include <sys/stat.h>
    #include <sys/time.h>
    #include <sys/un.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <fcntl.h>
    #include <unistd.h>
#endif /* __linux || __APPLE__ */

class CRemoteApiLink
{
public:
    CRemoteApiLink(const char* ip,int portNb,bool waitUntilConnected,bool doNotReconnect,int timeOut,int cycleTime);
    virtual ~CRemoteApiLink();

    void start();
    int getPortNb();
    int getClientId();
    void setDestroyAtSimulationEnd(bool destroy);
    bool getDestroyAtSimulationEnd();

    void handleCommand(int cmdId,CScriptFunctionData* inOutData,bool blocking,bool threaded);


    static unsigned int getTimeInMs();
    static unsigned int getTimeDiffInMs(unsigned int lastTime);
    static void waitXms(unsigned int xms);

protected:
    void* _run();
    void _lock();
    void _unlock();

    void _stop();

    void _handleCommand(int cmdId,CScriptFunctionData* inOutData);

    volatile bool _threadLaunched;
    volatile bool _threadEnded;

    std::string _ipAddress;
    int _portNb;
    bool _waitUntilConnected;
    bool _doNotReconnect;
    int _timeOut;
    int _cycleTime;
    int _clientId;
    bool _destroyAtSimulationEnd;

    std::vector<int> _commandIds;
    std::vector<CScriptFunctionData*> _commandsToHandle; // NULL represents free locations
    std::vector<int*> _executionStatus; // 0=not handled yes, 1=executed


//  int _lockLevel;
#ifdef _WIN32
    static DWORD WINAPI _staticThreadProc(LPVOID arg);
    #define MUTEX_HANDLE HANDLE
    #define MUTEX_HANDLE_X MUTEX_HANDLE
    #define THREAD_ID DWORD
#endif /* _WIN32 */
#if defined (__linux) || defined (__APPLE__)
    static void* _staticThreadProc(void *arg);
    #define MUTEX_HANDLE pthread_mutex_t
    #define MUTEX_HANDLE_X MUTEX_HANDLE*
    #define THREAD_ID pthread_t
    pthread_t _theThread;
#endif /* __linux || __APPLE__ */

    void _simpleLock(MUTEX_HANDLE_X mutex);
    void _simpleUnlock(MUTEX_HANDLE_X mutex);

    MUTEX_HANDLE _mutex;
//  MUTEX_HANDLE _mutexAux;
//  THREAD_ID _lockThreadId;
};
