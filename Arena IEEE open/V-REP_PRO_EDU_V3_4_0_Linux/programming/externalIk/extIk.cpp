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

#include "extIk.h"
#include "Ct.h"
#include "v_repConst.h"

bool hasLaunched()
{
    return(Ct::currentHandle>0);
}

int simEmbLaunch()
{
    Ct* newCt=new Ct();
    return(Ct::insertNewInstance(newCt));
}

int simEmbShutDown()
{
    if (!hasLaunched())
        return(-1);
    return(Ct::removeInstance(Ct::currentHandle));
}

bool simEmbSwitch(int handle)
{
    if (!hasLaunched())
        return(false);
    return(Ct::switchToInstance(handle));
}

int simEmbStart(unsigned char* data,int dataLength)
{
    if (!hasLaunched())
        return(-1);
    if ((data==NULL)||(dataLength==0))
        return(-1);
    CExtIkSer ar(data,dataLength);
    Ct::ct->objCont->importIkContent(ar);
    return(int(Ct::ct->objCont->objectList.size()));
}

int simEmbGetObjectHandle(const char* objectName)
{
    if (!hasLaunched())
        return(-1);
    C3DObject* it=Ct::ct->objCont->getObject(objectName);
    if (it==NULL)
        return(-1);
    return(it->getID());
}

int simEmbGetObjectTransformation(int objectHandle,int relativeToObjectHandle,extIkReal* position,extIkReal* quaternion)
{
    if (!hasLaunched())
        return(-1);
    // V-REP quaternion, internally: w x y z
    // V-REP quaternion, at interfaces: x y z w (like ROS)
    C3DObject* it=Ct::ct->objCont->getObject(objectHandle);
    if (it==NULL)
        return(-1);
    if (relativeToObjectHandle==sim_handle_parent)
    {
        relativeToObjectHandle=-1;
        C3DObject* parent=it->getParent();
        if (parent!=NULL)
            relativeToObjectHandle=parent->getID();
    }
    C3DObject* relObj=Ct::ct->objCont->getObject(relativeToObjectHandle);
    if (relativeToObjectHandle!=-1)
    {
        if (relObj==NULL)
            return(-1);
    }
    C7Vector tr;
    if (relativeToObjectHandle==-1)
        tr=it->getCumulativeTransformationPart1();
    else
    {
        C7Vector relTr(relObj->getCumulativeTransformationPart1()); // added ..Part1 on 2010/06/14
        tr=relTr.getInverse()*it->getCumulativeTransformationPart1(); // Corrected bug on 2011/01/22: was getLocalTransformationPart1 before!!!
    }
    quaternion[0]=tr.Q(1);
    quaternion[1]=tr.Q(2);
    quaternion[2]=tr.Q(3);
    quaternion[3]=tr.Q(0);
    position[0]=tr.X(0);
    position[1]=tr.X(1);
    position[2]=tr.X(2);
    return(1);
}

int simEmbGetObjectMatrix(int objectHandle,int relativeToObjectHandle,extIkReal* matrix)
{
    if (!hasLaunched())
        return(-1);
    // V-REP matrix: Xx, Yx, Zx, Px, Xy, Yy, Zy, Py, Xz, Yz, Zz, Pz. (last row, i.e. 0, 0, 0, 1 is omitted)
    C3DObject* it=Ct::ct->objCont->getObject(objectHandle);
    if (it==NULL)
        return(-1);
    if (relativeToObjectHandle==sim_handle_parent)
    {
        relativeToObjectHandle=-1;
        C3DObject* parent=it->getParent();
        if (parent!=NULL)
            relativeToObjectHandle=parent->getID();
    }
    C3DObject* relObj=Ct::ct->objCont->getObject(relativeToObjectHandle);
    if (relativeToObjectHandle!=-1)
    {
        if (relObj==NULL)
            return(-1);
    }
    C7Vector tr;
    if (relativeToObjectHandle==-1)
        tr=it->getCumulativeTransformationPart1();
    else
    {
        C7Vector relTr(relObj->getCumulativeTransformationPart1()); // added ..Part1 on 2010/06/14
        tr=relTr.getInverse()*it->getCumulativeTransformationPart1(); // Corrected bug on 2011/01/22: was getLocalTransformationPart1 before!!!
    }
    tr.getMatrix().copyToInterface(matrix);
    return(1);
}

