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

#include <render/sim_texture.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CTexture::CTexture( const std::string &name )
{
	m_name		= name;
	m_iD		= 0;

	m_wrap		= k_Wrap_Default;
	m_filter	= k_Filter_Default;
	m_format	= k_Format_Default;
	m_type		= k_Type_Default;

	m_bits		= 0;
	m_width		= 0;
	m_height	= 0;
}

// ----------------------------------------------------------------------//

CTexture::~CTexture()
{
	if( m_iD ) {
		glDeleteTextures( 1, &m_iD );
	}
}

// ----------------------------------------------------------------------//

u8* CTexture::LoadTGA( io::CMemStream* memstream, u32 *width, u32 *height, u32 *bits, K_FORMAT *format )
{
	u8 *buf = NULL;
	u32 size;
	u8 *header;
	u32 w, h, b;

	header  = (u8*) memstream->Read( 18 );

	w		= header[ 13 ] * 256 + header[ 12 ];
	h		= header[ 15 ] * 256 + header[ 14 ];
	b		= header[ 16 ] >> 3;

	size = w * h * b;

	buf = SIM_NEW u8[ size ];

	if( header[ 2 ] == 10 || header[ 2 ] == 11 )
	{
		u32 i,
					 px_count = w * h,
					 px_index = 0,
					 by_index = 0;

		u8 chunk = 0, *bgra;

		do
		{
			chunk = *( (u8*) memstream->Read( 1 ) );

			if( chunk < 128 )
			{
				chunk++;

				i = 0;
				while( i != chunk )
				{
					SIM_MEMCPY( &buf[ by_index ], memstream->Read( b ), b );
					by_index += b;

					++i;
				}
			}
			else
			{
				chunk -= 127;

				bgra = (u8*) memstream->Read( b );

				i = 0;
				while( i != chunk )
				{
					SIM_MEMCPY( &buf[ by_index ], &bgra[ 0 ], b );
					by_index += b;

					++i;
				}
			}
			px_index += chunk;
		}
		while( px_index < px_count );
	}
	else { 
		SIM_MEMCPY( &buf[ 0 ], memstream->Read( size ), size ); 
	}

	if( b == 3 || b == 4 )
	{
		u32 i = 0;

		while( i != size )
		{
			u8 tmp = buf[ i ];

			buf[ i     ] = buf[ i + 2 ];
			buf[ i + 2 ] = tmp;

			i += b;
		}
	}

	if( !header[ 17 ] || header[ 17 ] == 8 )
	{
		// flip image
		u32 i    = 0,
				 size = w * h * b,

				 rows = w * b;

		u8 *flip = SIM_NEW u8 [ size ];

		while( i != h )
		{
			SIM_MEMCPY( flip + ( i * rows ),
					&buf[ 0 ] + ( ( ( h - i ) - 1 ) * rows ),
					rows );
			++i;
		}

		SIM_SAFE_DELETE_ARRAY( buf );

		buf = flip;
	}

	static const K_FORMAT fMap[] = { k_Format_Default, k_Format_Luminance, k_Format_LuminanceAlpha, k_Format_RGB, k_Format_RGBA };
	*format = fMap[ b ];

	*width	= w;
	*height = h;
	*bits	= b;

	return buf;
}

// ----------------------------------------------------------------------//

u8*	CTexture::LoadPVR( io::CMemStream* memstream, u32 *width, u32 *height, u32 *bits, K_FORMAT *format )
{
	static char Signature[]			= "PVR!";
	static u32*	SigU32				= (u32*) Signature;
	static const u32 FlagTypePVR2	= 24;
	static const u32 FlagTypePVR4	= 25;

    u32 formatFlags;
    u8 *buf				= ( u8* ) memstream->Read( 0 );
    TPVRHeader* header	= ( TPVRHeader* ) buf;

	if( header->dwPVR != (*SigU32) )
		return NULL;

    formatFlags = header->dwpfFlags & 0xFF;

    if( formatFlags != FlagTypePVR2 && formatFlags != FlagTypePVR4 )
		return NULL;

	*width	= header->dwWidth;
	*height = header->dwHeight;
        
	*bits	= header->dwAlphaBitMask ? 4 : 3;

    buf = ( u8* ) memstream->Drop();

	if( formatFlags == FlagTypePVR2 )
		*format = k_Format_Compressed2Bpp;
	
	if( formatFlags == FlagTypePVR4 )
		*format = k_Format_Compressed4Bpp;

	return buf;
}

// ----------------------------------------------------------------------//

u8*	CTexture::LoadMIP( io::CMemStream* memstream, u32 *width, u32 *height, u32 *bits, K_FORMAT *format )
{
    TMIPHeader *header = NULL;

	u8 *buf = (u8*) memstream->Read( 0 );
    header	= ( TMIPHeader* ) buf;

	*width	= header->dwWidth;
	*height = header->dwHeight;
	*bits	= header->dwBits;
	*format = ( K_FORMAT) header->dwFormat;

	buf		= ( u8* ) memstream->Drop();

	return buf;
}

// ----------------------------------------------------------------------//

