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

#include "extIk.h"
#include <stdio.h>
#include <stdlib.h>

extern "C" {
    #include "extApi.h"
}

// For double-precision, define EXTIK_DOUBLE in the project settings

int main(int argc, char* argv[])
{
    int portNb=0;
    int motorHandles[7];
    if (argc>=9)
    { // We get the port and the motor handles (for the visual feedback, not for IK!) via command-line arguments
        portNb=atoi(argv[1]);
        motorHandles[0]=atoi(argv[2]);
        motorHandles[1]=atoi(argv[3]);
        motorHandles[2]=atoi(argv[4]);
        motorHandles[3]=atoi(argv[5]);
        motorHandles[4]=atoi(argv[6]);
        motorHandles[5]=atoi(argv[7]);
        motorHandles[6]=atoi(argv[8]);
    }
    else
    {
        printf("Indicate following arguments: 'portNumber motor1Handle motor2Handle .. motor7Handle'!\n");
        extApi_sleepMs(5000);
        return 0;
    }

    // Read the kinematic file:
    FILE *file;
    file=fopen("lbr_iiwa_7_r800.ik","rb");
    unsigned char* data=NULL;
    int dataLength=0;
    if (file)
    {
        fseek(file,0,SEEK_END);
        unsigned long fl=ftell(file);
        dataLength=(int)fl;
        fseek(file,0,SEEK_SET);
        data=new unsigned char[dataLength];
        fread((char*)data,dataLength,1,file);
        fclose(file);
    }
    else
    {
        printf("The kinematic content file 'lbr.ik' could not be read!\n");
        extApi_sleepMs(5000);
        return 0;
    }

    // Initialize the embedded robot model:
    int handle1=simEmbLaunch();
    simEmbStart(data,dataLength);

    // Connect to V-REP at the above specified port, via the remote API. V-REP is just used for visual feed-back, not IK calculation!
    int clientID=simxStart("127.0.0.1",portNb,true,true,2000,5);
    if (clientID!=-1)
    {
        float simulationStep;
        simxGetFloatingParameter(clientID,sim_floatparam_simulation_time_step,&simulationStep,simx_opmode_streaming);

//      simxSynchronous(clientID,1); // We enable the synchronous mode, so that we can trigger each simulation step from here

        int embeddedIkGroupHandle=simEmbGetIkGroupHandle("LBR_iiwa_7_R800");
        int embeddedModelMotorHandles[7];
        embeddedModelMotorHandles[0]=simEmbGetObjectHandle("LBR_iiwa_7_R800_joint1");
        embeddedModelMotorHandles[1]=simEmbGetObjectHandle("LBR_iiwa_7_R800_joint2");
        embeddedModelMotorHandles[2]=simEmbGetObjectHandle("LBR_iiwa_7_R800_joint3");
        embeddedModelMotorHandles[3]=simEmbGetObjectHandle("LBR_iiwa_7_R800_joint4");
        embeddedModelMotorHandles[4]=simEmbGetObjectHandle("LBR_iiwa_7_R800_joint5");
        embeddedModelMotorHandles[5]=simEmbGetObjectHandle("LBR_iiwa_7_R800_joint6");
        embeddedModelMotorHandles[6]=simEmbGetObjectHandle("LBR_iiwa_7_R800_joint7");
        int embeddedModelTargetHandle=simEmbGetObjectHandle("LBR_iiwa_7_R800_target");
        int embeddedModelBaseHandle=simEmbGetObjectHandle("LBR_iiwa_7_R800");

        extIkReal v=0.0;

        // Get the initial target dummy matrix, of the embedded model, and change it a bit:
        extIkReal matrix[12];
        simEmbGetObjectMatrix(embeddedModelTargetHandle,embeddedModelBaseHandle,matrix);
        matrix[3]+=extIkReal(0.2);
        matrix[7]+=extIkReal(0.3);
        matrix[11]-=extIkReal(0.2);
        simEmbSetObjectMatrix(embeddedModelTargetHandle,embeddedModelBaseHandle,matrix);

        while (simxGetConnectionId(clientID)!=-1)
        {
            // find a config for the given end-effector pose:
            extIkReal config[7];
            simEmbGetConfigForTipPose(embeddedIkGroupHandle,7,embeddedModelMotorHandles,(extIkReal)0.4,1000,config,NULL,NULL,NULL);

            // Read the corresponding motor angles and send them to V-REP:
            simxPauseCommunication(clientID,1); // Temporarily pause the remote API communication, in order to send all following commands at once
            simxSetJointPosition(clientID,motorHandles[0],(float)config[0],simx_opmode_oneshot);
            simxSetJointPosition(clientID,motorHandles[1],(float)config[1],simx_opmode_oneshot);
            simxSetJointPosition(clientID,motorHandles[2],(float)config[2],simx_opmode_oneshot);
            simxSetJointPosition(clientID,motorHandles[3],(float)config[3],simx_opmode_oneshot);
            simxSetJointPosition(clientID,motorHandles[4],(float)config[4],simx_opmode_oneshot);
            simxSetJointPosition(clientID,motorHandles[5],(float)config[5],simx_opmode_oneshot);
            simxSetJointPosition(clientID,motorHandles[6],(float)config[6],simx_opmode_oneshot);
            simxPauseCommunication(clientID,0); // Unpause the remote API communication

            printf(".");
        }
        simEmbShutDown(); // End the external IK
        simxFinish(clientID); // End the remote API
    }
    return(0);
}

