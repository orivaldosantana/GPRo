# This file is part of the EXTERNAL RENDERER PLUGIN for V-REP
# 
# Copyright 2006-2017 Coppelia Robotics GmbH. All rights reserved. 
# marc@coppeliarobotics.com
# www.coppeliarobotics.com
# 
# The EXTERNAL RENDERER PLUGIN is licensed under the terms of GNU GPL:
# 
# -------------------------------------------------------------------
# The EXTERNAL RENDERER PLUGIN is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# THE EXTERNAL RENDERER PLUGIN IS DISTRIBUTED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
# WARRANTY. THE USER WILL USE IT AT HIS/HER OWN RISK. THE ORIGINAL
# AUTHORS AND COPPELIA ROBOTICS GMBH WILL NOT BE LIABLE FOR DATA LOSS,
# DAMAGES, LOSS OF PROFITS OR ANY OTHER KIND OF LOSS WHILE USING OR
# MISUSING THIS SOFTWARE.
#  
# See the GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with the EXTERNAL RENDERER PLUGIN.  If not, see <http://www.gnu.org/licenses/>.
# -------------------------------------------------------------------
#
# This file was automatically created for V-REP release V3.4.0 rev. 1 on April 5th 2017

TARGET = v_repExtExternalRenderer
TEMPLATE = lib
DEFINES -= UNICODE
CONFIG += shared
QT     += widgets opengl printsupport #printsupport required from MacOS, otherwise crashes strangely ('This CONFIG += shared

win32 {
    DEFINES += WIN_VREP
    greaterThan(QT_MAJOR_VERSION,4) {
        greaterThan(QT_MINOR_VERSION,4) {
            LIBS += -lopengl32
        }
    }
}

macx {
    INCLUDEPATH += "/usr/local/include"
    DEFINES += MAC_VREP
}

unix:!macx {
    DEFINES += LIN_VREP
}


*-msvc* {
        QMAKE_CXXFLAGS += -O2
        QMAKE_CXXFLAGS += -W3
}
*-g++* {
        QMAKE_CXXFLAGS += -O3
        QMAKE_CXXFLAGS += -Wall
        QMAKE_CXXFLAGS += -Wno-unused-parameter
        QMAKE_CXXFLAGS += -Wno-strict-aliasing
        QMAKE_CXXFLAGS += -Wno-empty-body
        QMAKE_CXXFLAGS += -Wno-write-strings

        QMAKE_CXXFLAGS += -Wno-unused-but-set-variable
        QMAKE_CXXFLAGS += -Wno-unused-local-typedefs
        QMAKE_CXXFLAGS += -Wno-narrowing

        QMAKE_CFLAGS += -O3
        QMAKE_CFLAGS += -Wall
        QMAKE_CFLAGS += -Wno-strict-aliasing
        QMAKE_CFLAGS += -Wno-unused-parameter
        QMAKE_CFLAGS += -Wno-unused-but-set-variable
        QMAKE_CFLAGS += -Wno-unused-local-typedefs
}

INCLUDEPATH += "../include"
INCLUDEPATH += "../v_repMath"

SOURCES += \
    ../common/v_repLib.cpp \
    v_repExtExternalRenderer.cpp \
    openglWidget.cpp \
    frameBufferObject.cpp \
    offscreenGlContext.cpp \
    openglOffscreen.cpp \
    openglBase.cpp \
    ocMesh.cpp \
    ocMeshContainer.cpp \
    ocTexture.cpp \
    ocTextureContainer.cpp \
    ../v_repMath/MyMath.cpp \
    ../v_repMath/3Vector.cpp \
    ../v_repMath/4Vector.cpp \
    ../v_repMath/6Vector.cpp \
    ../v_repMath/7Vector.cpp \
    ../v_repMath/3X3Matrix.cpp \
    ../v_repMath/4X4Matrix.cpp \
    ../v_repMath/6X6Matrix.cpp \

HEADERS +=\
    ../include/v_repLib.h \
    v_repExtExternalRenderer.h \
    openglWidget.h \
    frameBufferObject.h \
    offscreenGlContext.h \
    openglOffscreen.h \
    openglBase.h \
    ocMesh.h \
    ocMeshContainer.h \
    ocTexture.h \
    ocTextureContainer.h \
    ../v_repMath/MyMath.h \
    ../v_repMath/mathDefines.h \
    ../v_repMath/3Vector.h \
    ../v_repMath/4Vector.h \
    ../v_repMath/6Vector.h \
    ../v_repMath/7Vector.h \
    ../v_repMath/3X3Matrix.h \
    ../v_repMath/4X4Matrix.h \
    ../v_repMath/6X6Matrix.h \

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}



















