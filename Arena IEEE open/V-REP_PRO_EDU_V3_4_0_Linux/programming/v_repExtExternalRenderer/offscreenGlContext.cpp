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

#include "offscreenGlContext.h"

COffscreenGlContext::COffscreenGlContext(int resX,int resY) : QObject()
{
    _qOffscreenSurface=new QOffscreenSurface();
    QSurfaceFormat f;
    f.setSwapBehavior(QSurfaceFormat::SingleBuffer);
    f.setRenderableType(QSurfaceFormat::OpenGL);
    f.setRedBufferSize(8);
    f.setGreenBufferSize(8);
    f.setBlueBufferSize(8);
    f.setAlphaBufferSize(0);
    f.setStencilBufferSize(8);
    f.setDepthBufferSize(24);
    _qOffscreenSurface->setFormat(f);
    _qOffscreenSurface->create();
    if (_qOffscreenSurface->isValid())
    {
        _qContext=new QOpenGLContext();
        _qContext->setFormat(_qOffscreenSurface->format());
        _qContext->create();
    }

    makeCurrent();

    // Init GL:
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL); // Maybe useful with glPolygonOffset?
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
    glBlendFunc(GL_ONE_MINUS_DST_COLOR,GL_ZERO);
    glLineStipple(1,3855);
    glPixelStorei(GL_PACK_ALIGNMENT,1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DITHER);
    glEnable(GL_LIGHTING); // keep lighting on for everything, except for temporary operations.
    glShadeModel(GL_SMOOTH);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1); // Important in order to have both sides affected by lights!
}

COffscreenGlContext::~COffscreenGlContext()
{
    delete _qContext;
    _qOffscreenSurface->destroy();
    delete _qOffscreenSurface;
}

bool COffscreenGlContext::makeCurrent()
{
    _qContext->makeCurrent(_qOffscreenSurface);
    return(true);
}

bool COffscreenGlContext::doneCurrent()
{
    _qContext->doneCurrent();
    return(true);
}

