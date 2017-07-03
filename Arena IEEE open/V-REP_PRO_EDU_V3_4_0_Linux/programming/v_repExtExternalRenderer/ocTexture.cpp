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

#include "ocTexture.h"
#include <qopengl.h>

#define TEXTURE_INIT_USED_COUNT 10

COcTexture::COcTexture(int id,const unsigned char* textureBuff,int textureSizeX,int textureSizeY)
{
    _id=id;
    _textureSizeX=textureSizeX;
    _textureSizeY=textureSizeY;
    for (int i=0;i<4*textureSizeX*textureSizeY;i++)
        _textureBuff.push_back(textureBuff[i]);
    glGenTextures(1,&_textureName);
    _usedCount=TEXTURE_INIT_USED_COUNT;
}

COcTexture::~COcTexture()
{
    glDeleteTextures(1,&_textureName);
}

void COcTexture::decrementUsedCount()
{
    _usedCount--;
}

int COcTexture::getUsedCount()
{
    return(_usedCount);
}

int COcTexture::getId()
{
    return(_id);
}

void COcTexture::startTexture(bool repeatU,bool repeatV,bool interpolateColors,int applyMode)
{
    _usedCount=TEXTURE_INIT_USED_COUNT;

    glBindTexture(GL_TEXTURE_2D,_textureName);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,_textureSizeX,_textureSizeY,0,GL_RGBA,GL_UNSIGNED_BYTE,&_textureBuff[0]);
    GLint colInt=GL_NEAREST;
    if (interpolateColors)
        colInt=GL_LINEAR;
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // keep to GL_LINEAR here!!
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,colInt);
    GLint repS=GL_CLAMP;
    if (repeatU)
        repS=GL_REPEAT;
    glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,repS);
    GLint repT=GL_CLAMP;
    if (repeatV)
        repT=GL_REPEAT;
    glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,repT);
    GLint dec=GL_MODULATE;
    if (applyMode==1)
        dec=GL_DECAL;
    if (applyMode==2)
        dec=GL_ADD;
    if (applyMode==3)
        dec=GL_BLEND;
    glTexEnvi (GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,dec);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,_textureName);
    glColor3f(1.0f,1.0f,1.0f);
}

void COcTexture::endTexture()
{
    glDisable(GL_TEXTURE_2D);
}