int simEmbSetObjectTransformation(int objectHandle,int relativeToObjectHandle,const extIkReal* position,const extIkReal* quaternion)
{
    if (!hasLaunched())
        return(-1);
    // V-REP quaternion, internally: w x y z
    // V-REP quaternion, at interfaces: x y z w (like ROS)
    C3DObject* it=Ct::ct->objCont->getObject(objectHandle);
    if (it==NULL)
        return(-1);
    if (relativeToObjectHandle==sim_handle_parent)
    {
        relativeToObjectHandle=-1;
        C3DObject* parent=it->getParent();
        if (parent!=NULL)
            relativeToObjectHandle=parent->getID();
    }
    C3DObject* relObj=Ct::ct->objCont->getObject(relativeToObjectHandle);
    if (relativeToObjectHandle!=-1)
    {
        if (relObj==NULL)
            return(-1);
    }
    if (relativeToObjectHandle==-1)
    {
        C7Vector tr;
        tr.Q(0)=quaternion[3];
        tr.Q(1)=quaternion[0];
        tr.Q(2)=quaternion[1];
        tr.Q(3)=quaternion[2];
        tr.X(0)=position[0];
        tr.X(1)=position[1];
        tr.X(2)=position[2];
        Ct::ct->objCont->setAbsoluteConfiguration(it->getID(),tr,false);
    }
    else
    {
        C7Vector absTr(it->getCumulativeTransformationPart1());
        C7Vector relTr(relObj->getCumulativeTransformationPart1()); // added ..Part1 on 2010/06/14
        C7Vector x(relTr.getInverse()*absTr);
        x.Q(0)=quaternion[3];
        x.Q(1)=quaternion[0];
        x.Q(2)=quaternion[1];
        x.Q(3)=quaternion[2];
        x.X(0)=position[0];
        x.X(1)=position[1];
        x.X(2)=position[2];
        absTr=relTr*x;
        Ct::ct->objCont->setAbsoluteConfiguration(it->getID(),absTr,false);
    }
    return(1);
}

int simEmbSetObjectMatrix(int objectHandle,int relativeToObjectHandle,const extIkReal* matrix)
{
    if (!hasLaunched())
        return(-1);
    // V-REP matrix: Xx, Yx, Zx, Px, Xy, Yy, Zy, Py, Xz, Yz, Zz, Pz. (last row, i.e. 0, 0, 0, 1 is omitted)
    C3DObject* it=Ct::ct->objCont->getObject(objectHandle);
    if (it==NULL)
        return(-1);
    if (relativeToObjectHandle==sim_handle_parent)
    {
        relativeToObjectHandle=-1;
        C3DObject* parent=it->getParent();
        if (parent!=NULL)
            relativeToObjectHandle=parent->getID();
    }
    C3DObject* relObj=Ct::ct->objCont->getObject(relativeToObjectHandle);
    if (relativeToObjectHandle!=-1)
    {
        if (relObj==NULL)
            return(-1);
    }
    if (relativeToObjectHandle==-1)
    {
        C4X4Matrix m;
        m.copyFromInterface(matrix);
        C7Vector tr(m);
        Ct::ct->objCont->setAbsoluteConfiguration(it->getID(),tr,false);
    }
    else
    {
        C7Vector absTr(it->getCumulativeTransformationPart1());
        C7Vector relTr(relObj->getCumulativeTransformationPart1()); // added ..Part1 on 2010/06/14
        C7Vector x(relTr.getInverse()*absTr);
        C4X4Matrix m;
        m.copyFromInterface(matrix);
        x=m.getTransformation();
        absTr=relTr*x;
        Ct::ct->objCont->setAbsoluteConfiguration(it->getID(),absTr,false);
    }
    return(1);
}


int simEmbGetJointPosition(int jointHandle,extIkReal* position)
{
    if (!hasLaunched())
        return(-1);
    CJoint* it=Ct::ct->objCont->getJoint(jointHandle);
    if (it==NULL)
        return(-1);
    if (it->getJointType()==sim_joint_spherical_subtype)
        return(-1);
    position[0]=it->getPosition();
    return(1);
}

int simEmbSetJointPosition(int jointHandle,extIkReal position)
{
    if (!hasLaunched())
        return(-1);
    CJoint* it=Ct::ct->objCont->getJoint(jointHandle);
    if (it==NULL)
        return(-1);
    if (it->getJointType()==sim_joint_spherical_subtype)
        return(-1);
    it->setPosition(position);
    return(1);
}

int simEmbGetIkGroupHandle(const char* ikGroupName)
{
    if (!hasLaunched())
        return(-1);
    CikGroup* it=Ct::ct->ikGroups->getIkGroup(ikGroupName);
    if (it==NULL)
        return(-1);
    return(it->getObjectID());
}

int simEmbHandleIkGroup(int ikGroupHandle)
{
    if (!hasLaunched())
        return(-1);
    CikGroup* it=Ct::ct->ikGroups->getIkGroup(ikGroupHandle);
    if ( (ikGroupHandle!=sim_handle_all)&&(ikGroupHandle!=sim_handle_all_except_explicit) )
    {
        if (it==NULL)
            return(-1);
    }
    int returnValue=0;
    if (ikGroupHandle<0)
        returnValue=Ct::ct->ikGroups->computeAllIkGroups(ikGroupHandle==sim_handle_all_except_explicit);
    else
    { // explicit handling
        if (!it->getExplicitHandling())
            return(-1);
        returnValue=it->computeGroupIk(false);
    }
    return(returnValue);
}

