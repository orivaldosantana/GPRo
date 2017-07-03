CFLAGS = -I../include -I../include/stack -Wall -fPIC -static

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
	g++ $(CFLAGS) -c v_repExtPluginSkeleton.cpp -o v_repExtPluginSkeleton.o
	g++ $(CFLAGS) -c ../common/stack/stackBool.cpp -o stackBool.o
	g++ $(CFLAGS) -c ../common/stack/stackNull.cpp -o stackNull.o
	g++ $(CFLAGS) -c ../common/stack/stackNumber.cpp -o stackNumber.o
	g++ $(CFLAGS) -c ../common/stack/stackString.cpp -o stackString.o
	g++ $(CFLAGS) -c ../common/stack/stackArray.cpp -o stackArray.o
	g++ $(CFLAGS) -c ../common/stack/stackMap.cpp -o stackMap.o
	g++ $(CFLAGS) -c ../common/stack/stackObject.cpp -o stackObject.o
	g++ $(CFLAGS) -c ../common/v_repLib.cpp -o v_repLib.o
	@mkdir -p lib
	g++ stackBool.o stackNull.o stackNumber.o stackString.o stackArray.o stackMap.o stackObject.o v_repExtPluginSkeleton.o v_repLib.o -o lib/libv_repExtPluginSkeleton.$(EXT) -lpthread -ldl -shared 

