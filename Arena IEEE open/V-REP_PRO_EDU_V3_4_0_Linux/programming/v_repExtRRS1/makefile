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
	g++ $(CFLAGS) -c v_repExtRRS1.cpp -o v_repExtRRS1.o
	g++ $(CFLAGS) -c inputOutputBlock.cpp -o inputOutputBlock.o
	g++ $(CFLAGS) -c ../common/socketOutConnection.cpp -o socketOutConnection.o
	g++ $(CFLAGS) -c ../common/scriptFunctionData.cpp -o scriptFunctionData.o
	g++ $(CFLAGS) -c ../common/scriptFunctionDataItem.cpp -o scriptFunctionDataItem.o
	g++ $(CFLAGS) -c ../common/v_repLib.cpp -o v_repLib.o
	@mkdir -p lib
	g++ v_repExtRRS1.o socketOutConnection.o scriptFunctionData.o scriptFunctionDataItem.o inputOutputBlock.o v_repLib.o -o lib/libv_repExtRRS1.$(EXT) -lpthread -ldl -shared 

