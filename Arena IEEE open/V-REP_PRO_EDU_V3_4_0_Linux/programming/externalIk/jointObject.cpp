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

#include "jointObject.h"
#include "Ct.h"
#include "Tt.h"
#include "v_repConst.h"
#include "Global.h"


CJoint::CJoint()
{ // use only during serialization!!
    commonInit();
    // Following is not needed, but just in case!
    _jointType=sim_joint_revolute_subtype;
    _positionIsCyclic=true;
    _jointPositionRange=piValTimes2;
    _jointMinPosition=-piValue;
    _maxStepSize=extIkReal(10.0)*degToRad;
}

CJoint::CJoint(int jointType)
{
    _jointType=jointType;
    commonInit();
    if (jointType==sim_joint_revolute_subtype)
    {
        _jointMode=sim_jointmode_force;
        _positionIsCyclic=true;
        _jointPositionRange=piValTimes2;
        _jointMinPosition=-piValue;
        _maxStepSize=extIkReal(10.0)*degToRad;
    }
    if (jointType==sim_joint_prismatic_subtype)
    {
        _jointMode=sim_jointmode_force;
        _positionIsCyclic=false;
        _jointPositionRange=(extIkReal)1.0;
        _jointMinPosition=(extIkReal)-0.5;
        _maxStepSize=(extIkReal)0.1;
    }
    if (jointType==sim_joint_spherical_subtype)
    {
        _jointMode=sim_jointmode_force;
        _positionIsCyclic=true;
        _jointPositionRange=piValue;
        _jointMinPosition=0.0; // no meaning here
        _maxStepSize=extIkReal(10.0)*degToRad;
    }
}

void CJoint::commonInit()
{ // set the joint type directly after that!
    _objectType=sim_object_joint_type;
    _screwPitch=0.0;
    _sphericalTransformation.setIdentity();
    _jointPosition=0.0;
    _jointMode=sim_jointmode_ik;
    _dependencyJointID=-1;
    _dependencyJointCoeff=1.0;
    _dependencyJointFact=0.0;
    _objectName="default";
    _ikWeight=1.0;
}

CJoint::~CJoint()
{

}

void CJoint::performObjectLoadingMapping(std::vector<int>* map)
{ // New_Object_ID=map[Old_Object_ID]
    performObjectLoadingMappingMain(map);
    _dependencyJointID=Ct::ct->objCont->getLoadingMapping(map,_dependencyJointID);
}

void CJoint::setJointMode(int theMode)
{
    if (theMode==sim_jointmode_passive)
    {
        _dependencyJointID=-1;
        Ct::ct->objCont->actualizeObjectInformation(); // to actualize dependencies
        _jointMode=theMode;
    }
    if (theMode==sim_jointmode_motion_deprecated)
    {
        if (_jointType!=sim_joint_spherical_subtype)
        {
            _dependencyJointID=-1;
            Ct::ct->objCont->actualizeObjectInformation(); // to actualize dependencies
            _jointMode=theMode;
        }
    }
    if ((theMode==sim_jointmode_dependent)||(theMode==sim_jointmode_reserved_previously_ikdependent))
    {
        if (_jointType!=sim_joint_spherical_subtype)
        {
            Ct::ct->objCont->actualizeObjectInformation(); // to actualize dependencies
            _jointMode=theMode;
        }
    }
    if (theMode==sim_jointmode_dependent)
    {
        if (_jointType!=sim_joint_spherical_subtype)
        {
            Ct::ct->objCont->actualizeObjectInformation(); // to actualize dependencies
            _jointMode=theMode;
        }
    }
    if (theMode==sim_jointmode_force)
    {
        _dependencyJointID=-1;
        _screwPitch=0.0;
        if (_jointType==sim_joint_revolute_subtype)
            _jointPositionRange=tt::getLimitedFloat(0.0,piValTimes2,_jointPositionRange); // new since 18/11/2012 (was forgotten)
        if (_jointType==sim_joint_spherical_subtype)
            _jointPositionRange=piValue;

        Ct::ct->objCont->actualizeObjectInformation(); // to actualize dependencies
        _jointMode=theMode;
    }
    if (theMode==sim_jointmode_ik)
    {
        _dependencyJointID=-1;
        Ct::ct->objCont->actualizeObjectInformation(); // to actualize dependencies
        _jointMode=theMode;
    }
    setPosition(getPosition()); // not sure it is needed here, but better than not!
}

