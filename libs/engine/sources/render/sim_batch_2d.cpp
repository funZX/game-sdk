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
	m_vertexGroup->m_vertexSource->m_vertexFormat	= CVertexSource::AttributeFormat::Position | CVertexSource::AttributeFormat::TexCoord_0;
	m_vertexGroup->m_vertexSource->m_vertexStride	= CVertexSource::AttributeStride::Position + CVertexSource::AttributeStride::TexCoord_0;

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

	s32 vtxSize		= Value(m_vertexGroup->m_vertexSource->m_vertexStride) / sizeof( f32 );
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
	s32 vtxSize						= Value(m_vertexGroup->m_vertexSource->m_vertexStride) / sizeof( f32 );
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
