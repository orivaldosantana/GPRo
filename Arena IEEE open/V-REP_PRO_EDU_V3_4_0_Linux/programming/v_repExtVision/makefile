CFLAGS = -I../include -Wall -fPIC -static

OS = $(shell uname -s)
ifeq ($(OS), Linux)
	CFLAGS += -D__linux
	EXT = so
else
	CFLAGS += -D__APPLE__
	EXT = dylib
endif

all: 
	@rm -f lib/*.$(EXT)
	@rm -f *.o 
	g++ $(CFLAGS) -c visionTransf.cpp -o visionTransf.o
	g++ $(CFLAGS) -c visionTransfCont.cpp -o visionTransfCont.o
	g++ $(CFLAGS) -c visionVelodyneHDL64E.cpp -o visionVelodyneHDL64E.o
	g++ $(CFLAGS) -c visionVelodyneHDL64ECont.cpp -o visionVelodyneHDL64ECont.o
	g++ $(CFLAGS) -c visionVelodyneVPL16.cpp -o visionVelodyneVPL16.o
	g++ $(CFLAGS) -c visionVelodyneVPL16Cont.cpp -o visionVelodyneVPL16Cont.o
	g++ $(CFLAGS) -c v_repExtVision.cpp -o v_repExtVision.o
	g++ $(CFLAGS) -c ../common/scriptFunctionData.cpp -o scriptFunctionData.o
	g++ $(CFLAGS) -c ../common/scriptFunctionDataItem.cpp -o scriptFunctionDataItem.o
	g++ $(CFLAGS) -c ../common/v_repLib.cpp -o v_repLib.o
	@mkdir -p lib
	g++ scriptFunctionData.o scriptFunctionDataItem.o visionTransf.o visionTransfCont.o visionVelodyneHDL64E.o visionVelodyneHDL64ECont.o  visionVelodyneVPL16.o visionVelodyneVPL16Cont.o v_repExtVision.o v_repLib.o -o lib/libv_repExtVision.$(EXT) -lpthread -ldl -shared 