void CJoint::_rectifyDependentJoints()
{   // We rectify now all joints linked to that one (rewritten on 2009-01-27):
    for (int i=0;i<int(directDependentJoints.size());i++)
    {
        if (directDependentJoints[i]->getJointMode()==sim_jointmode_dependent)
            directDependentJoints[i]->setPosition(0.0,false); // value doesn't matter!
    }
}

int CJoint::getJointMode()
{
    return(_jointMode);
}

int CJoint::getDependencyJointID()
{
    return(_dependencyJointID);
}

extIkReal CJoint::getDependencyJointCoeff()
{
    return(_dependencyJointCoeff);
}

extIkReal CJoint::getDependencyJointFact()
{
    return(_dependencyJointFact);
}

int CJoint::getJointType()
{
    return(_jointType);
}

extIkReal CJoint::getScrewPitch() const
{
    return(_screwPitch);
}

void CJoint::setSphericalTransformation(const C4Vector& tr)
{
    C4Vector transf(tr);
    if (_jointPositionRange<piValue*extIkReal(0.99))
    {
        C3X3Matrix theTr(transf);
        C3Vector zReset(0.0,0.0,1.0);
        extIkReal angle=zReset.getAngle(theTr.axis[2]);
        if (angle>_jointPositionRange)
        { // We have to limit the movement:
            C3Vector rotAxis((theTr.axis[2]^zReset).getNormalized());
            C4Vector rot(angle-_jointPositionRange,rotAxis);
            transf=rot*transf;
        }
    }
    _sphericalTransformation=transf;
}

C4Vector CJoint::getSphericalTransformation() const
{
    return(_sphericalTransformation);
}

void CJoint::setMaxStepSize(extIkReal stepS)
{
    if (_jointType==sim_joint_revolute_subtype)
        tt::limitValue(extIkReal(0.01)*degToRad,extIkReal(100000.0),stepS); // high number for screws!
    if (_jointType==sim_joint_prismatic_subtype)
        tt::limitValue(extIkReal(0.0001),extIkReal(1000.0),stepS);
    if (_jointType==sim_joint_spherical_subtype)
        tt::limitValue(extIkReal(0.01)*degToRad,piValue,stepS);
    _maxStepSize=stepS;
}

extIkReal CJoint::getMaxStepSize()
{
    return(_maxStepSize);
}

extIkReal CJoint::getPosition(bool useTempValues) 
{  // useTempValues is false by default
    if (useTempValues)
        return(_jointPosition_tempForIK); 
    return(_jointPosition); 
}

extIkReal CJoint::getIKWeight()
{
    return(_ikWeight);
}

void CJoint::setIKWeight(extIkReal newWeight)
{
    newWeight=tt::getLimitedFloat(-1.0,1.0,newWeight);
    _ikWeight=newWeight;
}

extIkReal CJoint::getPosition_ratio()
{
    return((getPosition()-_jointMinPosition)/_jointPositionRange);
}

void CJoint::setPosition_ratio(extIkReal posRatio)
{
    setPosition(_jointMinPosition+posRatio*_jointPositionRange);
}

