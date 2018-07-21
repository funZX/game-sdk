/* 
*    SIM - OpenGL ES 2.0 portable game engine
*    Copyright (C) 2012  Adrian SIMINCIUC
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __SIM_TEXTURE_H
#define __SIM_TEXTURE_H

#include <render/sim_render.h>
#include <core/io/sim_mem_stream.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

class CTexture
{
public:
	CTexture(  const std::string &name );
	virtual ~CTexture();

	// ------------------------------------------------------------------//
	typedef enum
	{
		k_Wrap_Repeat,
		k_Wrap_Clamp,

		k_Wrap_Default = k_Wrap_Clamp,
	} K_WRAP;
	// ------------------------------------------------------------------//
	typedef enum
	{
		k_Filter_Nearest,
		k_Filter_Linear,
		k_Filter_Bilinear,
		k_Filter_Trilinear,
		k_Filter_Quadlinear,

		k_Filter_Default = k_Filter_Quadlinear,
	} K_FILTER;
	// ------------------------------------------------------------------//
	typedef enum
	{
		k_Format_Alpha				=	GL_ALPHA,
		k_Format_Luminance			=	GL_LUMINANCE,
		k_Format_LuminanceAlpha		=	GL_LUMINANCE_ALPHA,
		k_Format_RGB				=	GL_RGB,
		k_Format_RGBA				=	GL_RGBA,
		k_Format_RGB565				=	GL_RGB565,
		k_Format_RGB5A1				=	GL_RGB5_A1,
		k_Format_RGBA4				=	GL_RGBA4,
		k_Format_Compressed2Bpp		=	GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG,
		k_Format_Compressed4Bpp		=	GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG,

		k_Format_Default			= k_Format_RGB,
	} K_FORMAT;
	// ------------------------------------------------------------------//
	typedef enum
	{
		k_Type_TGA,
		k_Type_PVR,
		k_Type_MIP,
		
		k_Type_Default = k_Type_TGA,

	} K_TYPE;
	// ------------------------------------------------------------------//
	u32			            Generate( u8* buf,
                                      u32 width, u32 height,
                                      K_TYPE type, K_WRAP wrap, K_FILTER filter, K_FORMAT format );

	u32						Generate( io::CMemStream* memstream, K_TYPE type, K_WRAP wrap, K_FILTER filter );


	static u8*	            LoadTGA( io::CMemStream* memstream, u32 *width, u32 *height, u32 *bits, K_FORMAT *format );
	static u8*	            LoadPVR( io::CMemStream* memstream, u32 *width, u32 *height, u32 *bits, K_FORMAT *format );
	static u8*	            LoadMIP( io::CMemStream* memstream, u32 *width, u32 *height, u32 *bits, K_FORMAT *format );

	inline u32              GetID() { return m_iD; }
	inline u32              GetWidth() { return m_width; }
	inline u32              GetHeight() { return m_height; }

	inline std::string&		GetName() { return m_name; }

	static void				ApplyFilter( CTexture *tex, K_FILTER filter );
	// ------------------------------------------------------------------//

public:
	// ------------------------------------------------------------------//
	void					GenerateMipmapsPVR( u8 *buf, u32 maxLevel );
	void					GenerateMipmapsMIP( u8 *buf, u32 maxLevel );
	// ------------------------------------------------------------------//
	typedef struct
	{
		u32 dwHeaderSize;		/*!< size of the structure */
		u32 dwHeight;			/*!< height of surface to be created */
		u32 dwWidth;			/*!< width of input surface */
		u32 dwMipMapCount;		/*!< number of mip-map levels requested */
		u32 dwpfFlags;			/*!< pixel format flags */
		u32 dwTextureDataSize;	/*!< Total size in bytes */
		u32 dwBitCount;			/*!< number of bits per pixel  */
		u32 dwRBitMask;			/*!< mask for red bit */
		u32 dwGBitMask;			/*!< mask for green bits */
		u32 dwBBitMask;			/*!< mask for blue bits */
		u32 dwAlphaBitMask;		/*!< mask for alpha channel */
		u32 dwPVR;				/*!< magic number identifying pvr file */
		u32 dwNumSurfs;			/*!< the number of surfaces present in the pvr */
	} TPVRHeader;
	// ------------------------------------------------------------------//
	typedef struct
	{
		u32 dwWidth;
		u32 dwHeight;
		u32 dwBits;
		u32 dwPixelSize;
		u32 dwFormat;
		u32 dwDataSize;
		u32 dwNumMipmaps;
	} TMIPHeader;
	// ------------------------------------------------------------------//
	static const u32		MaxMipmapLevel = 16;

	std::string				m_name;

	u32			            m_width;
	u32			            m_height;
	u32			            m_bits;
	public:
	u32			            m_iD;

	K_FILTER				m_filter;
	K_WRAP					m_wrap;
	K_FORMAT			    m_format;

	K_TYPE					m_type;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_TEXTURE_H
