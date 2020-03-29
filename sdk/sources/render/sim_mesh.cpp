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
#include <core/io/sim_file_system.h>

#include <render/sim_shader.h>
#include <render/sim_driver.h>
#include <render/sim_material.h>
#include <render/sim_vertex_group.h>
#include <render/sim_vertex_source.h>
#include <render/sim_mesh.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CMesh::CMesh()
{
	m_vertexSource		= nullptr;
	m_vertexGroup		= nullptr;

    m_box               = axis::Origin.xyz;
	m_center            = axis::Origin.xyz;
	m_radius			= 1.0f;

	m_fs				= nullptr;
}

// ----------------------------------------------------------------------//

CMesh::CMesh( const std::string &name )
	: CMesh()
{
	m_name = name;
}
// ----------------------------------------------------------------------//

CMesh::CMesh(const std::string &name, io::CFileSystem* fs)
	: CMesh( name )
{
	m_fs = fs;
}

// ----------------------------------------------------------------------//

CMesh::~CMesh( void )
{
	SIM_SAFE_DELETE_ARRAY( m_vertexSource );
	SIM_SAFE_DELETE_ARRAY( m_vertexGroup );
}

// ----------------------------------------------------------------------//

void CMesh::Render( CDriver *driver )
{
	SIM_ASSERT( driver != nullptr );

	driver->Render( m_vertexGroup );
}

// ----------------------------------------------------------------------//

bool CMesh::Load( io::CMemStream* ms )
{
	m_box.x = ms->ReadF32();
	m_box.y = ms->ReadF32();
	m_box.z = ms->ReadF32();

	m_center.x = ms->ReadF32();
	m_center.y = ms->ReadF32();
	m_center.z = ms->ReadF32();

	m_radius = ms->ReadF32();

	m_material = ms->ReadString();

	SIM_ASSERT( m_vertexSource == nullptr );
	CVertexSource* vertexSource = SIM_NEW CVertexSource();

	vertexSource->m_vertexFormat = static_cast<CVertexSource::AttributeFormat>(ms->ReadU32());
	vertexSource->m_vertexStride = static_cast<CVertexSource::AttributeStride>(ms->ReadU32());
	vertexSource->m_vboSize		 = ms->ReadU32();
	
	vertexSource->m_vboData = SIM_NEW f32[ vertexSource->m_vboSize / sizeof( f32 ) ];
	SIM_MEMCPY( vertexSource->m_vboData, ms->Read(0), vertexSource->m_vboSize);

	SIM_ASSERT( m_vertexGroup == nullptr );
	CVertexGroup* vertexGroup	= SIM_NEW CVertexGroup();
	vertexGroup->m_vertexSource	= vertexSource;
		
	vertexGroup->Load( ms );

	m_vertexSource	= vertexSource;
	m_vertexGroup	= vertexGroup;

    return true;
}

// ----------------------------------------------------------------------//

bool CMesh::Save( io::CMemStream* ms )
{
	ms->WriteF32( m_box.x );
	ms->WriteF32( m_box.y );
	ms->WriteF32( m_box.z );

	ms->WriteF32( m_center.x );
	ms->WriteF32( m_center.y );
	ms->WriteF32( m_center.z );
	
	ms->WriteF32( m_radius );

	ms->WriteString( m_material );

	// vertex source
	{
		CVertexSource* vertexSource = m_vertexSource;

		ms->WriteU32( vertexSource->GetVertexFormat() );
		ms->WriteU32( vertexSource->GetVertexStride() );
		ms->WriteU32( vertexSource->GetVboSize() );

		ms->Write( vertexSource->m_vboData, vertexSource->GetVboSize() * vertexSource->GetVertexStride() );
	}

	// vertex group
	m_vertexGroup->Save( ms );

    return true;
}

// ----------------------------------------------------------------------//

void CMesh::BindMaterial()
{
	m_vertexGroup->SetMaterial( m_fs->GetMaterial( m_material ) );
}

// ----------------------------------------------------------------------//

CMaterial* CMesh::BindMaterial( CMaterial* material )
{
	CMaterial* oldMaterial = m_vertexGroup->GetMaterial();

	m_vertexGroup->SetMaterial( material );

	return oldMaterial;
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
