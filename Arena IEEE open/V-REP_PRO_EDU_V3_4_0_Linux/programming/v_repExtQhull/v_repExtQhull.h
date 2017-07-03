// This file is part of the Qhull interface for V-REP
// 
// -------------------------------------------------------------------
// Qhull, Copyright (c) 1993-2012
// 
// C.B. Barber
// Arlington, MA
// 
// and
// 
// The National Science and Technology Research Center for
// Computation and Visualization of Geometric Structures
// (The Geometry Center)
// University of Minnesota
// 
// email: qhull at qhull dot org
// 
// This software includes Qhull from C.B. Barber and The Geometry Center.
// Qhull is copyrighted as noted above. Qhull is free software and may
// be obtained via http from www.qhull.org. It may be freely copied, modified,
// and redistributed under the following conditions:
// 
// 1. All copyright notices must remain intact in all files.
// 
// 2. A copy of this text file must be distributed along with any copies
// of Qhull that you redistribute; this includes copies that you have
// modified, or copies of programs or other software products that
// include Qhull.
// 
// 3. If you modify Qhull, you must include a notice giving the
// name of the person performing the modification, the date of
// modification, and the reason for such modification.
// 
// 4. When distributing modified versions of Qhull, or other software
// products that include Qhull, you must provide notice that the original
// source code may be obtained as noted above.
// 
// 5. There is no warranty or other guarantee of fitness for Qhull, it is
// provided solely "as is". Bug reports or fixes may be sent to
// qhull_bug at qhull dot org; the authors may or may not act on them as
// they desire.
// -------------------------------------------------------------------

//
// This file was automatically created for V-REP release V3.4.0 rev. 1 on April 5th 2017

#pragma once

#ifdef _WIN32
    #define VREP_DLLEXPORT extern "C" __declspec(dllexport)
#endif /* _WIN32 */
#if defined (__linux) || defined (__APPLE__)
    #define VREP_DLLEXPORT extern "C"
#endif /* __linux || __APPLE__ */


// The 3 required entry points of the V-REP plugin:
VREP_DLLEXPORT unsigned char v_repStart(void* reservedPointer,int reservedInt);
VREP_DLLEXPORT void v_repEnd();
VREP_DLLEXPORT void* v_repMessage(int message,int* auxiliaryData,void* customData,int* replyData);
VREP_DLLEXPORT void v_repQhull(void* data);
