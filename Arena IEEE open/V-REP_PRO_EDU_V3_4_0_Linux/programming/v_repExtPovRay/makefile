CFLAGS = -Wall -fPIC -static

OS = $(shell uname -s)
ifeq ($(OS), Linux)
	CFLAGS += -D__linux -DLIN_VREP
	EXT = so
else
	CFLAGS += -D__APPLE__ -DMAC_VREP
	EXT = dylib
endif

CFLAGS += -I../include 
CFLAGS += -I../v_repMath 
CFLAGS += -Ipovray 
CFLAGS += -Ipovray/base 
CFLAGS += -Ipovray/frontend 

all: 
	@rm -f lib/*.$(EXT)
	@rm -f *.o 

	g++ $(CFLAGS) -c ../common/v_repLib.cpp -o v_repLib.o
	g++ $(CFLAGS) -c v_repExtPovRay.cpp -o v_repExtPovRay.o
	g++ $(CFLAGS) -c ../v_repMath/MyMath.cpp -o MyMath.o
	g++ $(CFLAGS) -c ../v_repMath/3Vector.cpp -o 3Vector.o
	g++ $(CFLAGS) -c ../v_repMath/4Vector.cpp -o 4Vector.o
	g++ $(CFLAGS) -c ../v_repMath/6Vector.cpp -o 6Vector.o
	g++ $(CFLAGS) -c ../v_repMath/7Vector.cpp -o 7Vector.o
	g++ $(CFLAGS) -c ../v_repMath/3X3Matrix.cpp -o 3X3Matrix.o
	g++ $(CFLAGS) -c ../v_repMath/4X4Matrix.cpp -o 4X4Matrix.o
	g++ $(CFLAGS) -c ../v_repMath/6X6Matrix.cpp -o 6X6Matrix.o
	g++ $(CFLAGS) -c povray/base/fileinputoutput.cpp -o fileinputoutput.o
	g++ $(CFLAGS) -c povray/base/povms.cpp -o povms.o
	g++ $(CFLAGS) -c povray/base/povmscpp.cpp -o povmscpp.o
	g++ $(CFLAGS) -c povray/base/processoptions.cpp -o processoptions.o
	g++ $(CFLAGS) -c povray/base/stringutilities.cpp -o stringutilities.o
	g++ $(CFLAGS) -c povray/base/textstream.cpp -o textstream.o
	g++ $(CFLAGS) -c povray/frontend/defaultplatformbase.cpp -o defaultplatformbase.o
	g++ $(CFLAGS) -c povray/frontend/processrenderoptions.cpp -o processrenderoptions.o
	g++ $(CFLAGS) -c povray/atmosph.cpp -o atmosph.o
	g++ $(CFLAGS) -c povray/bbox.cpp -o bbox.o
	g++ $(CFLAGS) -c povray/bcyl.cpp -o bcyl.o
	g++ $(CFLAGS) -c povray/benchmark.cpp -o benchmark.o
	g++ $(CFLAGS) -c povray/bezier.cpp -o bezier.o
	g++ $(CFLAGS) -c povray/blob.cpp -o blob.o
	g++ $(CFLAGS) -c povray/boxes.cpp -o boxes.o
	g++ $(CFLAGS) -c povray/bsphere.cpp -o bsphere.o
	g++ $(CFLAGS) -c povray/camera.cpp -o camera.o
	g++ $(CFLAGS) -c povray/chi2.cpp -o chi2.o
	g++ $(CFLAGS) -c povray/colour.cpp -o colour.o
	g++ $(CFLAGS) -c povray/colutils.cpp -o colutils.o
	g++ $(CFLAGS) -c povray/cones.cpp -o cones.o
	g++ $(CFLAGS) -c povray/csg.cpp -o csg.o
	g++ $(CFLAGS) -c povray/discs.cpp -o discs.o
	g++ $(CFLAGS) -c povray/express.cpp -o express.o
	g++ $(CFLAGS) -c povray/fncode.cpp -o fncode.o
	g++ $(CFLAGS) -c povray/fnintern.cpp -o fnintern.o
	g++ $(CFLAGS) -c povray/fnpovfpu.cpp -o fnpovfpu.o
	g++ $(CFLAGS) -c povray/fnsyntax.cpp -o fnsyntax.o
	g++ $(CFLAGS) -c povray/fpmetric.cpp -o fpmetric.o
	g++ $(CFLAGS) -c povray/fractal.cpp -o fractal.o
	g++ $(CFLAGS) -c povray/function.cpp -o function.o
	g++ $(CFLAGS) -c povray/hcmplx.cpp -o hcmplx.o
	g++ $(CFLAGS) -c povray/hfield.cpp -o hfield.o
	g++ $(CFLAGS) -c povray/histogra.cpp -o histogra.o
	g++ $(CFLAGS) -c povray/image.cpp -o image.o
	g++ $(CFLAGS) -c povray/interior.cpp -o interior.o
	g++ $(CFLAGS) -c povray/isosurf.cpp -o isosurf.o
	g++ $(CFLAGS) -c povray/lathe.cpp -o lathe.o
	g++ $(CFLAGS) -c povray/lbuffer.cpp -o lbuffer.o
	g++ $(CFLAGS) -c povray/lightgrp.cpp -o lightgrp.o
	g++ $(CFLAGS) -c povray/lighting.cpp -o lighting.o
	g++ $(CFLAGS) -c povray/mathutil.cpp -o mathutil.o
	g++ $(CFLAGS) -c povray/matrices.cpp -o matrices.o
	g++ $(CFLAGS) -c povray/media.cpp -o media.o
	g++ $(CFLAGS) -c povray/mesh.cpp -o mesh.o
	g++ $(CFLAGS) -c povray/normal.cpp -o normal.o
	g++ $(CFLAGS) -c povray/objects.cpp -o objects.o
	g++ $(CFLAGS) -c povray/octree.cpp -o octree.o
	g++ $(CFLAGS) -c povray/parse.cpp -o parse.o
	g++ $(CFLAGS) -c povray/parsestr.cpp -o parsestr.o
	g++ $(CFLAGS) -c povray/parstxtr.cpp -o parstxtr.o
	g++ $(CFLAGS) -c povray/pattern.cpp -o pattern.o
	g++ $(CFLAGS) -c povray/photons.cpp -o photons.o
	g++ $(CFLAGS) -c povray/pigment.cpp -o pigment.o
	g++ $(CFLAGS) -c povray/planes.cpp -o planes.o
	g++ $(CFLAGS) -c povray/point.cpp -o point.o
	g++ $(CFLAGS) -c povray/poly.cpp -o poly.o
	g++ $(CFLAGS) -c povray/polygon.cpp -o polygon.o
	g++ $(CFLAGS) -c povray/polysolv.cpp -o polysolv.o
	g++ $(CFLAGS) -c povray/pov_mem.cpp -o pov_mem.o
	g++ $(CFLAGS) -c povray/pov_util.cpp -o pov_util.o
	g++ $(CFLAGS) -c povray/povmsend.cpp -o povmsend.o
	g++ $(CFLAGS) -c povray/povmsrec.cpp -o povmsrec.o
	g++ $(CFLAGS) -c povray/povray.cpp -o povray.o
	g++ $(CFLAGS) -c povray/prism.cpp -o prism.o
	g++ $(CFLAGS) -c povray/quadrics.cpp -o quadrics.o
	g++ $(CFLAGS) -c povray/quatern.cpp -o quatern.o
	g++ $(CFLAGS) -c povray/rad_data.cpp -o rad_data.o
	g++ $(CFLAGS) -c povray/radiosit.cpp -o radiosit.o
	g++ $(CFLAGS) -c povray/ray.cpp -o ray.o
	g++ $(CFLAGS) -c povray/rendctrl.cpp -o rendctrl.o
	g++ $(CFLAGS) -c povray/render.cpp -o render.o
	g++ $(CFLAGS) -c povray/sor.cpp -o sor.o
	g++ $(CFLAGS) -c povray/spheres.cpp -o spheres.o
	g++ $(CFLAGS) -c povray/sphsweep.cpp -o sphsweep.o
	g++ $(CFLAGS) -c povray/splines.cpp -o splines.o
	g++ $(CFLAGS) -c povray/statspov.cpp -o statspov.o
	g++ $(CFLAGS) -c povray/super.cpp -o super.o
	g++ $(CFLAGS) -c povray/texture.cpp -o texture.o
	g++ $(CFLAGS) -c povray/tokenize.cpp -o tokenize.o
	g++ $(CFLAGS) -c povray/torus.cpp -o torus.o
	g++ $(CFLAGS) -c povray/triangle.cpp -o triangle.o
	g++ $(CFLAGS) -c povray/truetype.cpp -o truetype.o
	g++ $(CFLAGS) -c povray/vbuffer.cpp -o vbuffer.o
	g++ $(CFLAGS) -c povray/vlbuffer.cpp -o vlbuffer.o
	g++ $(CFLAGS) -c povray/warps.cpp -o warps.o
	g++ $(CFLAGS) -c povray/renderio.cpp -o renderio.o
	g++ $(CFLAGS) -c povray/rgbafile.cpp -o rgbafile.o
	g++ $(CFLAGS) -c povray/optout.cpp -o optout.o
	g++ $(CFLAGS) -c povray/userio.cpp -o userio.o
	g++ $(CFLAGS) -c povray/base/textstreambuffer.cpp -o textstreambuffer.o

	@mkdir -p lib
	g++ *.o -o lib/libv_repExtPovRay.$(EXT) -lpthread -ldl -shared 

