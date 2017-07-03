// This file is part of the RRS-1 interface for V-REP
// 
// Copyright 2006-2017 Coppelia Robotics GmbH. All rights reserved. 
// marc@coppeliarobotics.com
// www.coppeliarobotics.com
// 
// This interface is licensed under the terms of GNU GPL:
// 
// -------------------------------------------------------------------
// This interface is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// THIS INTERFACE IS DISTRIBUTED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
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

#include <stdio.h>
#include "socketInConnection.h"
#include <cstdlib>

#ifdef _WIN32
    #ifdef QT_COMPIL
        #include <direct.h>
    #else
        #include <Shellapi.h>
        #include <shlwapi.h>
        #pragma comment(lib, "Shlwapi.lib")
        #pragma comment(lib, "Shell32.lib")
    #endif
    #define RCS_LIBRARY HINSTANCE
#endif

#if defined (__linux) || defined (__APPLE__)
    #include <dlfcn.h>
    #include <unistd.h>
    #include <string.h>
    #define RCS_LIBRARY void*
    #define __stdcall __attribute__((stdcall))
#endif

typedef void (__stdcall *ptrRcsFunc)(void* in,void* out); // do not use __cdecl !!!

RCS_LIBRARY loadRcsModule(const char* filename)
{ // specify the rcs module's name with extension
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
    temp+="\\";
#else
    temp+="/";
#endif
    temp+=filename;

#ifdef _WIN32
    return LoadLibrary(temp.c_str());
#else
    return dlopen(temp.c_str(),RTLD_LAZY);
#endif
}

ptrRcsFunc getRcsFunction(RCS_LIBRARY lib,const char* funcName)
{
#ifdef _WIN32
    return (ptrRcsFunc)GetProcAddress(lib,funcName);
#else
    return (ptrRcsFunc)dlsym(lib,funcName);
#endif
}

void unloadRcsModule(RCS_LIBRARY lib)
{
#ifdef _WIN32
    if (lib!=0)
        FreeLibrary(lib);
#else
        dlclose(lib);
#endif
}

int main(int argc, char* argv[])
{ // We connect to the port number passed as argument
    std::string rcsPathAndFilename;
    std::string rcsFuncName;
    int portNb=0;

    if (argc>=4)
    {
        rcsPathAndFilename=argv[1];
        rcsFuncName=argv[2];
        portNb=atoi(argv[3]);
    }
    else
    {
        printf("Indicate following arguments: 'rcsPathAndFilename rcsFuncName portNumber'!\n");
#ifdef _WIN32
        Sleep(5000);
#else
        sleep(5);
#endif
        return 0;
    }

    CSocketInConnection connection(portNb);
    printf("Connecting to client...\n");
    if (connection.connectToClient())
    {
        printf("Connected to client.\n");
        printf("Loading RCS module...\n");
        RCS_LIBRARY lib=loadRcsModule(rcsPathAndFilename.c_str());
        int receivedDataLength;
        if (lib!=0)
        {
            printf("RCS module was loaded.\n");
            printf("Binding service function...\n");
            ptrRcsFunc rcsFunc=getRcsFunction(lib,rcsFuncName.c_str());
            if (rcsFunc!=0)
            {
                printf("Service function is ready.\n");
                char* receivedData=connection.receiveData(receivedDataLength);
                if (receivedData!=NULL)
                {
                    delete[] receivedData;
                    // We acknowledge that everything went fine:
                    char replyByte;
                    replyByte=2;
                    if (connection.replyToReceivedData(&replyByte,1))
                    { // ok, if we arrived here, we are set!
                        while (true)
                        { // the communication loop
                            // wait for a command:
                            receivedData=connection.receiveData(receivedDataLength);
                            if (receivedData!=NULL)
                            { // We received an RCS service call command
                                int outputBlockSize=((int*)receivedData)[1];
                                char* outputBlock=new char[outputBlockSize];
                                rcsFunc(receivedData,outputBlock);
                                delete[] receivedData;
                                int outputDataSize=((int*)outputBlock)[0];
                                if (connection.replyToReceivedData(outputBlock,outputDataSize))
                                { // ok, we could send the reply 
                                    delete[] outputBlock;
                                }
                                else
                                { // we could not send the reply
                                    delete[] outputBlock;
                                    printf("Connection error.\n");
                                    break;
                                }
                            }
                            else
                            {
                                printf("Connection error.\n");
                                break;
                            }
                        }
                    }
                    else
                        printf("Connection error.\n");
                }
                else
                {
                    printf("Connection error.\n");
                }
            }
            else
            {
                printf("Could not bind service function.\n");
                char* receivedData=connection.receiveData(receivedDataLength);
                if (receivedData!=NULL)
                {
                    delete[] receivedData;
                    // We acknowledge that there was a problem finding the function:
                    char replyByte;
                    replyByte=1;
                    connection.replyToReceivedData(&replyByte,1);
                }
                else
                    printf("Connection error.\n");
            }
            unloadRcsModule(lib);
        }
        else
        {
            printf("RCS module could not be found or loaded.\n");
            char* receivedData=connection.receiveData(receivedDataLength);
            if (receivedData!=NULL)
            {
                delete[] receivedData;
                // We acknowledge that there was a problem finding the library:
                char replyByte;
                replyByte=0;
                connection.replyToReceivedData(&replyByte,1);
            }
            else
                printf("Connection error.\n");
        }
    }
    else
        printf("Failed to connect to client.\n");
#ifdef _WIN32
    Sleep(5000);
#else
    sleep(5);
#endif
    return 0;
}

