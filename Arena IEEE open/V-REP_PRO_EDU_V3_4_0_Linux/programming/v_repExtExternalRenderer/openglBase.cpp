// This file is part of the EXTERNAL RENDERER PLUGIN for V-REP
// 
// Copyright 2006-2017 Coppelia Robotics GmbH. All rights reserved. 
// marc@coppeliarobotics.com
// www.coppeliarobotics.com
// 
// The EXTERNAL RENDERER PLUGIN is licensed under the terms of GNU GPL:
// 
// -------------------------------------------------------------------
// The EXTERNAL RENDERER PLUGIN is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// THE EXTERNAL RENDERER PLUGIN IS DISTRIBUTED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
// WARRANTY. THE USER WILL USE IT AT HIS/HER OWN RISK. THE ORIGINAL
// AUTHORS AND COPPELIA ROBOTICS GMBH WILL NOT BE LIABLE FOR DATA LOSS,
// DAMAGES, LOSS OF PROFITS OR ANY OTHER KIND OF LOSS WHILE USING OR
// MISUSING THIS SOFTWARE.
//  
// See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with the EXTERNAL RENDERER PLUGIN.  If not, see <http://www.gnu.org/licenses/>.
// -------------------------------------------------------------------
//
// This file was automatically created for V-REP release V3.4.0 rev. 1 on April 5th 2017

#include "openglBase.h"
#include "MyMath.h"
#include <qopengl.h>

COpenglBase::COpenglBase(int associatedObjectHandle)
{
    _associatedObjectHandle=associatedObjectHandle;
}

COpenglBase::~COpenglBase()
{
}

int COpenglBase::getAssociatedObjectHandle()
{
    return(_associatedObjectHandle);
}

void COpenglBase::makeContextCurrent()
{
}

void COpenglBase::doneCurrentContext()
{
}

void COpenglBase::initGL()
{
    makeContextCurrent();

    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
    glBlendFunc(GL_ONE_MINUS_DST_COLOR,GL_ZERO);
    glLineStipple(1,3855);
    glPixelStorei(GL_PACK_ALIGNMENT,1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DITHER);
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
}

void COpenglBase::clearBuffers(float viewAngle,float orthoViewSize,float nearClippingPlane,float farClippingPlane,bool perspectiveOperation,const float* backColor)
{
    glViewport(0,0,_resX,_resY);
    glClearColor(backColor[0],backColor[1],backColor[2],0.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glRenderMode(GL_RENDER);

    float ratio=float(_resX)/float(_resY);
    if (perspectiveOperation)
    {
        if (ratio>1.0f)
        {
            float a=2.0f*(float)atan(tan(viewAngle/2.0f)/ratio)*radToDeg;
            _perspectiveSpecial(a,ratio,nearClippingPlane,farClippingPlane);
        }
        else
            _perspectiveSpecial(viewAngle*radToDeg,ratio,nearClippingPlane,farClippingPlane);
    }
    else
    {
        if (ratio>1.0f)
            glOrtho(-orthoViewSize*0.5f,orthoViewSize*0.5f,-orthoViewSize*0.5f/ratio,orthoViewSize*0.5f/ratio,nearClippingPlane,farClippingPlane);
        else
            glOrtho(-orthoViewSize*0.5f*ratio,orthoViewSize*0.5f*ratio,-orthoViewSize*0.5f,orthoViewSize*0.5f,nearClippingPlane,farClippingPlane);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void COpenglBase::_frustumSpecial(float left,float right,float bottom,float top,float nearval,float farval)
{
    float x, y, a, b, c, d;
    float m[16];
    x=(2.0f*nearval)/(right-left);
    y=(2.0f*nearval)/(top-bottom);
    a=(right+left)/(right-left);
    b=(top+bottom)/(top-bottom);
    c=-(farval+nearval)/(farval-nearval);
    d=-(2.0f*farval*nearval)/(farval-nearval);
    #define M(row,col)  m[col*4+row]
        M(0,0)=x;     M(0,1)=0.0f;  M(0,2)=a;      M(0,3)=0.0f;
        M(1,0)=0.0f;  M(1,1)=y;     M(1,2)=b;      M(1,3)=0.0f;
        M(2,0)=0.0f;  M(2,1)=0.0f;  M(2,2)=c;      M(2,3)=d;
        M(3,0)=0.0f;  M(3,1)=0.0f;  M(3,2)=-1.0f;  M(3,3)=0.0f;
    #undef M
    glMultMatrixf(m);
}

void COpenglBase::_perspectiveSpecial(float fovy,float aspect,float zNear,float zFar)
{   // Same function as gluPerspective
    float ymax=zNear*(float)tan(fovy*3.1415f/360.0f);
    float ymin=-ymax;
    float xmin=ymin*aspect;
    float xmax=ymax*aspect;
    _frustumSpecial(xmin,xmax,ymin,ymax,zNear,zFar);
}

