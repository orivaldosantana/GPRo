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
#include "3X3Matrix.h"

class C4Vector  
{
public:
    C4Vector();
    C4Vector(extIkReal v0,extIkReal v1,extIkReal v2,extIkReal v3);
    C4Vector(const extIkReal v[4]);
    C4Vector(const C3Vector& v);
    C4Vector(const C4Vector& q);
    C4Vector(extIkReal a,extIkReal b,extIkReal g);
    C4Vector(extIkReal angle,const C3Vector& axis);
    C4Vector(const C3Vector& startV,const C3Vector& endV);
    ~C4Vector();

    void setEulerAngles(const C3Vector& v);
    void setEulerAngles(extIkReal a,extIkReal b,extIkReal g);
    void setAngleAndAxis(extIkReal angle,const C3Vector& axis);
    void setVectorMapping(const C3Vector& startV,const C3Vector& endV);
    void buildInterpolation(const C4Vector& fromThis,const C4Vector& toThat,extIkReal t);


    extIkReal getAngleBetweenQuaternions(const C4Vector& q) const;
    C4Vector getAngleAndAxis() const;
    C4Vector getAngleAndAxisNoChecking() const;
    
    C3Vector getEulerAngles() const;

    inline void getInternalData(extIkReal d[4]) const
    {
        d[0]=data[0];
        d[1]=data[1];
        d[2]=data[2];
        d[3]=data[3];
    }
    inline void setInternalData(const extIkReal d[4])
    {
        data[0]=d[0];
        data[1]=d[1];
        data[2]=d[2];
        data[3]=d[3];
    }
    inline extIkReal& operator() (unsigned i)
    {
        return(data[i]);
    }
    inline const extIkReal& operator() (unsigned i) const
    {
        return(data[i]);
    }
    inline void normalize()
    {
        extIkReal l=sqrt(data[0]*data[0]+data[1]*data[1]+data[2]*data[2]+data[3]*data[3]);
        data[0]/=l;
        data[1]/=l;
        data[2]/=l;
        data[3]/=l;
    }
    inline void clear()
    {
        data[0]=0.0;
        data[1]=0.0;
        data[2]=0.0;
        data[3]=0.0;
    }
    inline void setIdentity()
    {
        data[0]=1.0;
        data[1]=0.0;
        data[2]=0.0;
        data[3]=0.0;
    }
    inline C4Vector getInverse() const
    {
        return(C4Vector(data[0],-data[1],-data[2],-data[3]));
    }
    inline void inverse()
    {
        data[1]=-data[1];
        data[2]=-data[2];
        data[3]=-data[3];
    }
    inline C4Vector operator/ (extIkReal d) const
    {
        C4Vector retV;
        retV(0)=data[0]/d;
        retV(1)=data[1]/d;
        retV(2)=data[2]/d;
        retV(3)=data[3]/d;
        return(retV);
    }
    inline C4Vector operator* (extIkReal d) const
    {
        C4Vector retV;
        retV(0)=data[0]*d;
        retV(1)=data[1]*d;
        retV(2)=data[2]*d;
        retV(3)=data[3]*d;
        return(retV);
    }
    inline C4Vector& operator= (const C4Vector& v)
    {
        data[0]=v(0);
        data[1]=v(1);
        data[2]=v(2);
        data[3]=v(3);
        return(*this);
    }
    inline bool operator!= (const C4Vector& v)
    {
        return( (data[0]!=v(0))||(data[1]!=v(1))||(data[2]!=v(2))||(data[3]!=v(3)) );
    }
    inline C4Vector operator* (const C4Vector& v) const
    { // Quaternion multiplication.
        C4Vector retV;
        retV(0)=data[0]*v(0)-data[1]*v(1)-data[2]*v(2)-data[3]*v(3);
        retV(1)=data[0]*v(1)+v(0)*data[1]+data[2]*v(3)-data[3]*v(2);
        retV(2)=data[0]*v(2)+v(0)*data[2]+data[3]*v(1)-data[1]*v(3);
        retV(3)=data[0]*v(3)+v(0)*data[3]+data[1]*v(2)-data[2]*v(1);
        //  retV.normalize(); // NOOOOOOO!!!!!! We might compute the rotation of a vector which should be (q*v*qI).normalize and not q*((v*qi).normalize).normalize !!
        return(retV);
    }
    inline C3Vector getAxis(int index) const
    {
        C3X3Matrix m=getMatrix();
        return(m.axis[index]);
    }
    inline C3Vector operator* (const C3Vector& v) const
    { // Rotation of a vector.
        C4Vector tmpV(1.0,v(0),v(1),v(2));
        tmpV=(*this)*(tmpV*getInverse());
        return(C3Vector(tmpV(1),tmpV(2),tmpV(3)));
    }
    inline C4Vector operator+ (const C4Vector& v) const
    {
        C4Vector retV;
        retV(0)=data[0]+v(0);
        retV(1)=data[1]+v(1);
        retV(2)=data[2]+v(2);
        retV(3)=data[3]+v(3);
        return(retV);
    }
    inline C3X3Matrix getMatrix() const
    {
        C3X3Matrix retM;
        extIkReal xx=data[1]*data[1];
        extIkReal xy=data[1]*data[2];
        extIkReal xz=data[1]*data[3];
        extIkReal xw=data[1]*data[0];
        extIkReal yy=data[2]*data[2];
        extIkReal yz=data[2]*data[3];
        extIkReal yw=data[2]*data[0];
        extIkReal zz=data[3]*data[3];
        extIkReal zw=data[3]*data[0];

        retM(0,0)=extIkReal(1.0-2.0*(yy+zz));
        retM(0,1)=extIkReal(2.0)*(xy-zw);
        retM(0,2)=extIkReal(2.0)*(xz+yw);
        retM(1,0)=extIkReal(2.0)*(xy+zw);
        retM(1,1)=extIkReal(1.0-2.0*(xx+zz));
        retM(1,2)=extIkReal(2.0)*(yz-xw);
        retM(2,0)=extIkReal(2.0)*(xz-yw);
        retM(2,1)=extIkReal(2.0)*(yz+xw);
        retM(2,2)=extIkReal(1.0-2.0*(xx+yy));
        return(retM);
    }
    inline void operator/= (extIkReal d)
    {
        data[0]/=d;
        data[1]/=d;
        data[2]/=d;
        data[3]/=d;
    }
    inline void operator*= (extIkReal d)
    {
        data[0]*=d;
        data[1]*=d;
        data[2]*=d;
        data[3]*=d;
    }
    inline void operator*= (const C4Vector& v)
    {
        (*this)=(*this)*v;
        // Already normalized through * operator
    }
    inline void operator+= (const C4Vector& v)
    {
        data[0]+=v(0);
        data[1]+=v(1);
        data[2]+=v(2);
        data[3]+=v(3);
    }

    static const C4Vector identityRotation;

    extIkReal data[4];
};