void CJoint::setPosition(extIkReal parameter,bool useTempValues)
{ // useTempValues is false by default
    if (_positionIsCyclic)
        parameter=tt::getNormalizedAngle(parameter);
    else
    {
        if (parameter>(getPositionIntervalMin()+getPositionIntervalRange()))
            parameter=getPositionIntervalMin()+getPositionIntervalRange();
        if (parameter<getPositionIntervalMin())
            parameter=getPositionIntervalMin();
    }
    if (useTempValues)  
        _jointPosition_tempForIK=parameter;
    else
        _jointPosition=parameter;

    if (_jointMode==sim_jointmode_dependent)
    { // If this joint is constrained
        extIkReal linked=0.0;
        if (_dependencyJointID!=-1)
        {
            CJoint* anAct=Ct::ct->objCont->getJoint(_dependencyJointID);
            if (anAct!=NULL)
                linked=_dependencyJointCoeff*anAct->getPosition(useTempValues);
        }
        if (useTempValues)  
            _jointPosition_tempForIK=linked+_dependencyJointFact;
        else
            _jointPosition=linked+_dependencyJointFact;
    }
    _rectifyDependentJoints();
}

extIkReal CJoint::getPositionIntervalMin() 
{ 
    return(_jointMinPosition); 
}

void CJoint::setPositionIntervalMin(extIkReal m)
{
    if (_jointType==sim_joint_spherical_subtype)
        return;
    if (_jointType==sim_joint_revolute_subtype)
        m=tt::getLimitedFloat(extIkReal(-100000.0),extIkReal(100000.0),m);
    if (_jointType==sim_joint_prismatic_subtype)
        m=tt::getLimitedFloat(extIkReal(-1000.0),extIkReal(1000.0),m);
    _jointMinPosition=m;
    setPosition(getPosition()); // to make sure it is within range
}

extIkReal CJoint::getPositionIntervalRange() 
{ 
    return(_jointPositionRange); 
}

void CJoint::setPositionIntervalRange(extIkReal r)
{
    if (_jointType==sim_joint_revolute_subtype)
        r=tt::getLimitedFloat(extIkReal(0.001)*degToRad,extIkReal(10000000.0)*degToRad,r);
    if (_jointType==sim_joint_prismatic_subtype)
        r=tt::getLimitedFloat(0.0,extIkReal(1000.0),r);
    if (_jointType==sim_joint_spherical_subtype)
    {
        if (_jointMode!=sim_jointmode_force)
            r=tt::getLimitedFloat(extIkReal(0.001)*degToRad,extIkReal(10000000.0)*degToRad,r);
        else
            r=piValue;
    }
    _jointPositionRange=r;
    setPosition(getPosition()); // to make sure it is within range (will also get/set velocity)
    setSphericalTransformation(getSphericalTransformation());
}

bool CJoint::getPositionIsCyclic()
{
    if (_jointType==sim_joint_prismatic_subtype)
        return(false);
    return(_positionIsCyclic);
}

void CJoint::setPositionIsCyclic(bool c)
{
    if (!c)
        _positionIsCyclic=c;
    else
    {
        if (getJointType()==sim_joint_revolute_subtype)
        {
            _screwPitch=0.0;
            _jointMinPosition=-piValue;
            _jointPositionRange=piValTimes2;
            _positionIsCyclic=c;
        }
    }
}

void CJoint::initializeParametersForIK(extIkReal angularJointLimitationThreshold)
{
    if (_jointType!=sim_joint_spherical_subtype)
        _jointPosition_tempForIK=_jointPosition;
    else
    {
        // 1. Do we need to prepare the thing for the joint limitation?
        _sphericalTransformation_eulerLockTempForIK=0;
        C3X3Matrix m(_sphericalTransformation);
        extIkReal angle=C3Vector::unitZVector.getAngle(m.axis[2]);
        if ( (_jointPositionRange<extIkReal(179.9)*degToRad)&&(angle>1.0*degToRad) )
        {
            if ((_jointPositionRange-angularJointLimitationThreshold)/2.0<angle)
            { // We have to activate the second type of spherical joint (with joint limitation (IK pass dependent))
                _sphericalTransformation_eulerLockTempForIK=2;
                C3Vector n(m.axis[2]);
                n(2)=0.0;
                n.normalize();
                C3Vector y((C3Vector::unitZVector^n).getNormalized());
                extIkReal angle2=C3Vector::unitXVector.getAngle(y);
                C3Vector zz(C3Vector::unitXVector^y);
                if (zz(2)<0.0)
                    angle2=-angle2;
                _jointPosition_tempForIK=0.0; // Not really needed!
                _sphericalTransformation_euler1TempForIK=angle2;
                _sphericalTransformation_euler2TempForIK=angle;
                extIkReal angle3=m.axis[0].getAngle(y);
                C3Vector nz(y^m.axis[0]);
                if (nz*m.axis[2]<0.0)
                    angle3=-angle3;
                _sphericalTransformation_euler3TempForIK=angle3;
            }
        }
        if (_sphericalTransformation_eulerLockTempForIK==0)
        { // No joint limitations for the IK (in this IK pass)
            _jointPosition_tempForIK=0.0; // Not really needed!
            _sphericalTransformation_euler1TempForIK=0.0;
            _sphericalTransformation_euler2TempForIK=0.0;
            _sphericalTransformation_euler3TempForIK=0.0;
        }
    }
}

