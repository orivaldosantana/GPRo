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

#include "visionVelodyneHDL64ECont.h"
#include "v_repLib.h"

CVisionVelodyneHDL64ECont::CVisionVelodyneHDL64ECont()
{
}

CVisionVelodyneHDL64ECont::~CVisionVelodyneHDL64ECont()
{
    removeAll();
}

int CVisionVelodyneHDL64ECont::addObject(CVisionVelodyneHDL64E* obj)
{
    _allObjects.push_back(obj);
    return(obj->getVelodyneHandle());
}

bool CVisionVelodyneHDL64ECont::removeObject(int velodyneHandle)
{
    for (int i=0;i<int(_allObjects.size());i++)
    {
        if (_allObjects[i]->getVelodyneHandle()==velodyneHandle)
        {
            delete _allObjects[i];
            _allObjects.erase(_allObjects.begin()+i);
            return(true);
        }
    }
    return(false);
}

void CVisionVelodyneHDL64ECont::removeAll()
{
    for (int i=0;i<int(_allObjects.size());i++)
        delete _allObjects[i];
    _allObjects.clear();
}

CVisionVelodyneHDL64E* CVisionVelodyneHDL64ECont::getObject(int velodyneHandle)
{
    for (int i=0;i<int(_allObjects.size());i++)
    {
        if (_allObjects[i]->getVelodyneHandle()==velodyneHandle)
            return(_allObjects[i]);
    }
    return(0);
}
