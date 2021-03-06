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

#include <render/sim_vertex_source.h>
#include <render/sim_driver.h>

namespace sim
{
namespace ren
{
// ----------------------------------------------------------------------//

CVertexSource::CVertexSource()
{
	m_iD				= 0;

	m_vboData			= nullptr;
	m_vboSize			= 0;

	m_vertexStride		= AttributeStride::WorldPos;
	m_vertexFormat 		= AttributeFormat::WorldPos;

	m_type				= Type::Triangle;

    glGenBuffers(1, &m_iD);
    SIM_CHECK_OPENGL();
}
// ----------------------------------------------------------------------//

CVertexSource::CVertexSource( const std::string& name )
	: CVertexSource()
{
	m_name = name;
}
// ----------------------------------------------------------------------//

CVertexSource::~CVertexSource( void )
{
	if( m_iD != 0 )
	{
		glDeleteBuffers( 1, &m_iD );

		SIM_CHECK_OPENGL();
	}
}

// ----------------------------------------------------------------------//

void CVertexSource::BufferData( u32 glUsage )
{
    glBindBuffer( GL_ARRAY_BUFFER, m_iD );
    glBufferData( GL_ARRAY_BUFFER, m_vboSize, m_vboData, glUsage );

    m_vboData = nullptr;

    SIM_CHECK_OPENGL();
}

// ----------------------------------------------------------------------//

bool CVertexSource::Load(io::CMemStream* ms)
{
    return false;
}

// ----------------------------------------------------------------------//
bool CVertexSource::Save(io::CMemStream* ms)
{
    return false;
}
// ----------------------------------------------------------------------//
}; // namespace ren
}; // namespace sim