int CJoint::getDoFs()
{
    if (_jointType!=sim_joint_spherical_subtype)
        return(1);
    return(3);
}

int CJoint::getTempSphericalJointLimitations()
{
    return(_sphericalTransformation_eulerLockTempForIK);
}

void CJoint::getLocalTransformationExPart1(C7Vector& mTr,int index,bool useTempValues)
{ // Used for Jacobian calculation with spherical joints
    if (_sphericalTransformation_eulerLockTempForIK==0)
    { // Spherical joint limitations are not activated in the IK algorithm (but if we come close to the limit, it might get activated in next pass!)
        if (index==0)
        { 
            mTr.setIdentity();
            mTr.Q.setEulerAngles(0.0,piValD2,0.0);
            C7Vector tr2(getLocalTransformation());
            mTr=tr2*mTr;
        }
        if (index==1)
        {
            mTr.setIdentity();
            mTr.Q.setEulerAngles(-piValD2,0.0,-piValD2);
        }
        if (index==2)
        {
            mTr.setIdentity();
            mTr.Q.setEulerAngles(piValD2,0.0,0.0);
        }
    }
    else
    {
        if (index==0)
        {
            mTr=getLocalTransformationPart1();
        }
        if (index==1)
        {
            mTr.setIdentity();
            mTr.Q.setEulerAngles(0.0,piValD2,0.0);
        }
        if (index==2)
        {
            mTr.setIdentity();
            mTr.Q.setEulerAngles(0.0,-piValD2,0.0);
        }
    }
}

extIkReal CJoint::getTempParameterEx(int index)
{
    if (index==0)
        return(_sphericalTransformation_euler1TempForIK);
    if (index==1)
        return(_sphericalTransformation_euler2TempForIK);
    if (index==2)
        return(_sphericalTransformation_euler3TempForIK);
    return(0.0);
}

