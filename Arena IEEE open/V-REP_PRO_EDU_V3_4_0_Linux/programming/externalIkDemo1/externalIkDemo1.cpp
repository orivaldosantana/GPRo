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
#include <string.h>

extern "C" {
    #include "extApi.h"
}

// For double-precision, define EXTIK_DOUBLE in the project settings

void prepareShapeNames(std::vector<std::string>& shapeNames)
{   // Prepare a vector with all the shape names in the robot (except for the fixed shape, i.e. the base shape)).
    // The names in the external IK file and the names in the V-REP scene should be the same:
    shapeNames.clear();
    shapeNames.push_back("irb360_arm");
    shapeNames.push_back("irb360_arm0");
    shapeNames.push_back("irb360_arm1");
    shapeNames.push_back("irb360_axisL");
    shapeNames.push_back("irb360_axisL0");
    shapeNames.push_back("irb360_axisL1");
    shapeNames.push_back("irb360_axisL2");
    shapeNames.push_back("irb360_bridgeB");
    shapeNames.push_back("irb360_bridgeB0");
    shapeNames.push_back("irb360_bridgeB1");
    shapeNames.push_back("irb360_bridgeT");
    shapeNames.push_back("irb360_bridgeT0");
    shapeNames.push_back("irb360_bridgeT1");
    shapeNames.push_back("irb360_linkL");
    shapeNames.push_back("irb360_linkL0");
    shapeNames.push_back("irb360_linkL1");
    shapeNames.push_back("irb360_linkL2");
    shapeNames.push_back("irb360_linkL3");
    shapeNames.push_back("irb360_linkL4");
    shapeNames.push_back("irb360_platform");
}

int getVrepHandles(int clientID,const std::vector<std::string>& shapeNames,std::vector<int>& vrepShapeHandles,const char* baseName)
{ // Here we retrieve info from the V-REP scene: the handles of the shape named in 'shapeNames', and the baseObjectHandle:
    vrepShapeHandles.clear();

    // 1. We retrieve the handles and names of all shapes in the current V-REP scene:
    int _allVrepShapeCnt,_tmp;
    int* _allVrepShapeHandlesTmp;
    char* _allVrepShapeNamesTmp;
    simxGetObjectGroupData(clientID,sim_object_shape_type,0,&_allVrepShapeCnt,&_allVrepShapeHandlesTmp,NULL,NULL,NULL,NULL,&_tmp,&_allVrepShapeNamesTmp,simx_opmode_blocking);

    // 2.a. We put all V-REP shape handles and names into 2 corresponding vectors, for easier parsing:
    std::vector<int> _allVrepShapeHandles_forParsing;
    std::vector<std::string> _allVrepShapeNames_forParsing;
    // 2.b. We fill the two vectors:
    size_t off=0;
    for (int i=0;i<_allVrepShapeCnt;i++)
    {
        _allVrepShapeNames_forParsing.push_back(_allVrepShapeNamesTmp+off);
        _allVrepShapeHandles_forParsing.push_back(_allVrepShapeHandlesTmp[i]);
        off+=strlen(_allVrepShapeNamesTmp+off)+1;
    }

    // 3. We now go through all robot shapes and insert the corresponding V-REP object handle:
    for (size_t i=0;i<shapeNames.size();i++)
    {
        int handle=-1;
        for (size_t j=0;j<_allVrepShapeNames_forParsing.size();j++)
        {
            if (_allVrepShapeNames_forParsing[j].compare(shapeNames[i])==0)
            {
                handle=_allVrepShapeHandles_forParsing[j];
                break;
            }
        }
        if (handle!=-1)
            vrepShapeHandles.push_back(handle);
    }

    // Now get the handle of the robot's base:
    int baseHandle;
    simxGetObjectHandle(clientID,baseName,&baseHandle,simx_opmode_blocking);

    return(baseHandle);
}

void switchToFk(const int* embMotorHandles,int embMainIkGroup,int embTip)
{   // Here we switch the external IK robot to FK mode.

    // 1. We want the robot motors NOT to be part of IK resolution:
    simEmbSetJointMode(embMotorHandles[0],sim_jointmode_passive);
    simEmbSetJointMode(embMotorHandles[1],sim_jointmode_passive);
    simEmbSetJointMode(embMotorHandles[2],sim_jointmode_passive);

    // 2. We disable the IK element that handles the position of the tip (since we are in FK mode):
    simEmbSetIkElementProperties(embMainIkGroup,embTip,0,NULL,NULL); // i.e. no constraints for that element
}

