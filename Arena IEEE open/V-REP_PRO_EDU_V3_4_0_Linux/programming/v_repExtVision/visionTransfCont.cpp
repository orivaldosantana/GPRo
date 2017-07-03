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

#include "visionTransfCont.h"
#include "v_repLib.h"

CVisionTransfCont::CVisionTransfCont()
{
}

CVisionTransfCont::~CVisionTransfCont()
{
    removeAll();
}

int CVisionTransfCont::addObject(CVisionTransf* obj)
{
    _allObjects.push_back(obj);
    return(obj->getReferencePassiveVisionSensorHandle());
}

bool CVisionTransfCont::removeObject(int passiveVisionSensorHandle)
{
    for (int i=0;i<int(_allObjects.size());i++)
    {
        if (_allObjects[i]->getReferencePassiveVisionSensorHandle()==passiveVisionSensorHandle)
        {
            delete _allObjects[i];
            _allObjects.erase(_allObjects.begin()+i);
            return(true);
        }
    }
    return(false);
}

void CVisionTransfCont::removeAll()
{
    for (int i=0;i<int(_allObjects.size());i++)
        delete _allObjects[i];
    _allObjects.clear();
}

void CVisionTransfCont::removeInvalidObjects()
{
    for (int i=0;i<int(_allObjects.size());i++)
    {
        if (!_allObjects[i]->doAllObjectsExistAndAreVisionSensors())
        {
            delete _allObjects[i];
            _allObjects.erase(_allObjects.begin()+i);
            i--; // reprocess this position
        }
    }
}

CVisionTransf* CVisionTransfCont::getVisionTransfFromReferencePassiveVisionSensor(int passiveVisionSensorHandle)
{
    for (int i=0;i<int(_allObjects.size());i++)
    {
        if (_allObjects[i]->getReferencePassiveVisionSensorHandle()==passiveVisionSensorHandle)
            return(_allObjects[i]);
    }
    return(0);
}
