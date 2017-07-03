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

#include "mathDefines.h"
#include "extIk.h"
#include "3Vector.h"

class C6X6Matrix;

class C6Vector  
{
public:
    C6Vector();
    C6Vector(extIkReal v0,extIkReal v1,extIkReal v2,extIkReal v3,extIkReal v4,extIkReal v5);
    C6Vector(const extIkReal v[6]);
    C6Vector(const C3Vector& v0,const C3Vector& v1);
    C6Vector(const C6Vector& v);
    ~C6Vector();

    void clear();
    C6X6Matrix getSpatialCross() const;

    C6Vector operator* (extIkReal d) const;
    C6Vector operator/ (extIkReal d) const;
    C6Vector operator+ (const C6Vector& v) const;
    C6Vector operator- (const C6Vector& v) const;

    void operator*= (extIkReal d);
    void operator/= (extIkReal d);
    void operator+= (const C6Vector& v);
    void operator-= (const C6Vector& v);

    extIkReal operator* (const C6Vector& v) const;
    C6Vector& operator= (const C6Vector& v);

    inline void getInternalData(extIkReal d[6]) const
    {
        V[0].getInternalData(d+0);
        V[1].getInternalData(d+3);
    }
    inline void setInternalData(const extIkReal d[6])
    {
        V[0].setInternalData(d+0);
        V[1].setInternalData(d+3);
    }
    inline extIkReal& operator() (unsigned i)
    {
        if (i<3)
            return(V[0](i));
        else
            return(V[1](i-3));
    }
    inline const extIkReal& operator() (unsigned i) const
    {
        if (i<3)
            return(V[0](i));
        else
            return(V[1](i-3));
    }

    C3Vector V[2];
};
