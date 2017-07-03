/****************************************************************************
 *               rgbafile.cpp
 *
 * This module contains the code to read and write the RBGA output file
 * format.
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
 * $File: //depot/povray/3.6-release/source/rgbafile.cpp $
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

#include "frame.h"
#include "povray.h"
#include "rgbafile.h"
#include "pov_util.h"
#include "povmsend.h"
#include "colour.h"
#include "parse.h"
#include "tokenize.h"

BEGIN_POV_NAMESPACE

/*****************************************************************************
*
* FUNCTION
*
*   Read_RGBA_Image
*
* INPUT
*   
* OUTPUT
*   
* RETURNS
*   
* AUTHOR
*
* DESCRIPTION
*
* CHANGES
*
******************************************************************************/

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// @V-REP@                                                                   //
//                                                                           //
// This function reads V-REP bitmap textures included in the source file     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void Read_RGBA_Image (IMAGE *Image)
{
  unsigned i, j;
  unsigned char* row;
  unsigned char* src;
  unsigned char tgaheader[4];
  unsigned width, height;
  IMAGE8_LINE *line_data = NULL;

  if (! parse_binary (tgaheader, 4))
    Error ("Cannot reading header of RGBA image.");

  /* Decipher the header information */

  width  = tgaheader[1] + (tgaheader[0] << 8);
  height = tgaheader[3] + (tgaheader[2] << 8);

  Image->iwidth  = width;
  Image->iheight = height;
  Image->width   = (DBL)width;
  Image->height  = (DBL)height;
  Image->Colour_Map_Size = 0;
  Image->Colour_Map = NULL;

  Image->data.rgb8_lines = (IMAGE8_LINE *)POV_MALLOC(height * sizeof(IMAGE8_LINE), "RGBA image");

  for (i = 0; i < height; i++)
  {
     line_data = &Image->data.rgb8_lines[i];

     line_data->red    = (unsigned char *)POV_MALLOC(width, "RGBA image line");
     line_data->green  = (unsigned char *)POV_MALLOC(width, "RGBA image line");
     line_data->blue   = (unsigned char *)POV_MALLOC(width, "RGBA image line");
    line_data->transm = (unsigned char *)POV_MALLOC(width, "RGBA image line");
  }

  /* Read the image into the buffer */
  
    row = (unsigned char *)POV_MALLOC(width * 4, "RGBA image line");

    for (i = 0, line_data = &Image->data.rgb8_lines[0]; i < height; i++, line_data++)
    {
        if (! parse_binary (row, width * 4))
          Error ("Cannot read row of RGBA image.");
          
        for (j = 0, src = row; j < width; j++)
        {
            line_data->red[j]    = *src++;
            line_data->green[j]  = *src++;
            line_data->blue[j]   = *src++;
            line_data->transm[j] = 255 - *src++;
        }
    }

  /* Close the image file */

    POV_FREE (row);
}

END_POV_NAMESPACE
