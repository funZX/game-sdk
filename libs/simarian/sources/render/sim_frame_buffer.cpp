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
{
	m_name			= name;
	
	m_iD			= 0;
	m_depthBuffer	= 0;
	m_texID			= 0;

	m_width			= 0;
	m_height		= 0;
}

// ----------------------------------------------------------------------//

CFrameBuffer::~CFrameBuffer()
{
	if(m_depthBuffer) {
		glDeleteRenderbuffers( 1, &m_depthBuffer);
	}

	if (m_texID) {
		glDeleteTextures(1, &m_texID);
	}

	if ( m_iD ) {
		glDeleteFramebuffers(1, &m_iD);
	}

	SIM_CHECK_OPENGL();
}

// ----------------------------------------------------------------------//

void CFrameBuffer::Generate( u32 width, u32 height )
{
	m_width		= width;
	m_height	= height;

	glGenTextures( 1, &m_texID );
	glActiveTexture(GL_TEXTURE0);
	glBindTexture( GL_TEXTURE_2D, m_texID );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
	glBindTexture( GL_TEXTURE_2D, 0 );

	glGenRenderbuffers( 1, &m_depthBuffer);
	glBindRenderbuffer( GL_RENDERBUFFER, m_depthBuffer );
	glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height );
	glBindRenderbuffer( GL_RENDERBUFFER, 0 );

	glGenFramebuffers( 1, &m_iD );
	glBindFramebuffer( GL_FRAMEBUFFER, m_iD );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texID, 0 );
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);

	SIM_ASSERT( GL_FRAMEBUFFER_COMPLETE == glCheckFramebufferStatus( GL_FRAMEBUFFER ) );

	SIM_CHECK_OPENGL();

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

// ----------------------------------------------------------------------//

void CFrameBuffer::Bind( CDriver* driver )
{
	driver->BindFrameBuffer( this );
}

// ----------------------------------------------------------------------//

void CFrameBuffer::UnBind( CDriver* driver )
{
	driver->BindFrameBuffer( NULL );
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