void switchToIk(const int* embMotorHandles,int embMainIkGroup,int embTip,int embTarget)
{   // Here we switch the external IK robot to IK mode.

    // 1. We want the robot motors to be part of IK resolution:
    simEmbSetJointMode(embMotorHandles[0],sim_jointmode_ik);
    simEmbSetJointMode(embMotorHandles[1],sim_jointmode_ik);
    simEmbSetJointMode(embMotorHandles[2],sim_jointmode_ik);

    // 2. We enable the IK element that handles the position of the tip (since we are in IK mode):
    simEmbSetIkElementProperties(embMainIkGroup,embTip,sim_ik_x_constraint|sim_ik_y_constraint|sim_ik_z_constraint,NULL,NULL); // i.e. that element is constrained in x/y/z position

    // 3. We make sure the target is at the same position and orientation as the tip:
    extIkReal identityMatrix[12]={1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0};
    simEmbSetObjectMatrix(embTarget,embTip,identityMatrix);
}

void readShapeMatricesFromExtIkAndApplyToVrep(int clientID,const std::vector<int>& embShapeHandles,int embBaseHandle,const std::vector<int>& vrepShapeHandles,int vrepBaseHandle,const std::vector<extIkReal>& corrMatrices)
{
    simxPauseCommunication(clientID,1); // we temporarily pause the communication with V-REP, so that all positions/orientations are set at the same time on the V-REP side

    for (size_t i=0;i<embShapeHandles.size();i++)
    { // we skip the robot base. But we read/write everything relative to the robot base!
        extIkReal embMatrix[12];
        simEmbGetObjectMatrix(embShapeHandles[i],embBaseHandle,embMatrix); // we get the matrix of the ext. ik
        extIkReal vrepMatrix[12];
        simEmbMultiplyMatrices(embMatrix,&corrMatrices[0]+12*i,vrepMatrix); // we correct it
        extIkReal quat[4];
        extIkReal pos[3];
        extIkReal euler[3];
        simEmbMatrixToTransformation(vrepMatrix,pos,quat);
        simEmbQuaternionToEulerAngles(quat,euler);
        float posf[3];
        float eulerf[3];
        for (size_t j=0;j<3;j++)
        { // the external IK can use double precision!
            posf[j]=(float)pos[j];
            eulerf[j]=(float)euler[j];
        }
        simxSetObjectPosition(clientID,vrepShapeHandles[i],vrepBaseHandle,posf,simx_opmode_oneshot);                
        simxSetObjectOrientation(clientID,vrepShapeHandles[i],vrepBaseHandle,eulerf,simx_opmode_oneshot);   
    }
    simxPauseCommunication(clientID,0);
}

void getCorrectionMatrices(int clientID,const std::vector<int>& embShapeHandles,int embBaseHandle,const std::vector<int>& vrepShapeHandles,int vrepBaseHandle,std::vector<extIkReal>& corrMatrices)
{ // Here we compute: corrMatrix=inv(embMatrix)*vrepMatrix. This is the correction matrix.
    // Later, when we set the matrix to V-REP, we simply need to multiply the matrix from the external IK with that correction
    // There is a correction matrix (i.e. 12 values) for each shape.
    corrMatrices.clear();
    for (size_t i=0;i<embShapeHandles.size();i++)
    {
        extIkReal embMatrix[12];
        simEmbGetObjectMatrix(embShapeHandles[i],embBaseHandle,embMatrix);
        extIkReal vrepMatrix[12];
        float vrepPosf[3];
        float vrepEulerf[3];
        simxGetObjectPosition(clientID,vrepShapeHandles[i],vrepBaseHandle,vrepPosf,simx_opmode_blocking);
        simxGetObjectOrientation(clientID,vrepShapeHandles[i],vrepBaseHandle,vrepEulerf,simx_opmode_blocking);
        extIkReal vrepPos[3];
        extIkReal vrepEuler[3];
        for (size_t j=0;j<3;j++)
        { // the external IK can use double precision!
            vrepPos[j]=(extIkReal)vrepPosf[j];
            vrepEuler[j]=(extIkReal)vrepEulerf[j];
        }       
        extIkReal vrepQuat[4];
        simEmbEulerAnglesToQuaternion(vrepEuler,vrepQuat);
        simEmbTransformationToMatrix(vrepPos,vrepQuat,vrepMatrix);
        extIkReal corrMatrix[12];
        simEmbInvertMatrix(embMatrix);
        simEmbMultiplyMatrices(embMatrix,vrepMatrix,corrMatrix);
        for (size_t j=0;j<12;j++)
            corrMatrices.push_back(corrMatrix[j]);
    }
}