int simEmbQuaternionToEulerAngles(const extIkReal* quaternion,extIkReal* euler)
{
    if (!hasLaunched())
        return(-1);
    // V-REP quaternion, internally: w x y z
    // V-REP quaternion, at interfaces: x y z w (like ROS)
    C4Vector q(quaternion[3],quaternion[0],quaternion[1],quaternion[2]);
    C3Vector e(q.getEulerAngles());
    euler[0]=e(0);
    euler[1]=e(1);
    euler[2]=e(2);
    return(1);
}

int simEmbEulerAnglesToQuaternion(const extIkReal* euler,extIkReal* quaternion)
{
    if (!hasLaunched())
        return(-1);
    // V-REP quaternion, internally: w x y z
    // V-REP quaternion, at interfaces: x y z w (like ROS)
    C4Vector q(euler[0],euler[1],euler[2]);
    quaternion[0]=q(1);
    quaternion[1]=q(2);
    quaternion[2]=q(3);
    quaternion[3]=q(0);
    return(1);
}

int simEmbTransformationToMatrix(const extIkReal* position,const extIkReal* quaternion,extIkReal* matrix)
{
    if (!hasLaunched())
        return(-1);
    // V-REP quaternion, internally: w x y z
    // V-REP quaternion, at interfaces: x y z w (like ROS)
    // V-REP matrix: Xx, Yx, Zx, Px, Xy, Yy, Zy, Py, Xz, Yz, Zz, Pz. (last row, i.e. 0, 0, 0, 1 is omitted)
    C3Vector x(position);
    C4Vector q(quaternion[3],quaternion[0],quaternion[1],quaternion[2]);
    C7Vector tr(q,x);
    tr.getMatrix().copyToInterface(matrix);
    return(1);
}

int simEmbMatrixToTransformation(const extIkReal* matrix,extIkReal* position,extIkReal* quaternion)
{
    if (!hasLaunched())
        return(-1);
    // V-REP quaternion, internally: w x y z
    // V-REP quaternion, at interfaces: x y z w (like ROS)
    // V-REP matrix: Xx, Yx, Zx, Px, Xy, Yy, Zy, Py, Xz, Yz, Zz, Pz. (last row, i.e. 0, 0, 0, 1 is omitted)
    C4X4Matrix m;
    m.copyFromInterface(matrix);
    C7Vector tr(m.getTransformation());
    quaternion[0]=tr.Q(1);
    quaternion[1]=tr.Q(2);
    quaternion[2]=tr.Q(3);
    quaternion[3]=tr.Q(0);
    position[0]=tr.X(0);
    position[1]=tr.X(1);
    position[2]=tr.X(2);
    return(1);
}


int simEmbGetJointTransformation(int jointHandle,extIkReal* position,extIkReal* quaternion)
{
    if (!hasLaunched())
        return(-1);
    // V-REP quaternion, internally: w x y z
    // V-REP quaternion, at interfaces: x y z w (like ROS)
    CJoint* it=Ct::ct->objCont->getJoint(jointHandle);
    if (it==NULL)
        return(-1);
    C7Vector trFull(it->getLocalTransformation());
    C7Vector trPart1(it->getLocalTransformationPart1());
    C7Vector tr(trPart1.getInverse()*trFull);
    quaternion[0]=tr.Q(1);
    quaternion[1]=tr.Q(2);
    quaternion[2]=tr.Q(3);
    quaternion[3]=tr.Q(0);
    position[0]=tr.X(0);
    position[1]=tr.X(1);
    position[2]=tr.X(2);
    return(1);
}

int simEmbGetJointMatrix(int jointHandle,extIkReal* matrix)
{
    if (!hasLaunched())
        return(-1);
    // V-REP matrix: Xx, Yx, Zx, Px, Xy, Yy, Zy, Py, Xz, Yz, Zz, Pz. (last row, i.e. 0, 0, 0, 1 is omitted)
    CJoint* it=Ct::ct->objCont->getJoint(jointHandle);
    if (it==NULL)
        return(-1);
    C7Vector trFull(it->getLocalTransformation());
    C7Vector trPart1(it->getLocalTransformationPart1());
    C7Vector tr(trPart1.getInverse()*trFull);
    tr.getMatrix().copyToInterface(matrix);
    return(1);
}


int simEmbSetSphericalJointQuaternion(int jointHandle,const extIkReal* quaternion)
{
    if (!hasLaunched())
        return(-1);
    // V-REP quaternion, internally: w x y z
    // V-REP quaternion, at interfaces: x y z w (like ROS)
    CJoint* it=Ct::ct->objCont->getJoint(jointHandle);
    if (it==NULL)
        return(-1);
    if (it->getJointType()!=sim_joint_spherical_subtype)
        return(-1);
    C4Vector q;
    q(0)=quaternion[3];
    q(1)=quaternion[0];
    q(2)=quaternion[1];
    q(3)=quaternion[2];
    it->setSphericalTransformation(q);
    return(1);
}

