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

#include <render/sim_frame_buffer.h>
#include <render/sim_driver.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CFrameBuffer::CFrameBuffer( const std::string &name )
	: CTexture( name )
{
	m_name			= name;
	
	m_type			= k_Type_RGB;
	m_format		= k_Format_RGB;

	m_bufferiD		= 0;
}

// ----------------------------------------------------------------------//

CFrameBuffer::~CFrameBuffer()
{
if (m_bufferiD) {
		glDeleteFramebuffers(1, &m_bufferiD);
	}

	SIM_CHECK_OPENGL();
}

// ----------------------------------------------------------------------//

void CFrameBuffer::Generate( u32 width, u32 height )
{
	m_width		= mat::nextPowerOfTwo(width * 3 / 4);
	m_height	= mat::nextPowerOfTwo(height * 3 / 4);

	glGenFramebuffers(1, &m_bufferiD);
	glBindFramebuffer(GL_FRAMEBUFFER, m_bufferiD);

	glGenTextures( 1, &m_iD );
	CTexture::ApplyWrap(this, CTexture::k_Wrap_Clamp);
	CTexture::ApplyFilter(this, CTexture::k_Filter_Nearest);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0 );

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_iD, 0 );

#if SIM_DEBUG
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	SIM_ASSERT( status == GL_FRAMEBUFFER_COMPLETE);
#endif // SIM_DEBUG

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	SIM_CHECK_OPENGL();
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
