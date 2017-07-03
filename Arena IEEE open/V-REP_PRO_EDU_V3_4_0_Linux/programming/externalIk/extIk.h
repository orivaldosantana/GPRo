// This file is part of the EXTERNAL IK for V-REP
// 
// Copyright 2006-2017 Coppelia Robotics GmbH. All rights reserved. 
// marc@coppeliarobotics.com
// www.coppeliarobotics.com
// 
// The EXTERNAL IK is licensed under the terms of EITHER:
//   1. EXTERNAL IK commercial license (contact us for details)
//   2. EXTERNAL IK educational license (see below)
// 
// EXTERNAL IK educational license:
// -------------------------------------------------------------------
// The EXTERNAL IK educational license applies only to EDUCATIONAL
// ENTITIES composed by following people and institutions:
// 
// 1. Hobbyists, students, teachers and professors
// 2. Schools and universities
// 
// EDUCATIONAL ENTITIES do NOT include companies, research institutions,
// non-profit organisations, foundations, etc.
// 
// An EDUCATIONAL ENTITY may use, modify, compile and distribute the
// modified/unmodified EXTERNAL IK under following conditions:
//  
// 1. Distribution should be free of charge.
// 2. Distribution should be to EDUCATIONAL ENTITIES only.
// 3. Usage should be non-commercial.
// 4. Altered source versions must be plainly marked as such and distributed
//    along with any compiled code.
// 5. The origin of the EXTERNAL IK must not be misrepresented. you must
//    not claim that you wrote the original software.
// 
// THE EXTERNAL IK IS DISTRIBUTED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
// WARRANTY. THE USER WILL USE IT AT HIS/HER OWN RISK. THE ORIGINAL
// AUTHORS AND COPPELIA ROBOTICS GMBH WILL NOT BE LIABLE FOR DATA LOSS,
// DAMAGES, LOSS OF PROFITS OR ANY OTHER KIND OF LOSS WHILE USING OR
// MISUSING THIS SOFTWARE.
// -------------------------------------------------------------------
//
// This file was automatically created for V-REP release V3.4.0 rev. 1 on April 5th 2017

#pragma once

#include <math.h>
#include <vector>
#include <string>
#include <float.h>
#include <limits>
#include "mathDefines.h"

#define VREP_INLINE inline
#define SIM_ASSERT(x)
#define SIM_IS_BIT_SET(var,bit) (((var) & (1<<(bit)))!=0)

#ifdef _WIN32
    #define VREP_IS_FINITE(x) (_finite(x))
    #define VREP_IS_NAN(x) (_isnan(x))
#endif // _WIN32

#ifdef __APPLE__
    #define VREP_IS_FINITE(x) (finite(x))
    #define VREP_IS_NAN(x) ((boost::math::isnan)(x))
#endif // __APPLE__

#ifdef __linux
    #define VREP_IS_NAN(x) (isnan(x))
    #define VREP_IS_FINITE(x)                           \
            (sizeof (x) == sizeof (float)                   \
            ? __finitef (x)                                 \
            : sizeof (x) == sizeof (double)                 \
            ? __finite (x) : __finitel (x))
#endif // __linux

int simEmbLaunch();
int simEmbShutDown();
bool simEmbSwitch(int handle);
int simEmbStart(unsigned char* data,int dataLength);

int simEmbGetObjectHandle(const char* objectName);
int simEmbGetObjectParent(int objectHandle);
int simEmbSetObjectParent(int objectHandle,int parentObjectHandle,bool keepInPlace);

int simEmbGetJointPosition(int jointHandle,extIkReal* position);
int simEmbSetJointPosition(int jointHandle,extIkReal position);
int simEmbGetJointTransformation(int jointHandle,extIkReal* position,extIkReal* quaternion);
int simEmbGetJointMatrix(int jointHandle,extIkReal* matrix);
int simEmbSetSphericalJointQuaternion(int jointHandle,const extIkReal* quaternion);
int simEmbSetSphericalJointMatrix(int jointHandle,const extIkReal* matrix);
int simEmbSetJointMode(int jointHandle,int jointMode);
int simEmbGetJointInterval(int jointHandle,extIkReal* interval);
int simEmbSetJointInterval(int jointHandle,int cyclic,const extIkReal* interval);

