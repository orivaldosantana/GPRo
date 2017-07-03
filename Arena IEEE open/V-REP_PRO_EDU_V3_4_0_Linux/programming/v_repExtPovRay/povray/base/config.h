/****************************************************************************
 *               configbase.h
 *
 * This module contains all defines, typedefs, and prototypes for the base layer.
 *
 * from Persistence of Vision(tm) Ray Tracer version 3.6.
 * Copyright 1991-2003 Persistence of Vision Team
 * Copyright 2003-2004 Persistence of Vision Raytracer Pty. Ltd.
 *---------------------------------------------------------------------------
 * NOTICE: This source code file is provided so that users may experiment
 * with enhancements to POV-Ray and to port the software to platforms other
 * than those supported by the POV-Ray developers. There are strict rules
 * regarding how you are permitted to use this file. These rules are contained
 * in the distribution and derivative versions licenses which should have been
 * provided with this file.
 *
 * These licences may be found online, linked from the end-user license
 * agreement that is located at http://www.povray.org/povlegal.html
 *---------------------------------------------------------------------------
 * This program is based on the popular DKB raytracer version 2.12.
 * DKBTrace was originally written by David K. Buck.
 * DKBTrace Ver 2.0-2.12 were written by David K. Buck & Aaron A. Collins.
 *---------------------------------------------------------------------------
 * $File: //depot/povray/3.6-release/source/base/configbase.h $
 * $Revision: #2 $
 * $Change: 2939 $
 * $DateTime: 2004/07/04 13:43:26 $
 * $Author: root $
 * $Log$
 *****************************************************************************/

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Modified for the V-REP Ray-Tracer Plugin (see @V-REP@ for changes)        //
//                                                                           //
// Author:   Andreu Vidal Bramfeld-Software (bramfeld@diogen.de)             //
// Date:     2015-07-27                                                      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#ifndef PLATFORM_CONFIG_H
#define PLATFORM_CONFIG_H

#ifdef _WIN32
    #include <io.h>
#endif

#if defined (__linux) || defined (__APPLE__)
    #include <unistd.h>
#endif

#include <sys/types.h>
#include <stdio.h>

#define DONT_SHOW_IMAGE_LIB_VERSIONS
#define POVRAY_BEGIN_COOPERATE
#define POVRAY_END_COOPERATE
#define Do_Cooperate(n)

#define DBL double

#define SYS_IMAGE_HEADER  "rgbafile.h"
#define READ_SYS_IMAGE    Read_RGBA_Image

#define DEFAULT_ITEXTSTREAM_BUFFER_SIZE  4096

#define POV_DISPLAY_INIT(ref,w,h)  ((ref) != 0 && (w) != 0 && (h) != 0)

#define POV_DISPLAY_FINISHED(ref)

#define POV_DISPLAY_CLOSE(ref)

#define POV_DISPLAY_PLOT_ROW(ref,w,y,s,e,r,g,b,a)

#define POV_DISPLAY_PLOT_RECT(ref,x1,y1,x2,y2,r,g,b,a)

#define POV_DISPLAY_PLOT_BOX(ref,x1,y1,x2,y2,r,g,b,a)

#define POV_DISPLAY_PLOT(ref,x,y,r,g,b,a) \
    {\
    unsigned char* trg = ref + ((y) * Frame.Screen_Width + (x)) * 3; \
    trg[0] = (r); \
    trg[1] = (g); \
    trg[2] = (b); \
    }

#endif // PLATFORM_CONFIG_H
