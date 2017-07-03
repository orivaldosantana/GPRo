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
    int motorHandles2[7];
    if (argc>=9+7)
    { // We get the port and the motor handles (for the visual feedback, not for IK!) via command-line arguments
        portNb=atoi(argv[1]);
        motorHandles[0]=atoi(argv[2]);
        motorHandles[1]=atoi(argv[3]);
        motorHandles[2]=atoi(argv[4]);
        motorHandles[3]=atoi(argv[5]);
        motorHandles[4]=atoi(argv[6]);
        motorHandles[5]=atoi(argv[7]);
        motorHandles[6]=atoi(argv[8]);
        motorHandles2[0]=atoi(argv[9]);
        motorHandles2[1]=atoi(argv[10]);
        motorHandles2[2]=atoi(argv[11]);
        motorHandles2[3]=atoi(argv[12]);
        motorHandles2[4]=atoi(argv[13]);
        motorHandles2[5]=atoi(argv[14]);
        motorHandles2[6]=atoi(argv[15]);
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

    // Initialize the embedded robot model1:
    int handle1=simEmbLaunch();
    simEmbStart(data,dataLength);

    // Initialize the embedded robot model2:
    int handle2=simEmbLaunch();
    simEmbStart(data,dataLength);

    delete[] data;
    simEmbSwitch(handle1); // use robot model1

    // Connect to V-REP at the above specified port, via the remote API. V-REP is just used for visual feed-back, not IK calculation!
    int clientID=simxStart("127.0.0.1",portNb,true,true,2000,5);
    if (clientID!=-1)
    {
        float simulationStep;
        simxGetFloatingParameter(clientID,sim_floatparam_simulation_time_step,&simulationStep,simx_opmode_streaming);

        simxSynchronous(clientID,1); // We enable the synchronous mode, so that we can trigger each simulation step from here

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

        // Get the initial target dummy matrix, of the embedded model:
        extIkReal matrix[12];
        simEmbGetObjectMatrix(embeddedModelTargetHandle,embeddedModelBaseHandle,matrix);

        int embeddedModelMotorHandles2[7];
        embeddedModelMotorHandles2[0]=simEmbGetObjectHandle("LBR_iiwa_7_R800_joint1");
        embeddedModelMotorHandles2[1]=simEmbGetObjectHandle("LBR_iiwa_7_R800_joint2");
        embeddedModelMotorHandles2[2]=simEmbGetObjectHandle("LBR_iiwa_7_R800_joint3");
        embeddedModelMotorHandles2[3]=simEmbGetObjectHandle("LBR_iiwa_7_R800_joint4");
        embeddedModelMotorHandles2[4]=simEmbGetObjectHandle("LBR_iiwa_7_R800_joint5");
        embeddedModelMotorHandles2[5]=simEmbGetObjectHandle("LBR_iiwa_7_R800_joint6");
        embeddedModelMotorHandles2[6]=simEmbGetObjectHandle("LBR_iiwa_7_R800_joint7");
        int embeddedModelTargetHandle2=simEmbGetObjectHandle("LBR_iiwa_7_R800_target");
        int embeddedModelBaseHandle2=simEmbGetObjectHandle("LBR_iiwa_7_R800");

        extIkReal v2=0.0;

        // Get the initial target dummy matrix, of the embedded model:
        extIkReal matrix2[12];
        simEmbGetObjectMatrix(embeddedModelTargetHandle2,embeddedModelBaseHandle2,matrix2);


        while (simxGetConnectionId(clientID)!=-1)
        {
            
            // Following 3 commands will slow down the simulation, but garantee that if the simulation time step was changed,
            // that there won't be any jumps. Following 3 commands are not needed if you don't modify the simulation time step
            // (i.e. dt) during simulation.
            simxUChar vrepWaitingForTrigger=0;
            while ( (vrepWaitingForTrigger==0)&&(simxGetConnectionId(clientID)!=-1) )
                simxGetBooleanParameter(clientID,sim_boolparam_waiting_for_trigger,&vrepWaitingForTrigger,simx_opmode_blocking);

            simxGetFloatingParameter(clientID,sim_floatparam_simulation_time_step,&simulationStep,simx_opmode_buffer);
            v+=extIkReal(0.2)*extIkReal(simulationStep);
            v2+=extIkReal(0.28)*extIkReal(simulationStep);


            simEmbSwitch(handle1); // use robot model1
            // Set the desired tip matrix by setting the target dummy matrix:
            matrix[3]=extIkReal(-0.3-cos(v)*0.1);
            matrix[7]=extIkReal(sin(v)*0.1);
            matrix[11]=extIkReal(0.629+sin(v*9)*0.01);
            simEmbSetObjectMatrix(embeddedModelTargetHandle,embeddedModelBaseHandle,matrix);

            // calculate IK:
            simEmbHandleIkGroup(sim_handle_all);

            // Read the corresponding motor angles and send them to V-REP:
            simxPauseCommunication(clientID,1); // Temporarily pause the remote API communication, in order to send all following commands at once
            extIkReal pos;
            simEmbGetJointPosition(embeddedModelMotorHandles[0],&pos);
            simxSetJointPosition(clientID,motorHandles[0],(float)pos,simx_opmode_oneshot);
            simEmbGetJointPosition(embeddedModelMotorHandles[1],&pos);
            simxSetJointPosition(clientID,motorHandles[1],(float)pos,simx_opmode_oneshot);
            simEmbGetJointPosition(embeddedModelMotorHandles[2],&pos);
            simxSetJointPosition(clientID,motorHandles[2],(float)pos,simx_opmode_oneshot);
            simEmbGetJointPosition(embeddedModelMotorHandles[3],&pos);
            simxSetJointPosition(clientID,motorHandles[3],(float)pos,simx_opmode_oneshot);
            simEmbGetJointPosition(embeddedModelMotorHandles[4],&pos);
            simxSetJointPosition(clientID,motorHandles[4],(float)pos,simx_opmode_oneshot);
            simEmbGetJointPosition(embeddedModelMotorHandles[5],&pos);
            simxSetJointPosition(clientID,motorHandles[5],(float)pos,simx_opmode_oneshot);
            simEmbGetJointPosition(embeddedModelMotorHandles[6],&pos);
            simxSetJointPosition(clientID,motorHandles[6],(float)pos,simx_opmode_oneshot);

            simEmbSwitch(handle2); // use robot model2
            // Set the desired tip matrix by setting the target dummy matrix:
            matrix2[3]=extIkReal(-0.3-cos(v2)*0.1);
            matrix2[7]=extIkReal(sin(v2)*0.1);
            simEmbSetObjectMatrix(embeddedModelTargetHandle2,embeddedModelBaseHandle2,matrix2);

            // calculate IK:
            simEmbHandleIkGroup(sim_handle_all);

            // Read the corresponding motor angles and send them to V-REP:
            simEmbGetJointPosition(embeddedModelMotorHandles2[0],&pos);
            simxSetJointPosition(clientID,motorHandles2[0],(float)pos,simx_opmode_oneshot);
            simEmbGetJointPosition(embeddedModelMotorHandles2[1],&pos);
            simxSetJointPosition(clientID,motorHandles2[1],(float)pos,simx_opmode_oneshot);
            simEmbGetJointPosition(embeddedModelMotorHandles2[2],&pos);
            simxSetJointPosition(clientID,motorHandles2[2],(float)pos,simx_opmode_oneshot);
            simEmbGetJointPosition(embeddedModelMotorHandles2[3],&pos);
            simxSetJointPosition(clientID,motorHandles2[3],(float)pos,simx_opmode_oneshot);
            simEmbGetJointPosition(embeddedModelMotorHandles2[4],&pos);
            simxSetJointPosition(clientID,motorHandles2[4],(float)pos,simx_opmode_oneshot);
            simEmbGetJointPosition(embeddedModelMotorHandles2[5],&pos);
            simxSetJointPosition(clientID,motorHandles2[5],(float)pos,simx_opmode_oneshot);
            simEmbGetJointPosition(embeddedModelMotorHandles2[6],&pos);
            simxSetJointPosition(clientID,motorHandles2[6],(float)pos,simx_opmode_oneshot);

            simxPauseCommunication(clientID,0); // Unpause the remote API communication

            // Now step the simulation on V-REP side:
            int r=simx_return_remote_error_flag; // means for next remote API function call: step not triggered
            while (r==simx_return_remote_error_flag)
                r=simxSynchronousTrigger(clientID); // Trigger next simulation step
            if (r!=simx_return_ok)
                break;

            printf(".");
        }
        simEmbShutDown(); // End the external IK
        simEmbShutDown(); // End the external IK
        simxFinish(clientID); // End the remote API
    }
    return(0);
}

