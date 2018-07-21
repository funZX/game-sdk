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

#include <core/io/sim_mem_stream.h>
#include <render/sim_batch_2d.h>
#include <render/sim_material.h>

#include <render/sim_vertex_group.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CVertexGroup::CVertexGroup()
{
	m_iD			= 0;

	m_vertexSource	= NULL;

	m_vboSize		= 0;
	m_vboData       = NULL;

	m_material      = NULL;

	m_lod			= NULL;
	m_detail		= this;
}

// ----------------------------------------------------------------------//

CVertexGroup::~CVertexGroup()
{
	if( m_iD != 0 )
	{
		glDeleteBuffers( 1, &m_iD );

		SIM_CHECK_OPENGL();
	}

	SIM_SAFE_DELETE( m_lod );
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

	if ( m_lod != NULL )
		m_lod->SetMaterial( material );
}

void CVertexGroup::Load( io::CMemStream* ms )
{
	m_vboSize		= ms->ReadU16();

	u32 vboDim		= m_vboSize * sizeof( u16 );

	m_vboData		= SIM_NEW u16[ vboDim / sizeof( u16 ) ];

	SIM_MEMCPY( m_vboData, ms->Read( vboDim ), vboDim );

	if ( ms->ReadU8() )
	{
		CVertexGroup* lod = SIM_NEW CVertexGroup();

		lod->Load( ms );

		m_lod = lod;
	}
}

void CVertexGroup::Save( io::CMemStream* ms )
{
	ms->WriteU16( m_vboSize );
	ms->Write( ( void* ) m_vboData, m_vboSize * sizeof( u16 ) );

	if ( m_lod != NULL )
	{
		ms->WriteU8( 1 );

		m_lod->Save( ms );
	}
	else
		ms->WriteU8( 0 );
}

// ----------------------------------------------------------------------//

void CVertexGroup::SetDetail( u32 detail )
{
	CVertexGroup* lod = this;

	for ( u32 k = 0; k < detail; k++ )
		if ( lod->m_lod )
			lod = lod->m_lod;

	m_detail = lod;
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