int simEmbGetIkGroupHandle(const char* ikGroupName);
int simEmbHandleIkGroup(int ikGroupHandle);
int simEmbSetIkGroupExplicitHandling(int ikGroupHandle,bool explicitHandling);
int simEmbSetIkGroupProperties(int ikGroupHandle,int resolutionMethod,int maxIterations,extIkReal damping);
int simEmbSetIkElementProperties(int ikGroupHandle,int tipDummyHandle,int constraints,const extIkReal* precision,const extIkReal* weight);
int simEmbGetConfigForTipPose(int ikGroupHandle,int jointCnt,const int* jointHandles,extIkReal thresholdDist,int maxIterations,extIkReal* retConfig,const extIkReal* metric,const int* jointOptions,void* reserved);

int simEmbGetObjectTransformation(int objectHandle,int relativeToObjectHandle,extIkReal* position,extIkReal* quaternion);
int simEmbGetObjectMatrix(int objectHandle,int relativeToObjectHandle,extIkReal* matrix);
int simEmbSetObjectTransformation(int objectHandle,int relativeToObjectHandle,const extIkReal* position,const extIkReal* quaternion);
int simEmbSetObjectMatrix(int objectHandle,int relativeToObjectHandle,const extIkReal* matrix);
int simEmbQuaternionToEulerAngles(const extIkReal* quaternion,extIkReal* euler);
int simEmbEulerAnglesToQuaternion(const extIkReal* euler,extIkReal* quaternion);
int simEmbTransformationToMatrix(const extIkReal* position,const extIkReal* quaternion,extIkReal* matrix);
int simEmbMatrixToTransformation(const extIkReal* matrix,extIkReal* position,extIkReal* quaternion);
int simEmbInvertTransformation(extIkReal* position,extIkReal* quaternion);
int simEmbInvertMatrix(extIkReal* matrix);
int simEmbMultiplyTransformations(const extIkReal* position1,const extIkReal* quaternion1,const extIkReal* position2,const extIkReal* quaternion2,extIkReal* positionOut,extIkReal* quaternionOut);
int simEmbMultiplyMatrices(const extIkReal* matrix1,const extIkReal* matrix2,extIkReal* matrixOut);
int simEmbInterpolateTransformations(const extIkReal* position1,const extIkReal* quaternion1,const extIkReal* position2,const extIkReal* quaternion2,extIkReal interpolFactor,extIkReal* positionOut,extIkReal* quaternionOut);
int simEmbInterpolateMatrices(const extIkReal* matrix1,const extIkReal* matrix2,extIkReal interpolFactor,extIkReal* matrixOut);
int simEmbMultTransformationWithVector(const extIkReal* position,const extIkReal* quaternion,extIkReal* vect);
int simEmbMultMatrixWithVector(const extIkReal* matrix,extIkReal* vect);
int simEmbGetRotationAxis(const extIkReal* positionStart,const extIkReal* quaternionStart,const extIkReal* positionGoal,const extIkReal* quaternionGoal,extIkReal* axis,extIkReal* angle);
int simEmbGetRotationAxis_matrix(const extIkReal* matrixStart,const extIkReal* matrixGoal,extIkReal* axis,extIkReal* angle);
int simEmbRotateAroundAxis(const extIkReal* positionIn,const extIkReal* quaternionIn,const extIkReal* axisVector,const extIkReal* axisPosition,extIkReal angle,extIkReal* positionOut,extIkReal* quaternionOut);
int simEmbRotateAroundAxis_matrix(const extIkReal* matrixIn,const extIkReal* axisVector,const extIkReal* axisPosition,extIkReal angle,extIkReal* matrixOut);
