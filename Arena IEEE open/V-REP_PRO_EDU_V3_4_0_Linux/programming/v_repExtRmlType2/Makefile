CFLAGS = -I./v_repExtReflexxesTypeII -I./ReflexxesTypeII/include -I../include -Wall -fPIC
LDFLAGS = -ldl

OBJS = v_repExtReflexxesTypeII/v_repExtReflexxesTypeII.o ../common/v_repLib.o ReflexxesTypeII/src/TypeIIRML/ReflexxesAPI.o ReflexxesTypeII/src/TypeIIRML/TypeIIRMLCalculatePositionalExtrems.o ReflexxesTypeII/src/TypeIIRML/TypeIIRMLDecisions.o ReflexxesTypeII/src/TypeIIRML/TypeIIRMLDecisionTree1A.o ReflexxesTypeII/src/TypeIIRML/TypeIIRMLDecisionTree1B.o ReflexxesTypeII/src/TypeIIRML/TypeIIRMLDecisionTree1C.o ReflexxesTypeII/src/TypeIIRML/TypeIIRMLDecisionTree2.o ReflexxesTypeII/src/TypeIIRML/TypeIIRMLFallBackStrategy.o ReflexxesTypeII/src/TypeIIRML/TypeIIRMLIsPhaseSynchronizationPossible.o ReflexxesTypeII/src/TypeIIRML/TypeIIRMLPolynomial.o ReflexxesTypeII/src/TypeIIRML/TypeIIRMLPosition.o ReflexxesTypeII/src/TypeIIRML/TypeIIRMLQuicksort.o ReflexxesTypeII/src/TypeIIRML/TypeIIRMLSetupModifiedSelectionVector.o ReflexxesTypeII/src/TypeIIRML/TypeIIRMLStep1.o ReflexxesTypeII/src/TypeIIRML/TypeIIRMLStep1IntermediateProfiles.o ReflexxesTypeII/src/TypeIIRML/TypeIIRMLStep1Profiles.o ReflexxesTypeII/src/TypeIIRML/TypeIIRMLStep2.o ReflexxesTypeII/src/TypeIIRML/TypeIIRMLStep2IntermediateProfiles.o ReflexxesTypeII/src/TypeIIRML/TypeIIRMLStep2PhaseSynchronization.o ReflexxesTypeII/src/TypeIIRML/TypeIIRMLStep2Profiles.o ReflexxesTypeII/src/TypeIIRML/TypeIIRMLStep2WithoutSynchronization.o ReflexxesTypeII/src/TypeIIRML/TypeIIRMLStep3.o ReflexxesTypeII/src/TypeIIRML/TypeIIRMLVelocity.o ReflexxesTypeII/src/TypeIIRML/TypeIIRMLVelocityCalculatePositionalExtrems.o ReflexxesTypeII/src/TypeIIRML/TypeIIRMLVelocityFallBackStrategy.o ReflexxesTypeII/src/TypeIIRML/TypeIIRMLVelocityIsPhaseSynchronizationPossible.o ReflexxesTypeII/src/TypeIIRML/TypeIIRMLVelocityMethods.o ReflexxesTypeII/src/TypeIIRML/TypeIIRMLVelocitySetupPhaseSyncSelectionVector.o

OS = $(shell uname -s)
ECHO=@

ifeq ($(OS), Linux)
	CFLAGS += -D__linux
	OPTION = -shared
	EXT = so
else
	CFLAGS += -D__APPLE__
	OPTION = -dynamiclib -current_version 1.0
	EXT = dylib
endif

TARGET = libv_repExtReflexxesTypeII.$(EXT)

all: v_repExtReflexxesTypeIILib

v_repExtReflexxesTypeIILib: $(OBJS)
		@echo "Linking $(OBJS) to $(TARGET)"
		$(ECHO)$(CXX) $(CFLAGS) $(OBJS) $(OPTION) -o $(TARGET) $(LDFLAGS)

%.o: %.c
		@echo "Compiling $< to $@"
		$(ECHO)$(CXX) $(CFLAGS) -c $< -o $@

%.o: %.cpp
		@echo "Compiling $< to $@"
		$(ECHO)$(CXX) $(CFLAGS) -c $< -o $@

clean:
		@echo "Cleaning $(OBJS) $(TARGET)"
		$(ECHO)rm -rf $(OBJS) $(TARGET)
