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

#include "extIk.h"
#include "MyMath.h"
#include "extIkSer.h"
#include "3Vector.h"
#include "4Vector.h"
#include "7Vector.h"

class C3DObject  
{
public:

    C3DObject();
    virtual ~C3DObject();

public:
    virtual bool announceObjectWillBeErased(int objID,bool copyBuffer);
    virtual void announceIkObjectWillBeErased(int ikGroupID,bool copyBuffer);

    void announceObjectWillBeErasedMain(int objID,bool copyBuffer);
    void announceIkObjectWillBeErasedMain(int ikGroupID,bool copyBuffer);

    virtual void performObjectLoadingMapping(std::vector<int>* map);
    void performObjectLoadingMappingMain(std::vector<int>* map);

    void getCumulativeTransformationMatrix(extIkReal m[4][4],bool useTempValues=false);
    void getCumulativeTransformationMatrixPart1(extIkReal m[4][4],bool useTempValues=false);

    void setLocalTransformation(const C7Vector& v);
    void setLocalTransformation(const C4Vector& q);
    void setLocalTransformation(const C3Vector& x);

    void setAbsoluteTransformation(const C7Vector& v);
    void setAbsoluteTransformation(const C4Vector& q);
    void setAbsoluteTransformation(const C3Vector& x);
    C7Vector getParentCumulativeTransformation(bool useTempValues=false);
    C7Vector getCumulativeTransformation(bool useTempValues=false);
    C7Vector getLocalTransformation(bool useTempValues=false);
    C7Vector getCumulativeTransformationPart1(bool useTempValues=false);
    C7Vector& getLocalTransformationPart1(bool useTempValues=false); 

    int getID();
    void setID(int newID);
    void setName(std::string newName);
    std::string getName();
    C3DObject* getParent();
    bool isObjectParentedWith(C3DObject* thePotentialParent);
    int getParentIdentifierLoading();
    void setParentIdentifierLoading(int pID);
    void setParent(C3DObject* newParent,bool actualizeInfo=true);
    int getObjectType();

    std::vector<C3DObject*> childList;

protected:
    C7Vector _transformation; // Quaternion and position
    int _objectID;
    std::string _objectName;
    C3DObject* _parentObject; 
    int _parentID;
    int _objectType;

public:
    virtual void serializeRExtIk(CExtIkSer& ar);
    void serializeRExtIkMain(CExtIkSer& ar);
};
