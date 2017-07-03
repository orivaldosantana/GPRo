QT -= core
QT -= gui

TARGET = v_repExtOpenMesh
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

SOURCES += \
    v_repExtOpenMesh.cpp \
    ../common/scriptFunctionData.cpp \
    ../common/scriptFunctionDataItem.cpp \
    ../common/v_repLib.cpp \

HEADERS +=\
    v_repExtOpenMesh.h \
    ../include/scriptFunctionData.h \
    ../include/scriptFunctionDataItem.h \
    ../include/v_repLib.h \

win32 {
    INCLUDEPATH += "e:/OpenMesh 6.3/include"
    LIBS += "e:/OpenMesh 6.3/lib/OpenMeshCore.lib"
    LIBS += "e:/OpenMesh 6.3/lib/OpenMeshTools.lib"

    DEFINES += WIN_VREP
}

macx {
    INCLUDEPATH += "/pathTo/OpenMesh-3.3/src"
    LIBS += -L.
    LIBS += -lOpenMeshCore
    LIBS += -lOpenMeshTools
    DEFINES += MAC_VREP
}

unix:!macx {
    INCLUDEPATH += "/pathTo/OpenMesh-3.3/src"
    LIBS += -L.
    LIBS += -lOpenMeshCore
    LIBS += -lOpenMeshTools
    DEFINES += LIN_VREP
}
    
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

