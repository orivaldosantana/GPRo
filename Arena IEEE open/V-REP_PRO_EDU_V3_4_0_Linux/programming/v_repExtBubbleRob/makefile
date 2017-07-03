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
	g++ $(CFLAGS) -c v_repExtBubbleRob.cpp -o v_repExtBubbleRob.o
	g++ $(CFLAGS) -c ../common/scriptFunctionData.cpp -o scriptFunctionData.o
	g++ $(CFLAGS) -c ../common/scriptFunctionDataItem.cpp -o scriptFunctionDataItem.o
	g++ $(CFLAGS) -c ../common/v_repLib.cpp -o v_repLib.o
	@mkdir -p lib
	g++ scriptFunctionData.o scriptFunctionDataItem.o v_repExtBubbleRob.o v_repLib.o -o lib/libv_repExtBubbleRob.$(EXT) -lpthread -ldl -shared 
