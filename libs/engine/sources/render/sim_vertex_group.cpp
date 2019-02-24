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
#include <render/sim_batch_2d.h>
#include <render/sim_material.h>

#include <render/sim_vertex_group.h>
#include <render/animation/sim_bone_hierarchy.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CVertexGroup::CVertexGroup()
{
	m_iD			= 0;

	m_vertexSource	= nullptr;

	m_vboSize		= 0;
	m_vboData       = nullptr;

	m_material      = nullptr;

	m_boneHierarchy = nullptr;
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

	SIM_SAFE_DELETE_ARRAY( m_vboData );
}

// ----------------------------------------------------------------------//

u32 CVertexGroup::Generate()
{
	glGenBuffers( 1, &m_iD );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_iD );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_vboSize * sizeof(u16), m_vboData, GL_DYNAMIC_DRAW);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	SIM_CHECK_OPENGL();

	SIM_SAFE_DELETE_ARRAY( m_vboData );

	return m_iD;
}

// ----------------------------------------------------------------------//

void CVertexGroup::SetMaterial( CMaterial *material )
{
	m_material = material;
}

void CVertexGroup::Load( io::CMemStream* ms )
{
	m_vboSize		= ms->ReadU16();

	u32 vboDim		= m_vboSize * sizeof( u16 );

	m_vboData		= SIM_NEW u16[ vboDim / sizeof( u16 ) ];

	SIM_MEMCPY( m_vboData, ms->Read( vboDim ), vboDim );

	if ( ms->ReadU8() )
	{
		m_boneHierarchy = SIM_NEW CBoneHierarchy();
		m_boneHierarchy->Load( ms );
	}
}

void CVertexGroup::Save( io::CMemStream* ms )
{
	ms->WriteU16( m_vboSize );
	ms->Write( ( void* ) m_vboData, m_vboSize * sizeof( u16 ) );

	if (m_boneHierarchy != nullptr )
	{
		ms->WriteU8( 1 );
		m_boneHierarchy->Save( ms );
	}
	else
		ms->WriteU8( 0 );
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
