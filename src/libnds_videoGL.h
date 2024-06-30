// Some portion of libnds videoGL.h file

/*
  Copyright (C) 2005 - 2008
   Michael Noland (joat)
   Jason Rogers (dovoto)
   Dave Murpy (WinterMute)

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any
  damages arising from the use of this software.

  Permission is granted to anyone to use this software for any
  purpose, including commercial applications, and to alter it and
  redistribute it freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you
     must not claim that you wrote the original software. If you use
     this software in a product, an acknowledgment in the product
     documentation would be appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and
     must not be misrepresented as being the original software.
  3. This notice may not be removed or altered from any source
     distribution.
*/



/*! \brief Enums for setting how polygons will be displayed<BR>
<A HREF="http://problemkaputt.de/gbatek.htm#ds3dpolygonattributes">GBATEK http://problemkaputt.de/gbatek.htm#ds3dpolygonattributes</A><BR>
related functions: glPolyFmt(), glInit(), POLY_ALPHA(), POLY_ID() */
enum GL_POLY_FORMAT_ENUM {
	POLY_FORMAT_LIGHT0   = (1<<0), /*!< enable light number 0 */
	POLY_FORMAT_LIGHT1   = (1<<1), /*!< enable light number 1 */
	POLY_FORMAT_LIGHT2   = (1<<2), /*!< enable light number 2 */
	POLY_FORMAT_LIGHT3   = (1<<3), /*!< enable light number 3 */
	POLY_MODULATION      = (0<<4), /*!< enable modulation shading mode; this is the default */
	POLY_DECAL           = (1<<4), /*!< enable decal shading */
	POLY_TOON_HIGHLIGHT  = (2<<4), /*!< enable toon/highlight shading mode */
	POLY_SHADOW          = (3<<4), /*!< enable shadow shading */
	POLY_CULL_FRONT      = (1<<6), /*!< cull front polygons */
	POLY_CULL_BACK       = (2<<6), /*!< cull rear polygons */
	POLY_CULL_NONE       = (3<<6), /*!< don't cull any polygons */
	POLY_FOG             = (1<<15) /*!< enable/disable fog for this polygon */
};

/*! \fn  u32 POLY_ALPHA(u32 n)
\brief used in glPolyFmt() to set the alpha level for the following polygons, set to 0 for wireframe mode
\param n the level of alpha (0-31)
\return value for hw register*/
inline uint32_t POLY_ALPHA(uint32_t n) { return (uint32_t)((n) << 16); }