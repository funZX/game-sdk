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

	Vec3ToZero( &m_box );
	Vec3ToZero( &m_center );
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

void CMesh::SetDetail( u32 detail )
{
	m_vertexGroup->SetDetail( detail ); 
}

// ----------------------------------------------------------------------//

void CMesh::Load( io::CMemStream* ms )
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

	vertexSource->m_vertexFormat = ms->ReadU32();
	vertexSource->m_vertexStride = ms->ReadU32();
	vertexSource->m_vboSize		 = ms->ReadU16();
	
	u32 vboDim = 0;
	
	vboDim = vertexSource->m_vboSize * vertexSource->m_vertexStride;
	vertexSource->m_vboData = SIM_NEW f32[ vboDim / sizeof( f32 ) ];
	SIM_MEMCPY( vertexSource->m_vboData, ms->Read( vboDim ), vboDim );

	SIM_ASSERT( m_vertexGroup == nullptr );
	CVertexGroup* vertexGroup	= SIM_NEW CVertexGroup();
	vertexGroup->m_vertexSource	= vertexSource;
		
	vertexGroup->Load( ms );

	m_vertexSource	= vertexSource;
	m_vertexGroup	= vertexGroup;
}

// ----------------------------------------------------------------------//

void CMesh::Save( io::CMemStream* ms )
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
		ms->WriteU16( vertexSource->GetVboSize() );

		f32* vboData		= vertexSource->GetVboData();
		ms->Write( vboData, vertexSource->GetVboSize() * vertexSource->GetVertexStride() );
	}

	// vertex group
	m_vertexGroup->Save( ms );
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
