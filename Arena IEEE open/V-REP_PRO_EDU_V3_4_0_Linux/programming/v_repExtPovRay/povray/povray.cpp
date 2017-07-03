/****************************************************************************
 *               povray.cpp
 *
 * This module contains the entry routine for the raytracer and the code to
 * parse the parameters on the command line.
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
 * $File: //depot/povray/3.6-release/source/povray.cpp $
 * $Revision: #3 $
 * $Change: 3032 $
 * $DateTime: 2004/08/02 18:43:41 $
 * $Author: chrisc $
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

#include <ctype.h>
#include <time.h>
#include <algorithm>

#include "frame.h"
#include "bezier.h"
#include "blob.h"
#include "bbox.h"
#include "cones.h"
#include "csg.h"
#include "discs.h"
#include "express.h"
#include "fnpovfpu.h"
#include "fractal.h"
#include "hfield.h"
#include "lathe.h"
#include "lighting.h"
#include "lightgrp.h"
#include "mesh.h"
#include "photons.h"
#include "polysolv.h"
#include "objects.h"
#include "octree.h"
#include "parse.h"
#include "parstxtr.h"
#include "pigment.h"
#include "point.h"
#include "poly.h"
#include "polygon.h"
#include "povray.h"
#include "optout.h"
#include "quadrics.h"
#include "prism.h"
#include "radiosit.h"
#include "render.h"
#include "sor.h"
#include "spheres.h"
#include "super.h"
#include "targa.h"
#include "texture.h"
#include "tokenize.h"
#include "torus.h"
#include "triangle.h"
#include "truetype.h"
#include "userio.h"
#include "userdisp.h"
#include "lbuffer.h"
#include "vbuffer.h"
#include "povmsend.h"
#include "povmsrec.h"
#include "isosurf.h"
#include "sphsweep.h"
#include "pov_util.h"
#include "renderio.h"
#include "statspov.h"
#include "pov_err.h"
#include "optout.h"
#include "povms.h"
#include "rendctrl.h"
#include "platformbase.h"

#if(USE_LOCAL_POVMS_OUTPUT == 1)
    #include "defaultrenderfrontend.h"
    #include "defaultplatformbase.h"
    #include "processrenderoptions.h"

    USING_POV_FRONTEND_NAMESPACE
    USING_POV_BASE_NAMESPACE
#endif

USING_POV_NAMESPACE

BEGIN_POV_NAMESPACE

/*****************************************************************************
* Local preprocessor defines
******************************************************************************/

#ifndef POVMS_ALLOW_BINARY_MODE
    #ifdef ALTMAIN
        #define POVMS_ALLOW_BINARY_MODE 0
    #else
        #define POVMS_ALLOW_BINARY_MODE 1
    #endif
#endif


/*****************************************************************************
* Local typedefs
******************************************************************************/


/*****************************************************************************
* Global variables
******************************************************************************/

END_POV_NAMESPACE

bool Binary_POVMS_Stream_Mode = false; // GLOBAL VARIABLE

#ifdef POVRAY_COOPERATE_GLOBAL
POVRAY_COOPERATE_GLOBAL
#endif

BEGIN_POV_NAMESPACE

/*****************************************************************************
* Local variables
******************************************************************************/

// povray_init
int pre_init_flag = 0; // GLOBAL VARIABLE

// Used for povray_cooperate tricks
int Cooperate_Render_Flag = 0; // GLOBAL VARIABLE

END_POV_NAMESPACE

// Used for POVMS message receiving
POVMSContext POVMS_Render_Context = NULL; // GLOBAL VARIABLE

// Used for POVMS message sending
#if(USE_LOCAL_POVMS_OUTPUT == 1)
POVMSContext POVMS_Output_Context = NULL; // GLOBAL VARIABLE
#endif

