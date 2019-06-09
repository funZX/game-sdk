/*
*
*    MIT License
*
*    SIM - OpenGL ES 2.0 portable game engine
*    Copyright (C) 2012  Adrian SIMINCIUC
*
*    Permission is hereby granted, free of charge, to any person obtaining a copy
*    of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in all
*    copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*    SOFTWARE.
*/

#include <core/io/sim_file_stream.h>
#include <core/io/sim_mem_stream.h>

#include <render/sim_cube_texture.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CCubeTexture::CCubeTexture()
	: CTexture()
{
}

// ----------------------------------------------------------------------//

CCubeTexture::CCubeTexture(const std::string &name)
	: CCubeTexture()
{
	m_name = name;
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
	SIM_ASSERT( buf != nullptr);

	u8 *pvr = buf + sizeof( CTexture::TPVRHeader)	;

	u32 bs = 16;
	u32 bpp = 4;
	u32 bw = m_width >> 2;
	u32 bh = m_height >> 2;

	bw = zpl_max( 2, bw );
	bh = zpl_max( 2, bh );

	u32 pvrSize = bw * bh * ( (bs  * bpp) >> 3 );

	glCompressedTexImage2D( 
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 
		0, 
		Value(m_format),
		m_width, 
		m_height,
		0, 
		pvrSize,
		pvr 
	);

	SIM_CHECK_OPENGL();
	
	buf = LoadPVR( back, &m_width, &m_height, &m_bits, &m_format );
	SIM_ASSERT( buf != nullptr);

	pvr = buf + sizeof( CTexture::TPVRHeader)	;

	bw = m_width >> 2;
	bh = m_height >> 2;

	bw = zpl_max( 2, bw );
	bh = zpl_max( 2, bh );

	pvrSize = bw * bh * ( (bs  * bpp) >> 3 );

	glCompressedTexImage2D( 
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 
		0,
		Value(m_format),
		m_width,
		m_height,
		0, 
		pvrSize,
		pvr
	);

	SIM_CHECK_OPENGL();
	
	buf = LoadPVR( left, &m_width, &m_height, &m_bits, &m_format );
	SIM_ASSERT( buf != nullptr);

	pvr = buf + sizeof( CTexture::TPVRHeader)	;

	bw = m_width >> 2;
	bh = m_height >> 2;

	bw = zpl_max( 2, bw );
	bh = zpl_max( 2, bh );

	pvrSize = bw * bh * ( (bs  * bpp) >> 3 );

	glCompressedTexImage2D( 
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		0, 
		Value(m_format),
		m_width,
		m_height,
		0,
		pvrSize, 
		pvr 
	);

	SIM_CHECK_OPENGL();

	buf = LoadPVR( right, &m_width, &m_height, &m_bits, &m_format );
	SIM_ASSERT( buf != nullptr);

	pvr = buf + sizeof( CTexture::TPVRHeader)	;

	bw = m_width >> 2;
	bh = m_height >> 2;

	bw = zpl_max( 2, bw );
	bh = zpl_max( 2, bh );

	pvrSize = bw * bh * ( (bs  * bpp) >> 3 );

	glCompressedTexImage2D(
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		0,
		Value(m_format),
		m_width, 
		m_height,
		0,
		pvrSize, 
		pvr
	);

	SIM_CHECK_OPENGL();

	buf = LoadPVR( top, &m_width, &m_height, &m_bits, &m_format );
	SIM_ASSERT( buf != nullptr);

	pvr = buf + sizeof( CTexture::TPVRHeader)	;

	bw = m_width >> 2;
	bh = m_height >> 2;

	bw = zpl_max( 2, bw );
	bh = zpl_max( 2, bh );

	pvrSize = bw * bh * ( (bs  * bpp) >> 3 );

	glCompressedTexImage2D( 
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 
		0, 
		Value(m_format),
		m_width, 
		m_height, 
		0,
		pvrSize,
		pvr 
	);

	SIM_CHECK_OPENGL();

	buf = LoadPVR( bottom, &m_width, &m_height, &m_bits, &m_format );
	SIM_ASSERT( buf != nullptr);

	pvr = buf + sizeof( CTexture::TPVRHeader)	;

	bw = m_width >> 2;
	bh = m_height >> 2;

	bw = zpl_max( 2, bw );
	bh = zpl_max( 2, bh );

	pvrSize = bw * bh * ( (bs  * bpp) >> 3 );

	glCompressedTexImage2D( 
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		0, 
		Value(m_format),
		m_width,
		m_height,
		0,
		pvrSize, 
		pvr 
	);

	SIM_CHECK_OPENGL();

	glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );

	return m_iD;
}
// ----------------------------------------------------------------------//
bool CCubeTexture::Load(io::CMemStream* ms)
{
    return false;
}
// ----------------------------------------------------------------------//
bool CCubeTexture::Save(io::CMemStream* ms)
{
    return false;
}
// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
