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

#include "stackBool.h"

CStackBool::CStackBool(bool theValue)
{
    _objectType=STACK_BOOL;
    _value=theValue;
}

CStackBool::~CStackBool()
{
}

bool CStackBool::getValue()
{
    return(_value);
}

void CStackBool::setValue(bool theValue)
{
    _value=theValue;
}

CStackObject* CStackBool::copyYourself()
{
    CStackBool* retVal=new CStackBool(_value);
    return(retVal);
}