int simEmbSetSphericalJointMatrix(int jointHandle,const extIkReal* matrix)
{
    if (!hasLaunched())
        return(-1);
    // V-REP matrix: Xx, Yx, Zx, Px, Xy, Yy, Zy, Py, Xz, Yz, Zz, Pz. (last row, i.e. 0, 0, 0, 1 is omitted)
    CJoint* it=Ct::ct->objCont->getJoint(jointHandle);
    if (it==NULL)
        return(-1);
    if (it->getJointType()!=sim_joint_spherical_subtype)
        return(-1);
    C4X4Matrix m;
    m.copyFromInterface(matrix);
    it->setSphericalTransformation(m.getTransformation().Q);
    return(1);
}

int simEmbGetObjectParent(int objectHandle)
{
    if (!hasLaunched())
        return(-2);
    C3DObject* it=Ct::ct->objCont->getObject(objectHandle);
    if (it==NULL)
        return(-2);
    int retVal=-1;
    if (it->getParent()!=NULL)
        retVal=it->getParent()->getID();
    return(retVal);
}

int simEmbSetObjectParent(int objectHandle,int parentObjectHandle,bool keepInPlace)
{
    if (!hasLaunched())
        return(-1);
    C3DObject* it=Ct::ct->objCont->getObject(objectHandle);
    C3DObject* parentIt=Ct::ct->objCont->getObject(parentObjectHandle);
    if (it==NULL)
        return(-1);
    if ((parentIt==NULL)&&(parentObjectHandle!=-1))
        return(-1);
    if (keepInPlace)
        Ct::ct->objCont->makeObjectChildOf(it,parentIt);
    else
        it->setParent(parentIt);
    return(1);
}

int simEmbInvertTransformation(extIkReal* position,extIkReal* quaternion)
{
    if (!hasLaunched())
        return(-1);
    // V-REP quaternion, internally: w x y z
    // V-REP quaternion, at interfaces: x y z w (like ROS)
    C7Vector tr;
    tr.Q(0)=quaternion[3];
    tr.Q(1)=quaternion[0];
    tr.Q(2)=quaternion[1];
    tr.Q(3)=quaternion[2];
    tr.X(0)=position[0];
    tr.X(1)=position[1];
    tr.X(2)=position[2];
    tr.inverse();
    quaternion[0]=tr.Q(1);
    quaternion[1]=tr.Q(2);
    quaternion[2]=tr.Q(3);
    quaternion[3]=tr.Q(0);
    position[0]=tr.X(0);
    position[1]=tr.X(1);
    position[2]=tr.X(2);
    return(1);
}

int simEmbInvertMatrix(extIkReal* matrix)
{
    if (!hasLaunched())
        return(-1);
    // V-REP matrix: Xx, Yx, Zx, Px, Xy, Yy, Zy, Py, Xz, Yz, Zz, Pz. (last row, i.e. 0, 0, 0, 1 is omitted)
    C4X4Matrix m;
    m.copyFromInterface(matrix);
    m.inverse();
    m.copyToInterface(matrix);
    return(1);
}

int simEmbMultiplyTransformations(const extIkReal* position1,const extIkReal* quaternion1,const extIkReal* position2,const extIkReal* quaternion2,extIkReal* positionOut,extIkReal* quaternionOut)
{
    if (!hasLaunched())
        return(-1);
    // V-REP quaternion, internally: w x y z
    // V-REP quaternion, at interfaces: x y z w (like ROS)
    C7Vector tr1;
    tr1.Q(0)=quaternion1[3];
    tr1.Q(1)=quaternion1[0];
    tr1.Q(2)=quaternion1[1];
    tr1.Q(3)=quaternion1[2];
    tr1.X(0)=position1[0];
    tr1.X(1)=position1[1];
    tr1.X(2)=position1[2];

    C7Vector tr2;
    tr2.Q(0)=quaternion2[3];
    tr2.Q(1)=quaternion2[0];
    tr2.Q(2)=quaternion2[1];
    tr2.Q(3)=quaternion2[2];
    tr2.X(0)=position2[0];
    tr2.X(1)=position2[1];
    tr2.X(2)=position2[2];

    C7Vector trOut(tr1*tr2);

    quaternionOut[0]=trOut.Q(1);
    quaternionOut[1]=trOut.Q(2);
    quaternionOut[2]=trOut.Q(3);
    quaternionOut[3]=trOut.Q(0);
    positionOut[0]=trOut.X(0);
    positionOut[1]=trOut.X(1);
    positionOut[2]=trOut.X(2);
    return(1);
}

int simEmbMultiplyMatrices(const extIkReal* matrix1,const extIkReal* matrix2,extIkReal* matrixOut)
{
    if (!hasLaunched())
        return(-1);
    // V-REP matrix: Xx, Yx, Zx, Px, Xy, Yy, Zy, Py, Xz, Yz, Zz, Pz. (last row, i.e. 0, 0, 0, 1 is omitted)
    C4X4Matrix m1;
    m1.copyFromInterface(matrix1);
    C4X4Matrix m2;
    m2.copyFromInterface(matrix2);
    C4X4Matrix mout(m1*m2);
    mout.copyToInterface(matrixOut);
    return(1);
}

