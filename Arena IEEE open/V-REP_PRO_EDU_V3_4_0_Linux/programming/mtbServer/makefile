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
	g++ $(CFLAGS) -c mtbServer.cpp -o mtbServer.o
	g++ $(CFLAGS) -c robotLanguageInterpreter.cpp -o robotLanguageInterpreter.o
	g++ $(CFLAGS) -c ../common/socketInConnection.cpp -o socketInConnection.o
	@mkdir -p bin
	g++ robotLanguageInterpreter.o socketInConnection.o mtbServer.o -o bin/mtbServer -lpthread 
