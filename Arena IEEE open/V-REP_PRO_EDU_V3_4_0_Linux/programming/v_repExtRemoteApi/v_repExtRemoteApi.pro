# This file is part of the REMOTE API
# 
# Copyright 2006-2017 Coppelia Robotics GmbH. All rights reserved. 
# marc@coppeliarobotics.com
# www.coppeliarobotics.com
# 
# The REMOTE API is licensed under the terms of GNU GPL:
# 
# -------------------------------------------------------------------
# The REMOTE API is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# THE REMOTE API IS DISTRIBUTED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
# WARRANTY. THE USER WILL USE IT AT HIS/HER OWN RISK. THE ORIGINAL
# AUTHORS AND COPPELIA ROBOTICS GMBH WILL NOT BE LIABLE FOR DATA LOSS,
# DAMAGES, LOSS OF PROFITS OR ANY OTHER KIND OF LOSS WHILE USING OR
# MISUSING THIS SOFTWARE.
# 
# See the GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with the REMOTE API.  If not, see <http://www.gnu.org/licenses/>.
# -------------------------------------------------------------------
#
# This file was automatically created for V-REP release V3.4.0 rev. 1 on April 5th 2017

QT -= core
QT -= gui

TARGET = v_repExtRemoteApi
TEMPLATE = lib

DEFINES -= UNICODE
DEFINES += QT_COMPIL
CONFIG += shared
INCLUDEPATH += "../include"

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
    LIBS += -lwinmm
    LIBS += -lWs2_32
}

macx {
    DEFINES += MAC_VREP
}

unix:!macx {
    DEFINES += LIN_VREP
}

SOURCES += \
    v_repExtRemoteApi.cpp \
    confReader.cpp \
    inConnection.cpp \
    porting.cpp \
    simxCmd.cpp \
    simxConnections.cpp \
    simxContainer.cpp \
    simxSocket.cpp \
    simxUtils.cpp \
    ../common/scriptFunctionData.cpp \
    ../common/scriptFunctionDataItem.cpp \
    ../common/v_repLib.cpp \

HEADERS +=\
    v_repExtRemoteApi.h \
    confReader.h \
    inConnection.h \
    porting.h \
    simxCmd.h \
    simxConnections.h \
    simxContainer.h \
    simxSocket.h \
    simxUtils.h \
    ../include/scriptFunctionData.h \
    ../include/scriptFunctionDataItem.h \
    ../include/v_repLib.h \

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
