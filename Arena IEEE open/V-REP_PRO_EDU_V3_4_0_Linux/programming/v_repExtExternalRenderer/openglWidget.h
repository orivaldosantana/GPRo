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

#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include "openglBase.h"
#include <QGLWidget>

class COpenglWidget : public QGLWidget, public COpenglBase
{
    Q_OBJECT
public:
    explicit COpenglWidget(int associatedObjectHandle,QWidget *parent = 0);
    ~COpenglWidget();

    void initGL();
    void makeContextCurrent();
    void doneCurrentContext();

    void showAtGivenSizeAndPos(int resX,int resY,int posX,int posY);
    void getWindowResolution(int& resX,int& resY);

protected:
    void resizeEvent(QResizeEvent* rEvent);

    void paintGL();
    void paintEvent(QPaintEvent* event);
};
#endif // OPENGLWIDGET_H
