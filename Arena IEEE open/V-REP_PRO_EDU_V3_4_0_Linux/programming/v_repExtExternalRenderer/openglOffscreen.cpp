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

#include "openglOffscreen.h"

COpenglOffscreen::COpenglOffscreen(int associatedObjectHandle,int resX,int resY) : COpenglBase(associatedObjectHandle)
{
    _resX=resX;
    _resY=resY;
    _offscreenContext=new COffscreenGlContext(resX,resY);
    _frameBufferObject=new CFrameBufferObject(resX,resY);
    _offscreenContext->doneCurrent();
    _offscreenContext->makeCurrent();
    _frameBufferObject->switchToFbo();
}

COpenglOffscreen::~COpenglOffscreen()
{
    _offscreenContext->makeCurrent();
    delete _frameBufferObject;
    _offscreenContext->doneCurrent();
    delete _offscreenContext;
}

bool COpenglOffscreen::isResolutionSame(int resX,int resY)
{
    return ((resX==_resX)&&(resY=_resY));
}

void COpenglOffscreen::initGL()
{
    COpenglBase::initGL();
}

void COpenglOffscreen::makeContextCurrent()
{
    _offscreenContext->makeCurrent();
}

void COpenglOffscreen::doneCurrentContext()
{
    _offscreenContext->doneCurrent();
}