void CTexture::GenerateMipmapsPVR( u8 *buf, u32 maxLevel )
{
	TPVRHeader *header = (TPVRHeader*) buf;
	u8 *pvr = buf + sizeof(TPVRHeader);
	u32 pvrOff = 0;
	u32 pvrLen = header->dwTextureDataSize;
	u32 pvrSize = 0;
	u32 w = m_width;
	u32 h = m_height;
	u32 bs = 0, bw = 0, bh = 0, bpp = 0;
	u32 level = 0;

	while( pvrOff < pvrLen && level < maxLevel )
	{
		if( m_format == k_Format_Compressed4Bpp )
		{
			bs = 16;
			bw = w >> 2;
			bh = h >> 2;
			bpp = 4;
		}
		else
		{
			bs = 32; // Pixel by pixel block size for 2bpp
			bw = w >> 3;
			bh = h >> 2;
			bpp = 2;
		}

		bw = SIM_MAX( 2, bw );
		bh = SIM_MAX( 2, bh );

		pvrSize = bw * bh * ( (bs  * bpp) >> 3 );

		glCompressedTexImage2D( GL_TEXTURE_2D, level, m_format, w, h, 0, pvrSize, pvr );
		SIM_CHECK_OPENGL();

		pvrOff += pvrSize;
		pvr += pvrSize;

		w = SIM_MAX( w >> 1, 1 );
		h = SIM_MAX( h >> 1, 1 );

		++level;
	}
}

// ----------------------------------------------------------------------//

void CTexture::GenerateMipmapsMIP( u8 *buf, u32 maxLevel )
{
	TMIPHeader *header = (TMIPHeader*) buf;
	u8 *mip		= buf + sizeof(TMIPHeader);
	u32 w		= m_width;
	u32 h		= m_height;

	u32 level	= 0;
	s32 mipSize = 0;
	u32 mipOff	= 0;

	K_FORMAT format	= k_Format_RGBA;
	GLenum type		= GL_UNSIGNED_BYTE;

	if( m_format == k_Format_RGB || m_format == k_Format_RGB565 )
		format = k_Format_RGB;

	if( m_format == k_Format_RGB565 )
		type = GL_UNSIGNED_SHORT_5_6_5;

	if( m_format == k_Format_RGB5A1 )
		type = GL_UNSIGNED_SHORT_5_5_5_1;

	if( m_format == k_Format_RGBA4 )
		type = GL_UNSIGNED_SHORT_4_4_4_4;

	while( level < SIM_MIN( header->dwNumMipmaps, maxLevel ) )
	{
		glTexImage2D( GL_TEXTURE_2D,
			level,
			format,
			w,
			h,
			0,
			format,
			type,
			mip );
		
		mipSize = w * h * ( header->dwPixelSize >> 3 );
		mipOff += mipSize;
		mip += mipSize;

		w = SIM_MAX( w >> 1, 1 );
		h = SIM_MAX( h >> 1, 1 );

		++level;

		SIM_CHECK_OPENGL();
	}
}

// ----------------------------------------------------------------------//

void CTexture::ApplyWrap(CTexture *tex, K_WRAP wrap)
{
	tex->m_wrap = wrap;

	glBindTexture(GL_TEXTURE_2D, tex->m_iD);

	if (tex->m_wrap == k_Wrap_Clamp)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
}
// ----------------------------------------------------------------------//

void CTexture::ApplyFilter( CTexture *tex, K_FILTER filter )
{
	tex->m_filter = filter;

	glBindTexture( GL_TEXTURE_2D, tex->m_iD );

	switch( tex->m_filter )
	{
	case k_Filter_Nearest:
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
			break;
		}

	case k_Filter_Linear:
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
		}

	case k_Filter_Bilinear:
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
			break;
		}

	case k_Filter_Trilinear:
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			break;
		}

	case k_Filter_Quadlinear:
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
			break;
		}
	}
}

// ----------------------------------------------------------------------//

u32 CTexture::Generate(   u8 *buf,
						  u32 width, u32 height,
						  K_TYPE type, K_WRAP wrap, K_FILTER filter, K_FORMAT format )
{
	SIM_ASSERT( buf != NULL );

	m_type		= type;
	m_filter	= filter;
	m_wrap		= wrap;
	m_format	= format;

	m_width		= width;
	m_height	= height;

	glGenTextures( 1, &m_iD );
	glBindTexture( GL_TEXTURE_2D, m_iD );
	CTexture::ApplyWrap(this, wrap);
	CTexture::ApplyFilter(this, filter);

	if( m_format != k_Format_Compressed2Bpp && m_format != k_Format_Compressed4Bpp )
	{
		bool isTGA = ( type == k_Type_TGA );

		if( isTGA )
		{
			glTexImage2D( GL_TEXTURE_2D,
				0,
				m_format,
				m_width,
				m_height,
				0,
				m_format,
				GL_UNSIGNED_BYTE,
				&buf[ 0 ] );

			if( m_filter != k_Filter_Nearest ) {
				glGenerateMipmap( GL_TEXTURE_2D );
			}
		}
		else {
			GenerateMipmapsMIP( buf, m_filter != k_Filter_Nearest ? MaxMipmapLevel : 1 );
		}
	}
	else {
		GenerateMipmapsPVR( buf, m_filter != k_Filter_Nearest ? MaxMipmapLevel : 1 );
	}

	glBindTexture( GL_TEXTURE_2D, 0 );

	SIM_CHECK_OPENGL();

	return m_iD;
}

// ----------------------------------------------------------------------//

u32 CTexture::Generate( io::CMemStream* memstream, K_TYPE type, K_WRAP wrap, K_FILTER filter )
{
	u8 *buf = NULL;

	switch( type )
	{
	case k_Type_TGA:
		buf = LoadTGA( memstream, &m_width, &m_height, &m_bits, &m_format );
		break;

	case k_Type_PVR:
		buf = LoadPVR( memstream, &m_width, &m_height, &m_bits, &m_format );
		break;

	case k_Type_MIP:
		buf = LoadMIP( memstream, &m_width, &m_height, &m_bits, &m_format );
		break;
	}

	if( buf )
	{
		Generate( buf, m_width, m_height, type, wrap, filter, m_format );
		SIM_SAFE_DELETE_ARRAY( buf );
	}

	return m_iD;
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
