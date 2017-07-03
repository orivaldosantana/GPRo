# This file is part of the ConvexDecompose interface for V-REP
# 
# -------------------------------------------------------------------
# Copyright (c) 2011 Khaled Mamou (kmamou at gmail dot com)
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
# 
# 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
# 
# 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
# 
# 3. The names of the contributors may not be used to endorse or promote products derived from this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# -------------------------------------------------------------------

#
# This file was automatically created for V-REP release V3.4.0 rev. 1 on April 5th 2017

QT -= core
QT -= gui

TARGET = v_repExtConvexDecompose
TEMPLATE = lib

DEFINES -= UNICODE
DEFINES += QT_COMPIL
CONFIG += shared
INCLUDEPATH += "../include"
INCLUDEPATH += "../v_repMath"
INCLUDEPATH += "hacd"
INCLUDEPATH += "vhacd/inc"
INCLUDEPATH += "vhacd/public"
INCLUDEPATH += "vhacd/src"

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

win32 {
    DEFINES += WIN_VREP
}

macx {
    DEFINES += MAC_VREP
}

unix:!macx {
    DEFINES += LIN_VREP
}

SOURCES += \
    ../common/v_repLib.cpp \
    v_repExtConvexDecompose.cpp \
    hacd/hacdGraph.cpp \
    hacd/hacdHACD.cpp \
    hacd/hacdICHull.cpp \
    hacd/hacdManifoldMesh.cpp \
    hacd/hacdMeshDecimator.cpp \
    hacd/hacdMicroAllocator.cpp \
    hacd/hacdRaycastMesh.cpp \
    vhacd/src/btAlignedAllocator.cpp \
    vhacd/src/btConvexHullComputer.cpp \
    vhacd/src/VHACD.cpp \
    vhacd/src/vhacdICHull.cpp \
    vhacd/src/vhacdManifoldMesh.cpp \
    vhacd/src/vhacdMesh.cpp \
    vhacd/src/vhacdVolume.cpp \

HEADERS +=\
    ../include/v_repLib.h \
    v_repExtConvexDecompose.h \
    hacd/hacdCircularList.h \
    hacd/hacdGraph.h \
    hacd/hacdHACD.h \
    hacd/hacdICHull.h \
    hacd/hacdManifoldMesh.h \
    hacd/hacdMeshDecimator.h \
    hacd/hacdMicroAllocator.h \
    hacd/hacdRaycastMesh.h \
    hacd/hacdSArray.h \
    hacd/hacdVector.h \
    hacd/hacdVersion.h \
    vhacd/inc/btAlignedAllocator.h \
    vhacd/inc/btAlignedObjectArray.h \
    vhacd/inc/btConvexHullComputer.h \
    vhacd/inc/btMinMax.h \
    vhacd/inc/btScalar.h \
    vhacd/inc/btVector3.h \
    vhacd/public/VHACD.h \
    vhacd/inc/vhacdCircular.h \
    vhacd/inc/vhacdlCHull.h \
    vhacd/inc/vhacdManifoldMesh.h \
    vhacd/inc/vhacdMesh.h \
    vhacd/inc/vhacdMesh.h \
    vhacd/inc/vhacdMutex.h \
    vhacd/inc/vhacdSArray.h \
    vhacd/inc/vhacdTimer.h \
    vhacd/inc/vhacdVector.h \
    vhacd/inc/vhacdVHACD.h \
    vhacd/inc/vhacdVolume.h \

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
