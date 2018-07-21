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
	: CTexture(name)
{
	m_name			= name;
	
	m_type			= k_Type_RGB;
	m_format		= k_Format_RGBA;
	m_filter		= k_Filter_Nearest;
	m_wrap			= k_Wrap_Clamp;

	m_renderiD		= 0;
	m_bufferiD		= 0;
	m_depthiD		= 0;
}

// ----------------------------------------------------------------------//

CFrameBuffer::~CFrameBuffer()
{
	if (m_depthiD) {
		glDeleteRenderbuffers(1, &m_depthiD);
	}

	if(m_renderiD) {
		glDeleteRenderbuffers( 1, &m_renderiD);
	}

	if (m_bufferiD) {
		glDeleteFramebuffers(1, &m_bufferiD);
	}

	SIM_CHECK_OPENGL();
}

// ----------------------------------------------------------------------//

void CFrameBuffer::Generate( u32 width, u32 height )
{
	m_width		= width;
	m_height	= height;

	glGenTextures( 1, &m_iD );

	CTexture::ApplyFilter(this, m_filter);

	glBindTexture( GL_TEXTURE_2D, m_iD);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0 );
	glBindTexture( GL_TEXTURE_2D, 0 );

	glGenRenderbuffers( 1, &m_renderiD);
	glBindRenderbuffer( GL_RENDERBUFFER, m_renderiD);
	glRenderbufferStorage( GL_RENDERBUFFER, GL_RGB565, width, height );
	glBindRenderbuffer( GL_RENDERBUFFER, 0 );

	glGenRenderbuffers(1, &m_depthiD);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthiD);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenFramebuffers( 1, &m_bufferiD );
	glBindFramebuffer( GL_FRAMEBUFFER, m_bufferiD);
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_iD, 0 );
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_renderiD);
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthiD );

#if SIM_DEBUG
	GLint format = 0, type = 0;
	glGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_FORMAT, &format);
	SIM_ASSERT(format == GL_RGB);

	glGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_TYPE, &type);
	SIM_ASSERT(type == GL_UNSIGNED_BYTE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	SIM_ASSERT( status == GL_FRAMEBUFFER_COMPLETE);
#endif // SIM_DEBUG

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	SIM_CHECK_OPENGL();
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
