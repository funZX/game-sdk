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

#include <render/sim_vertex_source.h>
#include <render/sim_driver.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CVertexSource::CVertexSource()
{
	m_iD				= 0;

	m_vboData			= nullptr;
	m_vboSize			= 0;

	m_vertexStride		= AttributeStride::Position;
	m_vertexFormat 		= AttributeFormat::Position;

	m_type				= Type::Triangle;
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

	SIM_SAFE_DELETE_ARRAY( m_vboData );
}

u32 CVertexSource::Generate()
{
	glGenBuffers( 1, &m_iD );
	glBindBuffer( GL_ARRAY_BUFFER, m_iD );
	glBufferData( GL_ARRAY_BUFFER, m_vboSize * EnumValue(m_vertexStride), m_vboData, GL_DYNAMIC_DRAW);
	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	SIM_CHECK_OPENGL();

	SIM_SAFE_DELETE_ARRAY( m_vboData );

	return m_iD;
}
// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
