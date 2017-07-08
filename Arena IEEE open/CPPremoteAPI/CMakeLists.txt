cmake_minimum_required(VERSION 2.6)
project(cppremoteapi)

INCLUDE(FindPkgConfig)

IF(CMAKE_COMPILER_IS_GNUCC)
SET(CMAKE_CXX_FLAGS
"${CMAKE_CXX_FLAGS} -Wall -std=c++0x -DBUILD_SHARED_LIBS=OFF")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

# definicoes da remoteAPI para CPP
add_definitions(-DNON_MATLAB_PARSING)
add_definitions(-DMAX_EXT_API_CONNECTIONS=255)

set(SOURCES
    remoteApi/extApi.c
    #remoteApi/extApiCustom.c
    remoteApi/extApiPlatform.c
    VDC/VDC.cpp
    VDC/Skills.cpp
)

set(HEADERS
    remoteApi/extApi.h
    #remoteApi/extApiCustom.h
    remoteApi/extApiInternal.h
    remoteApi/extApiPlatform.h
    include/v_repConst.h
    include/v_repLib.h
    VDC/VDC.h
    VDC/Skills.h
    VDC/Utilities.h
    VDC/OpenCV/OCVbridgeType.h
    
)
include_directories("include/")
include_directories("remoteApi/")
include_directories("VDC/")


find_package (Threads)
find_package( OpenCV REQUIRED ) 

add_executable(cppremoteapi
               main.cpp
               ${SOURCES}
	       ${HEADERS})

target_link_libraries( cppremoteapi ${OpenCV_LIBS} )
target_link_libraries (cppremoteapi ${CMAKE_THREAD_LIBS_INIT})


install(TARGETS cppremoteapi RUNTIME DESTINATION bin)
