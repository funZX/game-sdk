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

#include <core/io/sim_file_stream.h>
#include <render/sim_cube_texture.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CCubeTexture::CCubeTexture( const std::string &name )
	: CTexture( name )
{
	
}
// ----------------------------------------------------------------------//

CCubeTexture::~CCubeTexture()
{

}

// ----------------------------------------------------------------------//

u32 CCubeTexture::Generate(io::CMemStream* front, io::CMemStream* back,
						  io::CMemStream* left, io::CMemStream* right,
						  io::CMemStream* top, io::CMemStream* bottom )
{
	glGenTextures( 1, &m_iD );
	glBindTexture( GL_TEXTURE_CUBE_MAP, m_iD );

	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	u8 *buf = LoadPVR( front, &m_width, &m_height, &m_bits, &m_format );
	SIM_ASSERT( buf != NULL);

	u8 *pvr = buf + sizeof( CTexture::TPVRHeader)	;

	u32 bs = 16;
	u32 bpp = 4;
	u32 bw = m_width >> 2;
	u32 bh = m_height >> 2;

	bw = SIM_MAX( 2, bw );
	bh = SIM_MAX( 2, bh );

	u32 pvrSize = bw * bh * ( (bs  * bpp) >> 3 );

	glCompressedTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, m_format, m_width, m_height, 0, pvrSize, pvr );
	SIM_CHECK_OPENGL();

	SIM_SAFE_DELETE_ARRAY( buf );

	buf = LoadPVR( back, &m_width, &m_height, &m_bits, &m_format );
	SIM_ASSERT( buf != NULL);

	pvr = buf + sizeof( CTexture::TPVRHeader)	;

	bw = m_width >> 2;
	bh = m_height >> 2;

	bw = SIM_MAX( 2, bw );
	bh = SIM_MAX( 2, bh );

	pvrSize = bw * bh * ( (bs  * bpp) >> 3 );

	glCompressedTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, m_format, m_width, m_height, 0, pvrSize, pvr );
	SIM_CHECK_OPENGL();

	SIM_SAFE_DELETE_ARRAY( buf );

	buf = LoadPVR( left, &m_width, &m_height, &m_bits, &m_format );
	SIM_ASSERT( buf != NULL);

	pvr = buf + sizeof( CTexture::TPVRHeader)	;

	bw = m_width >> 2;
	bh = m_height >> 2;

	bw = SIM_MAX( 2, bw );
	bh = SIM_MAX( 2, bh );

	pvrSize = bw * bh * ( (bs  * bpp) >> 3 );

	glCompressedTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, m_format, m_width, m_height, 0, pvrSize, pvr );
	SIM_CHECK_OPENGL();

	SIM_SAFE_DELETE_ARRAY( buf );

	buf = LoadPVR( right, &m_width, &m_height, &m_bits, &m_format );
	SIM_ASSERT( buf != NULL);

	pvr = buf + sizeof( CTexture::TPVRHeader)	;

	bw = m_width >> 2;
	bh = m_height >> 2;

	bw = SIM_MAX( 2, bw );
	bh = SIM_MAX( 2, bh );

	pvrSize = bw * bh * ( (bs  * bpp) >> 3 );

	glCompressedTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, m_format, m_width, m_height, 0, pvrSize, pvr );
	SIM_CHECK_OPENGL();

	SIM_SAFE_DELETE_ARRAY( buf );

	buf = LoadPVR( top, &m_width, &m_height, &m_bits, &m_format );
	SIM_ASSERT( buf != NULL);

	pvr = buf + sizeof( CTexture::TPVRHeader)	;

	bw = m_width >> 2;
	bh = m_height >> 2;

	bw = SIM_MAX( 2, bw );
	bh = SIM_MAX( 2, bh );

	pvrSize = bw * bh * ( (bs  * bpp) >> 3 );

	glCompressedTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, m_format, m_width, m_height, 0, pvrSize, pvr );
	SIM_CHECK_OPENGL();

	SIM_SAFE_DELETE_ARRAY( buf );

	buf = LoadPVR( bottom, &m_width, &m_height, &m_bits, &m_format );
	SIM_ASSERT( buf != NULL);

	pvr = buf + sizeof( CTexture::TPVRHeader)	;

	bw = m_width >> 2;
	bh = m_height >> 2;

	bw = SIM_MAX( 2, bw );
	bh = SIM_MAX( 2, bh );

	pvrSize = bw * bh * ( (bs  * bpp) >> 3 );

	glCompressedTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, m_format, m_width, m_height, 0, pvrSize, pvr );
	SIM_CHECK_OPENGL();
	
	SIM_SAFE_DELETE_ARRAY( buf );

	glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );

	return m_iD;
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
