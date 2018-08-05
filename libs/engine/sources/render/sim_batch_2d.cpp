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

#include <render/sim_driver.h>
#include <render/sim_material.h>
#include <render/sim_effect.h>
#include <render/sim_shader.h>
#include <render/sim_vertex_group.h>
#include <render/sim_vertex_source.h>
#include <render/sim_batch_2d.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CBatch2D::CBatch2D( CDriver *driver )
{
	SIM_ASSERT( driver != nullptr );

	m_driver			                = driver;

	m_numQuads 			                = 0;

	m_vertexGroup						= SIM_NEW CVertexGroup();
	m_vertexGroup->m_vertexSource		= SIM_NEW CVertexSource();

	m_vertexGroup->m_vertexSource->m_type			= CVertexSource::Type::Triangle;
	m_vertexGroup->m_vertexSource->m_vertexFormat	= CVertexSource::k_Vertex_Attribute_Format_Position | CVertexSource::k_Vertex_Attribute_Format_TexCoord_0;
	m_vertexGroup->m_vertexSource->m_vertexStride	= CVertexSource::k_Vertex_Attribute_Offset_Position + CVertexSource::k_Vertex_Attribute_Offset_TexCoord_0;

	AllocateQuads( CBatch2D::MaxQuads );
}

// ----------------------------------------------------------------------//

CBatch2D::CBatch2D( const std::string& name, CDriver *driver )
	: CBatch2D( driver )
{
	m_name = name;
}

// ----------------------------------------------------------------------//

CBatch2D::~CBatch2D()
{
	SIM_SAFE_DELETE_ARRAY( m_vertexGroup->m_vertexSource->m_vboData );
	SIM_SAFE_DELETE( m_vertexGroup->m_vertexSource );

	SIM_SAFE_DELETE_ARRAY( m_vertexGroup->m_vboData );
	SIM_SAFE_DELETE( m_vertexGroup );
}

// ----------------------------------------------------------------------//

void CBatch2D::AddQuad( CMaterial *material, const f32* v0, const f32* v1, const f32* v2, const f32* v3 )
{
	CMaterial* crtMaterial = m_vertexGroup->GetMaterial();

	if ( material != crtMaterial || m_numQuads >= MaxQuads )
	{
		if ( m_numQuads > 0 )
		{
			m_driver->EnableBatch2D(false);
			Render(m_driver);
			m_driver->EnableBatch2D(true);
		}

		m_vertexGroup->SetMaterial( material );
	}

	s32 vtxSize		= m_vertexGroup->m_vertexSource->m_vertexStride / sizeof( f32 );
	s32 currIndex	= 4 * vtxSize * m_numQuads;

	const f32 *vertices[] = { v0, v1, v2, v3 };

	for( s32 n = 0; n < 4; n++ )
	{
		s32 vOff	= n * vtxSize;

		// x,y,z
		m_vertexGroup->m_vertexSource->m_vboData[ currIndex + vOff + 0 ] = vertices[ n ][ 0 ];
		m_vertexGroup->m_vertexSource->m_vboData[ currIndex + vOff + 1 ] = vertices[ n ][ 1 ];
		m_vertexGroup->m_vertexSource->m_vboData[ currIndex + vOff + 2 ] = vertices[ n ][ 2 ];

		// u,v
		m_vertexGroup->m_vertexSource->m_vboData[ currIndex + vOff + 3 ] = vertices[ n ][ 3 ];
		m_vertexGroup->m_vertexSource->m_vboData[ currIndex + vOff + 4 ] = vertices[ n ][ 4 ];
	}

	++m_numQuads;
}

// ----------------------------------------------------------------------//

void CBatch2D::Render( CDriver *driver )
{
	if( m_numQuads == 0 )
		return;

	m_vertexGroup->m_vboSize					= 6 * m_numQuads;
	m_vertexGroup->m_vertexSource->m_vboSize	= 4 * m_numQuads;

	driver->Render( m_vertexGroup );

	m_numQuads = 0;
	m_vertexGroup->SetMaterial( nullptr );

	SIM_CHECK_OPENGL();
}

// ----------------------------------------------------------------------//

void CBatch2D::AllocateQuads( s32 numQuads )
{
	s32 vtxSize						= m_vertexGroup->m_vertexSource->m_vertexStride / sizeof( f32 );
	s32 newFloats 					= 4 * numQuads * vtxSize;
	s32 newIndices					= 6 * numQuads;

	m_vertexGroup->m_vboSize		= newIndices;
	m_vertexGroup->m_vboData		= SIM_NEW u16[ newIndices ];
	m_vertexGroup->m_vertexSource->m_vboData		= SIM_NEW f32[ newFloats ];

	SIM_MEMSET( m_vertexGroup->m_vertexSource->m_vboData, 0, newFloats * sizeof( f32 ) );

	for( s32 n = 0; n < numQuads; n++ )
	{
	    s32 triSize     = 6 * n;
	    s32 quadSize    = 4 * n;

		m_vertexGroup->m_vboData[ triSize + 0 ] = (u16) quadSize + 2;
		m_vertexGroup->m_vboData[ triSize + 1 ] = (u16) quadSize + 1;
		m_vertexGroup->m_vboData[ triSize + 2 ] = (u16) quadSize + 0;
		m_vertexGroup->m_vboData[ triSize + 3 ] = (u16) quadSize + 2;
		m_vertexGroup->m_vboData[ triSize + 4 ] = (u16) quadSize + 3;
		m_vertexGroup->m_vboData[ triSize + 5 ] = (u16) quadSize + 1;
	}
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