void CJoint::setTempParameterEx(extIkReal parameter,int index)
{
    if (index==0)
        _sphericalTransformation_euler1TempForIK=parameter;
    if (index==1)
        _sphericalTransformation_euler2TempForIK=parameter;
    if (index==2)
        _sphericalTransformation_euler3TempForIK=parameter;

    if (_sphericalTransformation_eulerLockTempForIK==0)
    { // Spherical joint limitations are not activated in the IK algorithm (but if we come close to the limit, it might get activated in next pass!)
        C4Vector saved(_sphericalTransformation);
        applyTempParametersEx();
        C4Vector tr(saved.getInverse()*_sphericalTransformation);
        C3Vector euler(tr.getEulerAngles());
        _sphericalTransformation_euler1TempForIK=euler(0);
        _sphericalTransformation_euler2TempForIK=euler(1);
        _sphericalTransformation_euler3TempForIK=euler(2);
        _sphericalTransformation=saved;
    }
    else
    { // Spherical joint limitations are activated in the IK algorithm
        C4Vector saved(_sphericalTransformation);
        applyTempParametersEx();

        C3X3Matrix m(_sphericalTransformation);

        extIkReal angle=C3Vector::unitZVector.getAngle(m.axis[2]);
        if (angle>extIkReal(0.01)*degToRad)
        {
            C3Vector n(m.axis[2]);
            n(2)=0.0;
            n.normalize();
            C3Vector y((C3Vector::unitZVector^n).getNormalized());
            extIkReal angle2=C3Vector::unitXVector.getAngle(y);
            C3Vector zz(C3Vector::unitXVector^y);
            if (zz(2)<0.0)
                angle2=-angle2;
            _sphericalTransformation_euler1TempForIK=angle2;
            _sphericalTransformation_euler2TempForIK=angle;
            extIkReal angle3=m.axis[0].getAngle(y);
            C3Vector nz(y^m.axis[0]);
            if (nz*m.axis[2]<0.0)
                angle3=-angle3;
            _sphericalTransformation_euler3TempForIK=angle3;
        }
        else
        { // This is a rare case and should never happen if the spherical joint limitation is not too small!
            extIkReal angle=C3Vector::unitXVector.getAngle(m.axis[0]);
            if ((C3Vector::unitXVector^m.axis[0])(2)<0.0)
                angle=-angle;
            _sphericalTransformation_euler1TempForIK=angle;
            _sphericalTransformation_euler2TempForIK=0.0;
            _sphericalTransformation_euler3TempForIK=0.0;
        }
        _sphericalTransformation=saved;
    }
}

void CJoint::applyTempParametersEx()
{
    if (_jointType==sim_joint_spherical_subtype)
    {
        C7Vector tr1(getLocalTransformationPart1(true));
        C7Vector tr2(getLocalTransformation(true));
        setSphericalTransformation(tr1.Q.getInverse()*tr2.Q);
    }
}

bool CJoint::announceObjectWillBeErased(int objID,bool copyBuffer)
{   // copyBuffer is false by default (if true, we are 'talking' to objects
    // in the copyBuffer)
    // This routine can be called for objCont-objects, but also for objects
    // in the copy-buffer!! So never make use of any 
    // 'Ct::ct->objCont->getObject(id)'-call or similar
    // Return value true means 'this' has to be erased too!
    announceObjectWillBeErasedMain(objID,copyBuffer);
    if (_dependencyJointID==objID)
    {
        _dependencyJointID=-1;
    }
    // We check if the joint is listed in the directDependentJoints:
    for (int i=0;i<int(directDependentJoints.size());i++)
    {
        if (directDependentJoints[i]->getID()==objID)
            directDependentJoints.erase(directDependentJoints.begin()+i);
    }
    return(false);
}

void CJoint::announceIkObjectWillBeErased(int ikGroupID,bool copyBuffer)
{   // copyBuffer is false by default (if true, we are 'talking' to objects
    // in the copyBuffer)
    announceIkObjectWillBeErasedMain(ikGroupID,copyBuffer);
}

void CJoint::serializeRExtIk(CExtIkSer& ar)
{
    serializeRExtIkMain(ar);

    _jointType=ar.readInt();

    _screwPitch=(extIkReal)ar.readFloat();

    _sphericalTransformation(0)=(extIkReal)ar.readFloat();
    _sphericalTransformation(1)=(extIkReal)ar.readFloat();
    _sphericalTransformation(2)=(extIkReal)ar.readFloat();
    _sphericalTransformation(3)=(extIkReal)ar.readFloat();

    unsigned char dummy=ar.readByte();
    _positionIsCyclic=SIM_IS_BIT_SET(dummy,0);

    _jointMinPosition=(extIkReal)ar.readFloat();
    _jointPositionRange=(extIkReal)ar.readFloat();

    _jointPosition=(extIkReal)ar.readFloat();

    _maxStepSize=(extIkReal)ar.readFloat();

    _ikWeight=(extIkReal)ar.readFloat();

    _jointMode=ar.readInt();

    _dependencyJointID=ar.readInt();
    _dependencyJointCoeff=(extIkReal)ar.readFloat();
    _dependencyJointFact=(extIkReal)ar.readFloat();
}
