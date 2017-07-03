# This file is part of the Qhull interface for V-REP
# 
# -------------------------------------------------------------------
# Qhull, Copyright (c) 1993-2012
# 
# C.B. Barber
# Arlington, MA
# 
# and
# 
# The National Science and Technology Research Center for
# Computation and Visualization of Geometric Structures
# (The Geometry Center)
# University of Minnesota
# 
# email: qhull at qhull dot org
# 
# This software includes Qhull from C.B. Barber and The Geometry Center.
# Qhull is copyrighted as noted above. Qhull is free software and may
# be obtained via http from www.qhull.org. It may be freely copied, modified,
# and redistributed under the following conditions:
# 
# 1. All copyright notices must remain intact in all files.
# 
# 2. A copy of this text file must be distributed along with any copies
# of Qhull that you redistribute; this includes copies that you have
# modified, or copies of programs or other software products that
# include Qhull.
# 
# 3. If you modify Qhull, you must include a notice giving the
# name of the person performing the modification, the date of
# modification, and the reason for such modification.
# 
# 4. When distributing modified versions of Qhull, or other software
# products that include Qhull, you must provide notice that the original
# source code may be obtained as noted above.
# 
# 5. There is no warranty or other guarantee of fitness for Qhull, it is
# provided solely "as is". Bug reports or fixes may be sent to
# qhull_bug at qhull dot org; the authors may or may not act on them as
# they desire.
# -------------------------------------------------------------------

#
# This file was automatically created for V-REP release V3.4.0 rev. 1 on April 5th 2017

QT -= core
QT -= gui

TARGET = v_repExtQhull
TEMPLATE = lib

DEFINES -= UNICODE
DEFINES += QT_COMPIL
CONFIG += shared
INCLUDEPATH += "../include"
INCLUDEPATH += "../v_repMath"
INCLUDEPATH += "qHull"

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
    INCLUDEPATH += "/usr/local/include"
    DEFINES += MAC_VREP
}

unix:!macx {
    DEFINES += LIN_VREP
}

SOURCES += \
    ../common/scriptFunctionData.cpp \
    ../common/scriptFunctionDataItem.cpp \
    ../common/v_repLib.cpp \
    v_repExtQhull.cpp \
    ../v_repMath/MyMath.cpp \
    ../v_repMath/3Vector.cpp \
    ../v_repMath/4Vector.cpp \
    ../v_repMath/7Vector.cpp \
    ../v_repMath/3X3Matrix.cpp \
    ../v_repMath/4X4Matrix.cpp \
    qHull/userprintf_rbox.c \
    qHull/userprintf.c \
    qHull/usermem.c \
    qHull/user.c \
    qHull/stat.c \
    qHull/rboxlib.c \
    qHull/random.c \
    qHull/qset.c \
    qHull/poly2.c \
    qHull/poly.c \
    qHull/merge.c \
    qHull/mem.c \
    qHull/libqhull.c \
    qHull/io.c \
    qHull/global.c \
    qHull/geom2.c \
    qHull/geom.c \

HEADERS +=\
    ../include/scriptFunctionData.h \
    ../include/scriptFunctionDataItem.h \
    ../include/v_repLib.h \
    v_repExtQhull.h \
    ../v_repMath/MyMath.h \
    ../v_repMath/mathDefines.h \
    ../v_repMath/3Vector.h \
    ../v_repMath/4Vector.h \
    ../v_repMath/7Vector.h \
    ../v_repMath/3X3Matrix.h \
    ../v_repMath/4X4Matrix.h \
    qHull/user.h \
    qHull/stat.h \
    qHull/random.h \
    qHull/qset.h \
    qHull/qhull_a.h \
    qHull/poly.h \
    qHull/merge.h \
    qHull/mem.h \
    qHull/libqhull.h \
    qHull/io.h \
    qHull/geom.h \

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