int simEmbInterpolateTransformations(const extIkReal* position1,const extIkReal* quaternion1,const extIkReal* position2,const extIkReal* quaternion2,extIkReal interpolFactor,extIkReal* positionOut,extIkReal* quaternionOut)
{
    if (!hasLaunched())
        return(-1);
    // V-REP quaternion, internally: w x y z
    // V-REP quaternion, at interfaces: x y z w (like ROS)
    C7Vector tr1;
    tr1.Q(0)=quaternion1[3];
    tr1.Q(1)=quaternion1[0];
    tr1.Q(2)=quaternion1[1];
    tr1.Q(3)=quaternion1[2];
    tr1.X(0)=position1[0];
    tr1.X(1)=position1[1];
    tr1.X(2)=position1[2];

    C7Vector tr2;
    tr2.Q(0)=quaternion2[3];
    tr2.Q(1)=quaternion2[0];
    tr2.Q(2)=quaternion2[1];
    tr2.Q(3)=quaternion2[2];
    tr2.X(0)=position2[0];
    tr2.X(1)=position2[1];
    tr2.X(2)=position2[2];

    C7Vector trOut;
    trOut.buildInterpolation(tr1,tr2,interpolFactor);

    quaternionOut[0]=trOut.Q(1);
    quaternionOut[1]=trOut.Q(2);
    quaternionOut[2]=trOut.Q(3);
    quaternionOut[3]=trOut.Q(0);
    positionOut[0]=trOut.X(0);
    positionOut[1]=trOut.X(1);
    positionOut[2]=trOut.X(2);
    return(1);
}

int simEmbInterpolateMatrices(const extIkReal* matrix1,const extIkReal* matrix2,extIkReal interpolFactor,extIkReal* matrixOut)
{
    if (!hasLaunched())
        return(-1);
    // V-REP matrix: Xx, Yx, Zx, Px, Xy, Yy, Zy, Py, Xz, Yz, Zz, Pz. (last row, i.e. 0, 0, 0, 1 is omitted)
    C4X4Matrix m;
    m.copyFromInterface(matrix1);
    C7Vector tr1(m.getTransformation());

    m.copyFromInterface(matrix2);
    C7Vector tr2(m.getTransformation());

    C7Vector trOut;
    trOut.buildInterpolation(tr1,tr2,interpolFactor);

    trOut.getMatrix().copyToInterface(matrixOut);
    return(1);
}

int simEmbMultTransformationWithVector(const extIkReal* position,const extIkReal* quaternion,extIkReal* vect)
{
    if (!hasLaunched())
        return(-1);
    // V-REP quaternion, internally: w x y z
    // V-REP quaternion, at interfaces: x y z w (like ROS)
    C7Vector tr;
    tr.Q(0)=quaternion[3];
    tr.Q(1)=quaternion[0];
    tr.Q(2)=quaternion[1];
    tr.Q(3)=quaternion[2];
    tr.X(0)=position[0];
    tr.X(1)=position[1];
    tr.X(2)=position[2];

    C3Vector v1(vect);

    C3Vector v2(tr*v1);

    vect[0]=v2(0);
    vect[1]=v2(1);
    vect[2]=v2(2);
    return(1);
}

int simEmbMultMatrixWithVector(const extIkReal* matrix,extIkReal* vect)
{
    if (!hasLaunched())
        return(-1);
    // V-REP matrix: Xx, Yx, Zx, Px, Xy, Yy, Zy, Py, Xz, Yz, Zz, Pz. (last row, i.e. 0, 0, 0, 1 is omitted)
    C4X4Matrix m;
    m.copyFromInterface(matrix);

    C3Vector v1(vect);

    C3Vector v2(m*v1);

    vect[0]=v2(0);
    vect[1]=v2(1);
    vect[2]=v2(2);
    return(1);
}

int simEmbGetRotationAxis(const extIkReal* quaternionStart,const extIkReal* quaternionGoal,extIkReal* axis,extIkReal* angle)
{
    if (!hasLaunched())
        return(-1);
    // V-REP quaternion, internally: w x y z
    // V-REP quaternion, at interfaces: x y z w (like ROS)
    C4Vector qStart;
    qStart(0)=quaternionStart[3];
    qStart(1)=quaternionStart[0];
    qStart(2)=quaternionStart[1];
    qStart(3)=quaternionStart[2];

    C4Vector qGoal;
    qGoal(0)=quaternionGoal[3];
    qGoal(1)=quaternionGoal[0];
    qGoal(2)=quaternionGoal[1];
    qGoal(3)=quaternionGoal[2];

    // Following few lines taken from the quaternion interpolation part:
    C4Vector AA(qStart);
    C4Vector BB(qGoal);
    if (AA(0)*BB(0)+AA(1)*BB(1)+AA(2)*BB(2)+AA(3)*BB(3)<0.0)
        AA=AA*-1.0;
    C4Vector r((AA.getInverse()*BB).getAngleAndAxis());

    C3Vector v(r(1),r(2),r(3));
    v=AA*v;

    axis[0]=v(0);
    axis[1]=v(1);
    axis[2]=v(2);
    extIkReal l=sqrt(v(0)*v(0)+v(1)*v(1)+v(2)*v(2));
    if (l!=0.0)
    {
        axis[0]/=l;
        axis[1]/=l;
        axis[2]/=l;
    }
    angle[0]=r(0);

    return(1);
}

