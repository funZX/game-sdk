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

#include <core/io/sim_file_stream.h>
#include <core/io/sim_json_stream.h>

namespace sim
{
namespace io
{
// ----------------------------------------------------------------------//

CJsonStream::CJsonStream( const std::string &filename )
	: m_jsonRoot(nullptr)
{
	CFileStream* fileStream = SIM_NEW CFileStream( filename );

	if( fileStream->Open() )
	{
		m_jsonRoot = json_loads( ( const char* ) fileStream->Read( 0 ), 0, &m_jsonError );
	}

	SIM_SAFE_DELETE( fileStream );
}

// ----------------------------------------------------------------------//

CJsonStream::CJsonStream( const u8* memfile )
{
	m_jsonRoot = json_loads( ( const char* ) memfile, 0, &m_jsonError );
}

// ----------------------------------------------------------------------//

CJsonStream::~CJsonStream()
{
	if ( m_jsonRoot != nullptr )
	{
		json_decref( m_jsonRoot );
		m_jsonRoot = nullptr;
	}
}

// ----------------------------------------------------------------------//
}; // namespace json
}; // namespace sim

