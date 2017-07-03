CFLAGS =  -Wall -fPIC -static -llibluajit

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
	g++ $(CFLAGS) -c v_repLua.cpp -o v_repLua.o
	@mkdir -p lib
	g++ v_repLua.o -o lib/libv_repLua.$(EXT) -lpthread -ldl -shared 

