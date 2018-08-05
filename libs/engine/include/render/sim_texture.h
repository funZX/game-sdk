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

#include <core/sim_interfaces.h>
#include <render/sim_render.h>

namespace sim
{
namespace io { class CMemStream; }
namespace rnr
{
// ----------------------------------------------------------------------//

class CTexture : public IEngineItem
{
public:
	CTexture();
	CTexture(  const std::string &name );
	virtual ~CTexture();

	// ------------------------------------------------------------------//
	enum class Wrap : u32
	{
		Repeat,
		Clamp,
	};
	// ------------------------------------------------------------------//
	enum class Filter : u32
	{
		Nearest,
		Linear,
		Bilinear,
		Trilinear,
		Quadlinear,
	};
	// ------------------------------------------------------------------//
	enum class Format : u32
	{
		Alpha				=	GL_ALPHA,
		Luminance			=	GL_LUMINANCE,
		LuminanceAlpha		=	GL_LUMINANCE_ALPHA,
		RGB					=	GL_RGB,
		RGBA				=	GL_RGBA,
		RGB565				=	GL_RGB565,
		RGB5A1				=	GL_RGB5_A1,
		RGBA4				=	GL_RGBA4,
		Compressed2Bpp		=	GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG,
		Compressed4Bpp		=	GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG,
	};
	// ------------------------------------------------------------------//
	enum class Type : u32
	{
		TGA,
		PVR,
		MIP,
		RGB,
	};
	// ------------------------------------------------------------------//
	u32			            Generate( u8* buf,
                                      u32 width, u32 height,
                                      Type type, Wrap wrap, Filter filter, Format format );

	u32						Generate( io::CMemStream* memstream, Type type, Wrap wrap, Filter filter );


	static u8*	            LoadTGA( io::CMemStream* memstream, u32 *width, u32 *height, u32 *bits, Format *format );
	static u8*	            LoadPVR( io::CMemStream* memstream, u32 *width, u32 *height, u32 *bits, Format *format );
	static u8*	            LoadMIP( io::CMemStream* memstream, u32 *width, u32 *height, u32 *bits, Format *format );

	inline u32              GetID() { return m_iD; }
	inline u32              GetWidth() { return m_width; }
	inline u32              GetHeight() { return m_height; }

	static void				ApplyFilter( CTexture *tex, Filter filter );
	static void				ApplyWrap(CTexture *tex, Wrap wrap);
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

	u32			            m_width;
	u32			            m_height;
	u32			            m_bits;
	public:
	u32			            m_iD;

	Filter					m_filter;
	Wrap					m_wrap;
	Format					m_format;

	Type					m_type;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_TEXTURE_H
