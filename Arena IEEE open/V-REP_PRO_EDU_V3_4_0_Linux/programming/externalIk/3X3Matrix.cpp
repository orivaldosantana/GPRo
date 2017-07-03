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

#include "3X3Matrix.h"
#include "4Vector.h"
#include "MyMath.h"


C3X3Matrix::C3X3Matrix()
{

}

C3X3Matrix::C3X3Matrix(const C3Vector& xAxis,const C3Vector& yAxis,const C3Vector& zAxis)
{
    (*this).axis[0]=xAxis;
    (*this).axis[1]=yAxis;
    (*this).axis[2]=zAxis;
}

C3X3Matrix::C3X3Matrix(const C4Vector& q)
{
    (*this)=q.getMatrix();
}

C3X3Matrix::C3X3Matrix(const C3X3Matrix& m)
{
    (*this).axis[0]=m.axis[0];
    (*this).axis[1]=m.axis[1];
    (*this).axis[2]=m.axis[2];
}

C3X3Matrix::~C3X3Matrix()
{

}

void C3X3Matrix::setEulerAngles(extIkReal a,extIkReal b,extIkReal g)
{ // a,b anf g are in radian!
    extIkReal A=(extIkReal)cos(a);
    extIkReal B=(extIkReal)sin(a);
    extIkReal C=(extIkReal)cos(b);
    extIkReal D=(extIkReal)sin(b);
    extIkReal E=(extIkReal)cos(g);
    extIkReal F=(extIkReal)sin(g);
    extIkReal AD=A*D;
    extIkReal BD=B*D;
    axis[0](0)=C*E;
    axis[1](0)=-C*F;
    axis[2](0)=D;
    axis[0](1)=BD*E+A*F;
    axis[1](1)=-BD*F+A*E;
    axis[2](1)=-B*C;
    axis[0](2)=-AD*E+B*F;
    axis[1](2)=AD*F+B*E;
    axis[2](2)=A*C;
}

void C3X3Matrix::setEulerAngles(const C3Vector& v)
{ // v(0), v(1), v(2) are in radian!
    setEulerAngles(v(0),v(1),v(2));
}

C3Vector C3X3Matrix::getEulerAngles() const
{ // Angles are in radians!! // THERE IS ANOTHER SUCH ROUTINE IN C4X4MATRIX
    C3Vector retV;
    extIkReal m02=axis[2](0);
    if (m02>1.0)
        m02=1.0;    // Just in case
    if (m02<-1.0)
        m02=-1.0;   // Just in case
    
    retV(1)=CMath::robustAsin(m02);
    if (m02<0.0)
        m02=-m02;
    if (m02<extIkReal(0.999995))
    {   // No gimbal lock
        retV(0)=(extIkReal)atan2(-axis[2](1),axis[2](2));
        retV(2)=(extIkReal)atan2(-axis[1](0),axis[0](0));
    }
    else
    {   // Gimbal lock has occured
        retV(0)=0.0;
        retV(2)=(extIkReal)atan2(axis[0](1),axis[1](1));
    }
    return(retV);
}

C4Vector C3X3Matrix::getQuaternion() const
{   
    C4Vector retV;
    extIkReal trace=axis[0](0)+axis[1](1)+axis[2](2);
    if (trace>=0.0) 
    {
        extIkReal s=sqrt(trace+extIkReal(1.0))*extIkReal(2.0);
        retV(1)=(axis[1](2)-axis[2](1))/s;
        retV(2)=(axis[2](0)-axis[0](2))/s;
        retV(3)=(axis[0](1)-axis[1](0))/s;
        retV(0)=extIkReal(0.25)*s;
    }
    else
    {
        if ((axis[0](0)>=axis[1](1))&&(axis[0](0)>=axis[2](2)))
        {
            extIkReal s=sqrt(extIkReal(1.0)+axis[0](0)-axis[1](1)-axis[2](2))*extIkReal(2.0);
            retV(1)=extIkReal(0.25)*s;
            retV(2)=(axis[0](1)+axis[1](0))/s;
            retV(3)=(axis[2](0)+axis[0](2))/s;
            retV(0)=(axis[1](2)-axis[2](1))/s;
        }
        else
        {
            if ((axis[1](1)>=axis[0](0))&&(axis[1](1)>=axis[2](2)))
            {
                extIkReal s=sqrt(extIkReal(1.0)+axis[1](1)-axis[0](0)-axis[2](2))*extIkReal(2.0);
                retV(1)=(axis[0](1)+axis[1](0))/s;
                retV(2)=extIkReal(0.25)*s;
                retV(3)=(axis[1](2)+axis[2](1))/s;
                retV(0)=(axis[2](0)-axis[0](2))/s;
            }
            else
            {
                extIkReal s=sqrt(extIkReal(1.0)+axis[2](2)-axis[0](0)-axis[1](1))*extIkReal(2.0);
                retV(1)=(axis[2](0)+axis[0](2))/s;
                retV(2)=(axis[1](2)+axis[2](1))/s;
                retV(3)=extIkReal(0.25)*s;
                retV(0)=(axis[0](1)-axis[1](0))/s;
            }
        }
    }
    retV.normalize(); // Really needed?
    return(retV);
}

void C3X3Matrix::buildInterpolation(const C3X3Matrix& fromThis,const C3X3Matrix& toThat,extIkReal t)
{   // Builds the interpolation (based on t) from 'fromThis' to 'toThat'
    C4Vector out;
    out.buildInterpolation(fromThis.getQuaternion(),toThat.getQuaternion(),t);
    (*this)=out;
}

void C3X3Matrix::buildXRotation(extIkReal angle)
{
    extIkReal c=(extIkReal)cos(angle);
    extIkReal s=(extIkReal)sin(angle);
    axis[0](0)=1.0;
    axis[1](0)=0.0;
    axis[2](0)=0.0;
    axis[0](1)=0.0;
    axis[1](1)=c;
    axis[2](1)=-s;
    axis[0](2)=0.0;
    axis[1](2)=s;
    axis[2](2)=c;
}

void C3X3Matrix::buildYRotation(extIkReal angle)
{
    extIkReal c=(extIkReal)cos(angle);
    extIkReal s=(extIkReal)sin(angle);
    axis[0](0)=c;
    axis[1](0)=0.0;
    axis[2](0)=s;
    axis[0](1)=0.0;
    axis[1](1)=1.0;
    axis[2](1)=0.0;
    axis[0](2)=-s;
    axis[1](2)=0.0;
    axis[2](2)=c;
}

void C3X3Matrix::buildZRotation(extIkReal angle)
{
    extIkReal c=(extIkReal)cos(angle);
    extIkReal s=(extIkReal)sin(angle);
    axis[0](0)=c;
    axis[1](0)=-s;
    axis[2](0)=0.0;
    axis[0](1)=s;
    axis[1](1)=c;
    axis[2](1)=0.0;
    axis[0](2)=0.0;
    axis[1](2)=0.0;
    axis[2](2)=1.0;
}

C3Vector C3X3Matrix::getNormalVector() const
{ // returns the normal vector to the plane described by axis[0],axis[1],axis[2]
    C3Vector v0(axis[0]-axis[1]);
    C3Vector v1(axis[1]-axis[2]);
    return((v0^v1).getNormalized());
}
