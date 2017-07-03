CFLAGS = -I../remoteApi -I../externalIk -I../include -DNON_MATLAB_PARSING -DMAX_EXT_API_CONNECTIONS=255

# For double-precision, add -DEXTIK_DOUBLE in line above

OS = $(shell uname -s)
ifeq ($(OS), Linux)
	CFLAGS += -D__linux
else
	CFLAGS += -D__APPLE__
endif

all: 
	@rm -f bin/*.*
	@rm -f *.o 
	g++ $(CFLAGS) -c externalIkDemo1.cpp -o externalIkDemo1.o
	gcc $(CFLAGS) -c ../remoteApi/extApi.c -o extApi.o
	gcc $(CFLAGS) -c ../remoteApi/extApiPlatform.c -o extApiPlatform.o
	g++ $(CFLAGS) -c ../externalIk/Vector.cpp -o Vector.o
	g++ $(CFLAGS) -c ../externalIk/Tt.cpp -o Tt.o
	g++ $(CFLAGS) -c ../externalIk/Registerediks.cpp -o Registerediks.o
	g++ $(CFLAGS) -c ../externalIk/ObjCont.cpp -o ObjCont.o
	g++ $(CFLAGS) -c ../externalIk/MyMath.cpp -o MyMath.o
	g++ $(CFLAGS) -c ../externalIk/MMatrix.cpp -o MMatrix.o
	g++ $(CFLAGS) -c ../externalIk/MainCont.cpp -o MainCont.o
	g++ $(CFLAGS) -c ../externalIk/ikRoutine.cpp -o ikRoutine.o
	g++ $(CFLAGS) -c ../externalIk/ikGroup.cpp -o ikGroup.o
	g++ $(CFLAGS) -c ../externalIk/ikEl.cpp -o ikEl.o
	g++ $(CFLAGS) -c ../externalIk/extIkSer.cpp -o extIkSer.o
	g++ $(CFLAGS) -c ../externalIk/extIk.cpp -o extIk.o
	g++ $(CFLAGS) -c ../externalIk/Dummy.cpp -o Dummy.o
	g++ $(CFLAGS) -c ../externalIk/Ct.cpp -o Ct.o
	g++ $(CFLAGS) -c ../externalIk/jointObject.cpp -o jointObject.o
	g++ $(CFLAGS) -c ../externalIk/7Vector.cpp -o 7Vector.o
	g++ $(CFLAGS) -c ../externalIk/6X6Matrix.cpp -o 6X6Matrix.o
	g++ $(CFLAGS) -c ../externalIk/6Vector.cpp -o 6Vector.o
	g++ $(CFLAGS) -c ../externalIk/4X4Matrix.cpp -o 4X4Matrix.o
	g++ $(CFLAGS) -c ../externalIk/4X4FullMatrix.cpp -o 4X4FullMatrix.o
	g++ $(CFLAGS) -c ../externalIk/4Vector.cpp -o 4Vector.o
	g++ $(CFLAGS) -c ../externalIk/3X3Matrix.cpp -o 3X3Matrix.o
	g++ $(CFLAGS) -c ../externalIk/3Vector.cpp -o 3Vector.o
	g++ $(CFLAGS) -c ../externalIk/3DObject.cpp -o 3DObject.o
	@mkdir -p bin
	g++ externalIkDemo1.o extApi.o extApiPlatform.o Vector.o Tt.o Registerediks.o ObjCont.o MyMath.o MMatrix.o MainCont.o ikRoutine.o ikGroup.o ikEl.o extIkSer.o extIk.o Dummy.o Ct.o jointObject.o 7Vector.o 6X6Matrix.o 6Vector.o 4X4Matrix.o 4X4FullMatrix.o 4Vector.o 3X3Matrix.o 3Vector.o 3DObject.o -o bin/externalIkDemo1 -lpthread