int main(int argc, char* argv[])
{
    // Retrieve the command line argument that represents the port where the remote API should connect:
    int portNb=0;
    if (argc>=2)
        portNb=atoi(argv[1]);
    else
    {
        printf("Indicate following arguments: 'portNumber'!\n");
        extApi_sleepMs(5000);
        return 0;
    }

    // Prepare a vector with all the shape names in the robot (except for the fixed shape in the model).
    // The names in the external IK file and the names in the V-REP scene should be the same:
    std::vector<std::string> shapeNames;
    prepareShapeNames(shapeNames);

    // Read the IK file for that robot (the names should be the same as in the V-REP scene used to visualize the robot movement)
    FILE *file;
    file=fopen("irb360s.ik","rb");
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
        printf("The kinematic content file 'irb360s.ik' could not be read!\n");
        extApi_sleepMs(5000);
        return 0;
    }

    // Initialize the embedded robot model in the external IK:
    simEmbLaunch();
    simEmbStart(data,dataLength);
    delete[] data;

    // Now retrieve a few handles FROM THE EXTERNAL IK FILE:
    int embMotorHandles[4]; // the robot's motor handles
    embMotorHandles[0]=simEmbGetObjectHandle("irb360_drivingJoint1"); // the motor of arm1
    embMotorHandles[1]=simEmbGetObjectHandle("irb360_drivingJoint2"); // the motor of arm2
    embMotorHandles[2]=simEmbGetObjectHandle("irb360_drivingJoint3"); // the motor of arm3
    embMotorHandles[3]=simEmbGetObjectHandle("irb360_motor"); // the central motor (i.e. for the orientation)
    int embBaseHandle=simEmbGetObjectHandle("irb360_base"); // the base object relative to which we do retrieve matrices
    int embTip=simEmbGetObjectHandle("irb360_ikTip"); // the tip object
    int embTarget=simEmbGetObjectHandle("irb360_ikTarget"); // the target object
    int embMainIkGroup=simEmbGetIkGroupHandle("irb360_mainTask"); // the main IK handle. This is needed when we switch from IK to FK and vice-versa

    std::vector<int> embShapeHandles; // all the shape handles. Those are needed in order to reflect the position/orientation of the shapes of the robot in the V-REP scene
    for (size_t i=0;i<shapeNames.size();i++)
        embShapeHandles.push_back(simEmbGetObjectHandle(shapeNames[i].c_str()));

    // Now connect to V-REP:
    int clientID=simxStart("127.0.0.1",portNb,true,true,2000,5);
    if (clientID!=-1)
    { // connection was successful

        float simulationStep;
        simxGetFloatingParameter(clientID,sim_floatparam_simulation_time_step,&simulationStep,simx_opmode_streaming);

        simxSynchronous(clientID,1); // We enable the synchronous mode, so that we can trigger each simulation step from here

        // Now retrieve the shape handles FROM THE V-REP SCENE:
        std::vector<int> vrepShapeHandles; // all V-REP scene handles that correspond to the shapeNames vector
        int vrepBaseHandle=getVrepHandles(clientID,shapeNames,vrepShapeHandles,"irb360_base");

        // Now retrieve all the correction matrices that link the reference frame of a given shape in the external IK,
        // to its corresponding matrix in V-REP. Correction matrices are not needed when the shape's initially overlap.
        std::vector<extIkReal> corrMatrices;
        getCorrectionMatrices(clientID,embShapeHandles,embBaseHandle,vrepShapeHandles,vrepBaseHandle,corrMatrices);

        // Switch to FK mode:
        switchToFk(embMotorHandles,embMainIkGroup,embTip);

        extIkReal v=0.0;
        extIkReal a=0.0;
        extIkReal rot=0.0;

        bool breakOut=false;
        for (size_t kk=0;kk<3;kk++)
        {
            for (size_t fkc=0;fkc<60;fkc++)
            {

                // Set the desired motor position:
                simEmbSetJointPosition(embMotorHandles[kk],extIkReal(-45.0*3.1415*sin(float(fkc)*3.1415/60.0)/180.0));
                rot+=extIkReal(0.01);
                simEmbSetJointPosition(embMotorHandles[3],rot);

                // calculate FK:
                simEmbHandleIkGroup(sim_handle_all);

                // Now we could read the end-effector position relative to the robot base:
                // a) as pos and quaternion:
                extIkReal embTipPos[3];
                extIkReal embTipQuat[4];
                simEmbGetObjectTransformation(embTip,embBaseHandle,embTipPos,embTipQuat);
                // b) as matrix:
                extIkReal embTipMatrix[12];
                simEmbGetObjectMatrix(embTip,embBaseHandle,embTipMatrix);

                // Read and apply all shape positions:
                readShapeMatricesFromExtIkAndApplyToVrep(clientID,embShapeHandles,embBaseHandle,vrepShapeHandles,vrepBaseHandle,corrMatrices);

                // Trigger next simulation step:
                int r=simx_return_remote_error_flag; // means for next remote API function call: step not triggered
                while (r==simx_return_remote_error_flag)
                    r=simxSynchronousTrigger(clientID); // Trigger next simulation step
                if (r!=simx_return_ok)
                {
                    breakOut=true;
                    break;
                }

                printf(".");
            }
            if (breakOut)
                break;
        }

        // Switch to IK mode:
        switchToIk(embMotorHandles,embMainIkGroup,embTip,embTarget);

        extIkReal initialMatrix[12];
        simEmbGetObjectMatrix(embTip,embBaseHandle,initialMatrix); // get the matrix relative to the robot base

        while (simxGetConnectionId(clientID)!=-1)
        {
            // Following 3 commands will slow down the simulation, but garantee that if the simulation time step was changed,
            // that there won't be any jumps. Following 3 commands are not needed if you don't modify the simulation time step
            // (i.e. dt) during simulation.
            simxUChar vrepWaitingForTrigger=0;
            while ( (vrepWaitingForTrigger==0)&&(simxGetConnectionId(clientID)!=-1) )
                simxGetBooleanParameter(clientID,sim_boolparam_waiting_for_trigger,&vrepWaitingForTrigger,simx_opmode_blocking);

            simxGetFloatingParameter(clientID,sim_floatparam_simulation_time_step,&simulationStep,simx_opmode_buffer);
            v=v+extIkReal(0.08*simulationStep*20.0);
            if (v>extIkReal(10.0))
            {
                a=a+extIkReal(0.0005);
                if (a>1.0)
                    a=1.0;
            }
            rot+=extIkReal(0.01*simulationStep*20.0);

            // Set the desired tip position:
            extIkReal x=extIkReal(sin(v)*0.5);
            extIkReal y=extIkReal(cos(v)*0.5);
            extIkReal z=extIkReal(sin(v*3.0)*0.1);

            extIkReal newMatrix[12];
            for (size_t i=0;i<12;i++)
                newMatrix[i]=initialMatrix[i];
            newMatrix[3]+=extIkReal(x*a*1.1);
            newMatrix[7]+=extIkReal(y*a*1.1);
            newMatrix[11]+=extIkReal(z*(1.0-a));
            simEmbSetObjectMatrix(embTarget,embBaseHandle,newMatrix); // set the matrix relative to the robot base

            // Set the desired central motor rotation:
            simEmbSetJointPosition(embMotorHandles[3],rot);

            // calculate IK:
            simEmbHandleIkGroup(sim_handle_all);

            // Now we could read the computed motor angles:
            extIkReal embMotorAngles[3];
            simEmbGetJointPosition(embMotorHandles[0],embMotorAngles);
            simEmbGetJointPosition(embMotorHandles[1],embMotorAngles+1);
            simEmbGetJointPosition(embMotorHandles[2],embMotorAngles+2);

            // Read and apply all shape positions:
            readShapeMatricesFromExtIkAndApplyToVrep(clientID,embShapeHandles,embBaseHandle,vrepShapeHandles,vrepBaseHandle,corrMatrices);

            // Trigger next simulation step:
            int r=simx_return_remote_error_flag; // means for next remote API function call: step not triggered
            while (r==simx_return_remote_error_flag)
                r=simxSynchronousTrigger(clientID); // Trigger next simulation step
            if (r!=simx_return_ok)
                break;

            printf(".");
        }
        simxFinish(clientID);
    }
    simEmbShutDown();
    return(0);
}

