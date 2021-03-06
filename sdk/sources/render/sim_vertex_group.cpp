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

#include <core/io/sim_mem_stream.h>
#include <render/sim_material.h>

#include <render/sim_vertex_group.h>
#include <render/animation/sim_bone_hierarchy.h>

namespace sim
{
namespace ren
{
// ----------------------------------------------------------------------//

CVertexGroup::CVertexGroup()
{
	m_iD			= 0;

	m_vertexSource	= nullptr;

	m_vboSize		= 0;
	m_vboData       = nullptr;
    m_vboOffset     = 0;

	m_boneHierarchy = nullptr;

    glGenBuffers(1, &m_iD);
    SIM_CHECK_OPENGL();
}

// ----------------------------------------------------------------------//

CVertexGroup::~CVertexGroup()
{
	SIM_SAFE_DELETE( m_boneHierarchy );

	if( m_iD != 0 )
	{
		glDeleteBuffers( 1, &m_iD );

		SIM_CHECK_OPENGL();
	}
}

// ----------------------------------------------------------------------//
void CVertexGroup::BufferData( u32 glUsage )
{
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_iD );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_vboSize, m_vboData, glUsage );

    m_vboData = nullptr;

	SIM_CHECK_OPENGL();
}

// ----------------------------------------------------------------------//

void CVertexGroup::SetMaterial( CMaterial *material )
{
	m_material = *material;
}
// ----------------------------------------------------------------------//
bool CVertexGroup::Load( io::CMemStream* ms )
{
	m_vboSize		= ms->ReadU32();
	m_vboData		= (u16*)ms->Read(0);

	if ( ms->ReadU8() )
	{
		m_boneHierarchy = SIM_NEW CBoneHierarchy();
		m_boneHierarchy->Load( ms );
	}

    BufferData(GL_STREAM_DRAW);
    return true;
}
// ----------------------------------------------------------------------//
bool CVertexGroup::Save( io::CMemStream* ms )
{
	ms->WriteU32( m_vboSize );
	ms->Write( ( void* ) m_vboData, m_vboSize );

	if ( m_boneHierarchy != nullptr )
	{
		ms->WriteU8( 1 );
		m_boneHierarchy->Save( ms );
	}
	else
		ms->WriteU8( 0 );

    return true;
}

// ----------------------------------------------------------------------//
}; // namespace ren
}; // namespace sim