int simEmbGetRotationAxis_matrix(const extIkReal* matrixStart,const extIkReal* matrixGoal,extIkReal* axis,extIkReal* angle)
{
    if (!hasLaunched())
        return(-1);
    // V-REP matrix: Xx, Yx, Zx, Px, Xy, Yy, Zy, Py, Xz, Yz, Zz, Pz. (last row, i.e. 0, 0, 0, 1 is omitted)
    C4X4Matrix m;

    m.copyFromInterface(matrixStart);
    C4Vector qStart(m.M.getQuaternion());

    m.copyFromInterface(matrixGoal);
    C4Vector qGoal(m.M.getQuaternion());

    // Following few lines taken from the quaternion interpolation part:
    C4Vector AA(qStart);
    C4Vector BB(qGoal);
    if (AA(0)*BB(0)+AA(1)*BB(1)+AA(2)*BB(2)+AA(3)*BB(3)<0.0)
        AA=AA*-1.0;
    C4Vector r((AA.getInverse()*BB).getAngleAndAxis());

    C3Vector v(r(1),r(2),r(3));
    v=AA*v;

    axis[0]=v(0);
    axis[1]=v(1);
    axis[2]=v(2);
    extIkReal l=(extIkReal)sqrt(v(0)*v(0)+v(1)*v(1)+v(2)*v(2));
    if (l!=0.0)
    {
        axis[0]/=l;
        axis[1]/=l;
        axis[2]/=l;
    }
    angle[0]=r(0);

    return(1);
}

int simEmbRotateAroundAxis(const extIkReal* positionIn,const extIkReal* quaternionIn,const extIkReal* axisVector,const extIkReal* axisPosition,extIkReal angle,extIkReal* positionOut,extIkReal* quaternionOut)
{
    if (!hasLaunched())
        return(-1);
    // V-REP quaternion, internally: w x y z
    // V-REP quaternion, at interfaces: x y z w (like ROS)
    C7Vector m;
    m.Q(0)=quaternionIn[3];
    m.Q(1)=quaternionIn[0];
    m.Q(2)=quaternionIn[1];
    m.Q(3)=quaternionIn[2];
    m.X(0)=positionIn[0];
    m.X(1)=positionIn[1];
    m.X(2)=positionIn[2];

    C3Vector ax(axisVector);
    C3Vector pos(axisPosition);

    extIkReal alpha=-atan2(ax(1),ax(0));
    extIkReal beta=atan2(-sqrt(ax(0)*ax(0)+ax(1)*ax(1)),ax(2));
    m.X-=pos;
    C7Vector r;
    r.X.clear();
    r.Q.setEulerAngles(0.0,0.0,alpha);
    m=r*m;
    r.Q.setEulerAngles(0.0,beta,0.0);
    m=r*m;
    r.Q.setEulerAngles(0.0,0.0,angle);
    m=r*m;
    r.Q.setEulerAngles(0.0,-beta,0.0);
    m=r*m;
    r.Q.setEulerAngles(0.0,0.0,-alpha);
    m=r*m;
    m.X+=pos;

    quaternionOut[0]=m.Q(1);
    quaternionOut[1]=m.Q(2);
    quaternionOut[2]=m.Q(3);
    quaternionOut[3]=m.Q(0);
    positionOut[0]=m.X(0);
    positionOut[1]=m.X(1);
    positionOut[2]=m.X(2);
    return(1);
}

int simEmbRotateAroundAxis_matrix(const extIkReal* matrixIn,const extIkReal* axisVector,const extIkReal* axisPosition,extIkReal angle,extIkReal* matrixOut)
{
    if (!hasLaunched())
        return(-1);
    // V-REP matrix: Xx, Yx, Zx, Px, Xy, Yy, Zy, Py, Xz, Yz, Zz, Pz. (last row, i.e. 0, 0, 0, 1 is omitted)
    C4X4Matrix mat;
    mat.copyFromInterface(matrixIn);
    C7Vector m(mat.getTransformation());

    C3Vector ax(axisVector);
    C3Vector pos(axisPosition);

    extIkReal alpha=-atan2(ax(1),ax(0));
    extIkReal beta=atan2(-sqrt(ax(0)*ax(0)+ax(1)*ax(1)),ax(2));
    m.X-=pos;
    C7Vector r;
    r.X.clear();
    r.Q.setEulerAngles(0.0,0.0,alpha);
    m=r*m;
    r.Q.setEulerAngles(0.0,beta,0.0);
    m=r*m;
    r.Q.setEulerAngles(0.0,0.0,angle);
    m=r*m;
    r.Q.setEulerAngles(0.0,-beta,0.0);
    m=r*m;
    r.Q.setEulerAngles(0.0,0.0,-alpha);
    m=r*m;
    m.X+=pos;

    m.getMatrix().copyToInterface(matrixOut);
    return(1);
}

