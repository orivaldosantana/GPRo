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

#include "MyMath.h"

CMath::CMath()
{
}

CMath::~CMath()
{
}

void CMath::limitValue(extIkReal minValue,extIkReal maxValue,extIkReal &value)
{
    if (value>maxValue)
        value=maxValue;
    if (value<minValue) 
        value=minValue;
}

void CMath::limitValue(int minValue,int maxValue,int &value)
{
    if (value>maxValue) 
        value=maxValue;
    if (value<minValue) 
        value=minValue;
}

extIkReal CMath::robustAsin(extIkReal v)
{
    if (!isNumberOk(v))
        return(0.0);
    if (v>=1.0)
        return(piValue*(extIkReal)0.5);
    if (v<=-1.0)
        return(-piValue*(extIkReal)0.5);
    return(asin(v));
}

extIkReal CMath::robustAcos(extIkReal v)
{
    if (!isNumberOk(v))
        return(0.0);
    if (v>=1.0)
        return(0.0);
    if (v<=-1.0)
        return(piValue);
    return(acos(v));
}

bool CMath::isNumberOk(extIkReal v)
{
    return ( (!VREP_IS_NAN(v))&&(fabs(v)!=std::numeric_limits<extIkReal>::infinity()) );    
}
