CFLAGS = -I../include -Wall -fPIC -static

OS = $(shell uname -s)
ifeq ($(OS), Linux)
	CFLAGS += -D__linux -DLINUX 
	EXT = so
	CFLAGS += -I/pathTo/OpenMesh-3.3/src
else
	CFLAGS += -D__APPLE__
	EXT = dylib
	CFLAGS += -I/pathTo/OpenMesh-3.3/src
endif

LDFLAGS   += -L.
LDLIBS += -lOpenMeshCore
LDLIBS += -lOpenMeshTools


all: 
	@rm -f lib/*.$(EXT)
	@rm -f *.o 
	g++ $(CFLAGS) -c v_repExtOpenMesh.cpp -o v_repExtOpenMesh.o
	g++ $(CFLAGS) -c ../common/v_repLib.cpp -o v_repLib.o
	g++ $(CFLAGS) -c ../common/scriptFunctionData.cpp -o scriptFunctionData.o
	g++ $(CFLAGS) -c ../common/scriptFunctionDataItem.cpp -o scriptFunctionDataItem.o
	@mkdir -p lib
	g++ scriptFunctionData.o scriptFunctionDataItem.o v_repExtOpenMesh.o v_repLib.o -o lib/libv_repExtOpenMesh.$(EXT) $(LDFLAGS) $(LDLIBS) -lpthread -ldl -shared 