// Platform specific function interface self reference pointer
PlatformBase *POV_BASE_NAMESPACE::PlatformBase::self = NULL; // GLOBAL VARIABLE

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// @V-REP@                                                                   //
//                                                                           //
// This entry point replaces the main function to be called directly         //
// from the ray tracer plugin with custom arguments                          //                         
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int povray_render_scene (const char* file_name, unsigned char* target_buffer, int nx, int ny, int flg)
{
    DefaultPlatformBase platformbase;

    if (! file_name || ! target_buffer || ! nx || ! ny)
        return 0;

    // Init
    povray_init();
    init_vars();

    strcpy (opts.Input_File_Name, file_name);
    opts.Preview_RefCon = target_buffer;
    Frame.Screen_Width = nx;
    Frame.Screen_Height = ny;
    opts.Output_File_Type = NO_FILE;
    opts.Options = (opts.Options | DISPLAY) & ~DISKWRITE & ~USE_VISTA_BUFFER;

    // Strip path and extension off input name to create scene name
    fix_up_scene_name();
    // Make sure clock is okay, validate animation parameters
    fix_up_animation_values();
    // Fix-up rendering window values if necessary
    fix_up_rendering_window();

    // Enter the frame loop
    FrameLoop();

    // Finish
    povray_terminate();

    return 1;
}


/*****************************************************************************
*
* FUNCTION
*
*   povray_init
*
* INPUT -- none
*
* OUTPUT
*
* RETURNS
*
* AUTHOR -- CEY
*
* DESCRIPTION
*
*   This routine does essential initialization that is required before any
*   POV_MALLOC-like routines may be called and before any text streams and
*   the POVMS may be used.
*   
*   If you are not using any built-in main and need access to any part of
*   the generic code before povray_render is called, you MUST call this routine
*   first!  Also note that it is safe to call it twice. If you don't call it,
*   povray_render will. It won't hurt if you both do it.
*   
* CHANGES
*   Nov 1995 : Created by CEY
*
******************************************************************************/

void povray_init()
{
   Stage = STAGE_PREINIT;

   if (pre_init_flag == 0)
   {
      int err;

      Cooperate_Render_Flag = 0;

      err = POVMS_OpenContext(&POVMS_Render_Context);
   }

   /* Initialize memory. */
   POV_MEM_INIT();

   pre_init_tokenizer();

   pre_init_flag = 1234;
}


/*****************************************************************************
*
* FUNCTION
*
*   povray_terminate
*
* INPUT
*   
* OUTPUT
*   
* RETURNS
*
* AUTHOR
*
*   POV-Ray Team
*   
* DESCRIPTION
*
*   -
*
* CHANGES
*
*   -
*
******************************************************************************/

void povray_terminate()
{
   close_all();
   POV_MEM_RELEASE_ALL();

   (void)POVMS_CloseContext(POVMS_Render_Context);
   pre_init_flag = 0;

#if(USE_LOCAL_POVMS_OUTPUT == 1)
    (void)POVMS_CloseContext(POVMS_Output_Context);
#endif

   FINISH_POVRAY;
}


/*****************************************************************************
*
* FUNCTION
*
*   povray_exit
*
* INPUT
*   
* OUTPUT
*   
* RETURNS
*
* AUTHOR
*
*   POV-Ray Team
*   
* DESCRIPTION
*
*   -
*
* CHANGES
*
*   -
*
******************************************************************************/

void povray_exit(int i)
{
    if(Stage == STAGE_PARSING) 
    {
        Terminate_Tokenizer();
        Destroy_Textures(Default_Texture); 
        Destroy_Camera(Default_Camera); 
    }

    pre_init_flag = 1;

    Cooperate_Render_Flag = 3;

    EXIT_POVRAY(i); /* Must call exit(i) or somehow stop */
}


/*****************************************************************************
*
* FUNCTION
*
*   povray_getoutputcontext
*
* INPUT
*
* OUTPUT
*
* RETURNS
*
* AUTHOR
*
*   POV-Ray Team
*
* DESCRIPTION
*
*   -
*
* CHANGES
*
*   -
*
******************************************************************************/

#if(USE_LOCAL_POVMS_OUTPUT == 1)
POVMSAddress povray_getoutputcontext()
{
    POVMSAddress addr = POVMSInvalidAddress;

    if(POVMS_GetContextAddress(POVMS_Output_Context, &addr) != kNoErr)
        return POVMSInvalidAddress;

    return addr;
}
#endif