int simEmbSetJointMode(int jointHandle,int jointMode)
{
    if (!hasLaunched())
        return(-1);
    CJoint* it=Ct::ct->objCont->getJoint(jointHandle);
    if (it==NULL)
        return(-1);
    it->setJointMode(jointMode);
    return(1);
}

int simEmbGetJointInterval(int jointHandle,extIkReal* interval)
{
    if (!hasLaunched())
        return(-1);
    CJoint* it=Ct::ct->objCont->getJoint(jointHandle);
    if (it==NULL)
        return(-1);
    int retVal=0;
    if (it->getPositionIsCyclic())
        retVal=1;
    interval[0]=it->getPositionIntervalMin();
    interval[1]=it->getPositionIntervalRange();
    return(retVal);
}

int simEmbSetJointInterval(int jointHandle,int cyclic,const extIkReal* interval)
{
    if (!hasLaunched())
        return(-1);
    CJoint* it=Ct::ct->objCont->getJoint(jointHandle);
    if (it==NULL)
        return(-1);
    extIkReal previousPos=it->getPosition();
    it->setPositionIsCyclic(cyclic!=0);
    it->setPositionIntervalMin(interval[0]);
    it->setPositionIntervalRange(interval[1]);
    it->setPosition(previousPos);
    return(1);
}

int simEmbSetIkGroupExplicitHandling(int ikGroupHandle,bool explicitHandling)
{
    if (!hasLaunched())
        return(-1);
    CikGroup* it=Ct::ct->ikGroups->getIkGroup(ikGroupHandle);
    if (it==NULL)
        return(-1);
    it->setExplicitHandling(explicitHandling);
    return(1);
}

int simEmbSetIkGroupProperties(int ikGroupHandle,int resolutionMethod,int maxIterations,extIkReal damping)
{
    if (!hasLaunched())
        return(-1);
    CikGroup* it=Ct::ct->ikGroups->getIkGroup(ikGroupHandle);
    if (it==NULL)
        return(-1);
    it->setCalculationMethod(resolutionMethod);
    it->setMaxIterations(maxIterations);
    it->setDlsFactor(damping);
    return(1);
}

int simEmbSetIkElementProperties(int ikGroupHandle,int tipDummyHandle,int constraints,const extIkReal* precision,const extIkReal* weight)
{
    if (!hasLaunched())
        return(-1);
    CikGroup* it=Ct::ct->ikGroups->getIkGroup(ikGroupHandle);
    if (it==NULL)
        return(-1);
    CDummy* dum=Ct::ct->objCont->getDummy(tipDummyHandle);
    if (dum==NULL)
        return(-1);
    CikEl* el=it->getIkElementWithTooltipID(tipDummyHandle);
    if (el==NULL)
        return(-1);
    if (constraints!=-1)
        el->setConstraints(constraints);
    if (precision!=NULL)
    {
        el->setMinLinearPrecision(precision[0]);
        el->setMinAngularPrecision(precision[1]);
    }
    if (weight!=NULL)
    {
        el->setPositionWeight(weight[0]);
        el->setOrientationWeight(weight[1]);
    }
    return(1);
}

