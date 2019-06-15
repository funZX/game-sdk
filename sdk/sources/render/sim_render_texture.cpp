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

#include <render/sim_render_texture.h>
#include <render/sim_driver.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CRenderTexture::CRenderTexture()
	: CTexture()
{
	m_type			= Type::RGB;
	m_format		= Format::RGB;

	m_bufferiD		= 0;
}

// ----------------------------------------------------------------------//

CRenderTexture::CRenderTexture( const std::string &name )
	: CRenderTexture()
{
	m_name = name;
}

// ----------------------------------------------------------------------//

CRenderTexture::~CRenderTexture()
{
	if (m_bufferiD) {
		glDeleteFramebuffers(1, &m_bufferiD);
	}

	SIM_CHECK_OPENGL();
}

// ----------------------------------------------------------------------//

void CRenderTexture::Generate( u32 width, u32 height )
{
	m_width		= zpl_next_pow2( width );
	m_height	= zpl_next_pow2( height );

	glGenFramebuffers(1, &m_bufferiD);
	glBindFramebuffer(GL_FRAMEBUFFER, m_bufferiD);

	glGenTextures( 1, &m_iD );
	CTexture::ApplyWrap(this, CTexture::Wrap::Clamp);
	CTexture::ApplyFilter(this, CTexture::Filter::Nearest);
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
bool CRenderTexture::Load(io::CMemStream* ms)
{
    return false;
}
// ----------------------------------------------------------------------//
bool CRenderTexture::Save(io::CMemStream* ms)
{
    return false;
}
// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
