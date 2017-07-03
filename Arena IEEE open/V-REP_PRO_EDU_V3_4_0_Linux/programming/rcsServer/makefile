CFLAGS = -I../include

OS = $(shell uname -s)
ifeq ($(OS), Linux)
	CFLAGS += -D__linux
else
	CFLAGS += -D__APPLE__
endif

all: 
	@rm -f bin/*.*
	@rm -f *.o 
	g++ $(CFLAGS) -c rcsServer.cpp -o rcsServer.o
	g++ $(CFLAGS) -c ../common/socketInConnection.cpp -o socketInConnection.o
	@mkdir -p bin
	g++ socketInConnection.o rcsServer.o -o bin/rcsServer -ldl