int simEmbGetConfigForTipPose(int ikGroupHandle,int jointCnt,const int* jointHandles,extIkReal thresholdDist,int maxIterations,extIkReal* retConfig,const extIkReal* metric,const int* jointOptions,void* reserved)
{
    if (!hasLaunched())
        return(-1);
    CikGroup* ikGroup=Ct::ct->ikGroups->getIkGroup(ikGroupHandle);
    if (ikGroup==NULL)
        return(-1);
    const extIkReal _defaultMetric[4]={1.0,1.0,1.0,(extIkReal)0.1};
    const extIkReal* theMetric=_defaultMetric;
    if (metric!=NULL)
        theMetric=metric;
    std::vector<CJoint*> joints;
    std::vector<extIkReal> minVals;
    std::vector<extIkReal> rangeVals;
    bool err=(ikGroup==NULL);
    for (int i=0;i<jointCnt;i++)
    {
        CJoint* aJoint=Ct::ct->objCont->getJoint(jointHandles[i]);
        if (aJoint==NULL)
            err=true;
        else
        {
            joints.push_back(aJoint);
            if (aJoint->getPositionIsCyclic())
            {
                minVals.push_back(-piValue);
                rangeVals.push_back(piValTimes2);
            }
            else
            {
                minVals.push_back(aJoint->getPositionIntervalMin());
                rangeVals.push_back(aJoint->getPositionIntervalRange());
            }
        }
    }
    std::vector<CDummy*> tips;
    std::vector<CDummy*> targets;
    std::vector<C3DObject*> bases;
    if (ikGroup!=NULL)
    {
        if (ikGroup->ikElements.size()>0)
        {
            for (size_t i=0;i<ikGroup->ikElements.size();i++)
            {
                CDummy* tip=Ct::ct->objCont->getDummy(ikGroup->ikElements[i]->getTooltip());
                CDummy* target=Ct::ct->objCont->getDummy(ikGroup->ikElements[i]->getTarget());
                C3DObject* base=NULL;
                if (ikGroup->ikElements[i]->getAlternativeBaseForConstraints()!=-1)
                    base=Ct::ct->objCont->getObject(ikGroup->ikElements[i]->getAlternativeBaseForConstraints());
                else
                    base=Ct::ct->objCont->getObject(ikGroup->ikElements[i]->getBase());
                if ((tip==NULL)||(target==NULL))
                    err=true;
                tips.push_back(tip);
                targets.push_back(target);
                bases.push_back(base);
            }
        }
        else
            err=true;
    }

    if (!err)
    {
        // Save joint positions/modes (all of them, just in case)
        std::vector<CJoint*> sceneJoints;
        std::vector<extIkReal> initSceneJointValues;
        std::vector<int> initSceneJointModes;
        for (size_t i=0;i<Ct::ct->objCont->jointList.size();i++)
        {
            CJoint* aj=Ct::ct->objCont->getJoint(Ct::ct->objCont->jointList[i]);
            sceneJoints.push_back(aj);
            initSceneJointValues.push_back(aj->getPosition());
            initSceneJointModes.push_back(aj->getJointMode());
        }

        ikGroup->setAllInvolvedJointsToPassiveMode();

        bool ikGroupWasActive=ikGroup->getActive();
        if (!ikGroupWasActive)
            ikGroup->setActive(true);

        // It can happen that some IK elements get deactivated when the user provided wrong handles, so save the activation state:
        std::vector<bool> enabledElements;
        for (size_t i=0;i<ikGroup->ikElements.size();i++)
            enabledElements.push_back(ikGroup->ikElements[i]->getActive());

        // Set the correct mode for the joints involved:
        for (int i=0;i<jointCnt;i++)
        {
            if ( (jointOptions==NULL)||((jointOptions[i]&1)==0) )
                joints[i]->setJointMode(sim_jointmode_ik);
            else
                joints[i]->setJointMode(sim_jointmode_dependent);
        }

        // do the calculation:
        bool foundSomething=false;
        for (int iterationCnt=0;iterationCnt<maxIterations;iterationCnt++)
        {
            // 1. Pick a random state:
            for (int i=0;i<jointCnt;i++)
                joints[i]->setPosition(minVals[i]+(rand()/(extIkReal)RAND_MAX)*rangeVals[i]);

            // 2. Check distances between tip and target pairs (there might be several pairs!):
            extIkReal cumulatedDist=0.0;
            for (size_t el=0;el<ikGroup->ikElements.size();el++)
            {
                C7Vector tipTr(tips[el]->getCumulativeTransformation());
                C7Vector targetTr(targets[el]->getCumulativeTransformation());
                C7Vector relTrInv(C7Vector::identityTransformation);
                if (bases[el]!=NULL)
                    relTrInv=bases[el]->getCumulativeTransformationPart1().getInverse();
                tipTr=relTrInv*tipTr;
                targetTr=relTrInv*targetTr;
                C3Vector dx(tipTr.X-targetTr.X);
                dx(0)*=theMetric[0];
                dx(1)*=theMetric[1];
                dx(2)*=theMetric[2];
                extIkReal angle=tipTr.Q.getAngleBetweenQuaternions(targetTr.Q)*theMetric[3];
                cumulatedDist+=sqrtf(dx(0)*dx(0)+dx(1)*dx(1)+dx(2)*dx(2)+angle*angle);
            }

            // 3. If distance<=threshold, try to perform IK:
            if (cumulatedDist<=thresholdDist)
            {
                if (sim_ikresult_success==ikGroup->computeGroupIk(true))
                { // 3.1 We found a configuration that works!
                    for (int i=0;i<jointCnt;i++)
                        retConfig[i]=joints[i]->getPosition();
                    foundSomething=true;
                    break;
                }
            }
        }

        if (!ikGroupWasActive)
            ikGroup->setActive(false);

        // Restore the IK element activation state:
        for (size_t i=0;i<ikGroup->ikElements.size();i++)
            ikGroup->ikElements[i]->setActive(enabledElements[i]);

        // Restore joint positions/modes:
        for (size_t i=0;i<sceneJoints.size();i++)
        {
            if (sceneJoints[i]->getPosition()!=initSceneJointValues[i])
                sceneJoints[i]->setPosition(initSceneJointValues[i]);
            if (sceneJoints[i]->getJointMode()!=initSceneJointModes[i])
                sceneJoints[i]->setJointMode(initSceneJointModes[i]);
        }

        if (foundSomething)
            return(1);
        return(0);
    }
    return(-1);
}
