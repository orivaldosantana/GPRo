#PUT_VREP_REFLEXXES_COPYRIGHT_NOTICE_HERE

QT -= core
QT -= gui

TARGET = v_repExtReflexxesTypeII
TEMPLATE = lib

DEFINES -= UNICODE
DEFINES += QT_COMPIL
CONFIG += shared
INCLUDEPATH += "ReflexxesTypeII/include"
INCLUDEPATH += "v_repExtReflexxesTypeII"
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
    INCLUDEPATH += "c:/local/boost_1_62_0"
}

macx {
    DEFINES += MAC_VREP
    INCLUDEPATH += "/usr/local/include"
}

unix:!macx {
    DEFINES += LIN_VREP
    INCLUDEPATH += "../../boost_1_49_0" #probably not needed when installing boost with sudo apt-get install libboost-all-dev
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

HEADERS += \
    ../include/v_repLib.h \
    v_repExtReflexxesTypeII/v_repExtReflexxesTypeII.h \
    ReflexxesTypeII/src/TypeIIRML/ReflexxesAPI.h \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLCalculatePositionalExtrems.h \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLDecisions.h \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLDecisionTree1A.h \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLDecisionTree1B.h \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLDecisionTree1C.h \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLDecisionTree2.h \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLFallBackStrategy.h \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLIsPhaseSynchronizationPossible.h \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLPolynomial.h \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLPosition.h \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLQuicksort.h \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLSetupModifiedSelectionVector.h \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLStep1.h \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLStep1IntermediateProfiles.h \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLStep1Profiles.h \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLStep2.h \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLStep2IntermediateProfiles.h \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLStep2PhaseSynchronization.h \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLStep2Profiles.h \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLStep2WithoutSynchronization.h \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLStep3.h \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLVelocity.h \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLVelocityCalculatePositionalExtrems.h \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLVelocityFallBackStrategy.h \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLVelocityIsPhaseSynchronizationPossible.h \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLVelocityMethods.h \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLVelocitySetupPhaseSyncSelectionVector.h \

SOURCES += \
    ../common/v_repLib.cpp \
    v_repExtReflexxesTypeII/v_repExtReflexxesTypeII.cpp \
    ReflexxesTypeII/src/TypeIIRML/ReflexxesAPI.cpp \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLCalculatePositionalExtrems.cpp \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLDecisions.cpp \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLDecisionTree1A.cpp \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLDecisionTree1B.cpp \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLDecisionTree1C.cpp \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLDecisionTree2.cpp \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLFallBackStrategy.cpp \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLIsPhaseSynchronizationPossible.cpp \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLPolynomial.cpp \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLPosition.cpp \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLQuicksort.cpp \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLSetupModifiedSelectionVector.cpp \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLStep1.cpp \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLStep1IntermediateProfiles.cpp \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLStep1Profiles.cpp \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLStep2.cpp \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLStep2IntermediateProfiles.cpp \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLStep2PhaseSynchronization.cpp \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLStep2Profiles.cpp \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLStep2WithoutSynchronization.cpp \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLStep3.cpp \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLVelocity.cpp \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLVelocityCalculatePositionalExtrems.cpp \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLVelocityFallBackStrategy.cpp \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLVelocityIsPhaseSynchronizationPossible.cpp \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLVelocityMethods.cpp \
    ReflexxesTypeII/src/TypeIIRML/TypeIIRMLVelocitySetupPhaseSyncSelectionVector.cpp \

