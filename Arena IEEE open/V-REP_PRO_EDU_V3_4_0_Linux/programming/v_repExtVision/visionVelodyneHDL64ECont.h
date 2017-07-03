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

#pragma once

#include "visionVelodyneHDL64E.h"
#include <vector>

class CVisionVelodyneHDL64ECont  
{
public:
    CVisionVelodyneHDL64ECont();
    virtual ~CVisionVelodyneHDL64ECont();

    int addObject(CVisionVelodyneHDL64E* obj);
    CVisionVelodyneHDL64E* getObject(int velodyneHandle);
    bool removeObject(int velodyneHandle);
    void removeAll();

private:
    std::vector<CVisionVelodyneHDL64E*> _allObjects;
};
