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

namespace sim
{
namespace io
{
// ----------------------------------------------------------------------//

CFileStream::CFileStream( const std::string& filename )
{
	m_cursor 	= 0;
	m_size 		= 0;

	m_memory 	= nullptr;
	m_current 	= nullptr;

	m_filename 	= filename;
}

// ----------------------------------------------------------------------//

CFileStream::~CFileStream()
{
	Close();
}

// ----------------------------------------------------------------------//

bool CFileStream::Open()
{
	FILE *f = fopen( m_filename.c_str(), "rb" );

	if( f )
	{
		fseek( f, 0, SEEK_END );
		m_size = ftell( f );
		fseek( f, 0, SEEK_SET );

		s32 size	= m_size + 1;

		m_memory 	= SIM_NEW s8[ size ];
		m_current 	= m_memory;

		m_memory[ size - 1 ] = 0;

		fread( &m_memory[ 0 ], m_size, 1, f );
		fclose( f );

		return true;
	}

	return false;
}

// ----------------------------------------------------------------------//

void CFileStream::Close( void )
{
	SIM_SAFE_DELETE( m_memory );
}

// ----------------------------------------------------------------------//

void* CFileStream::Drop()
{
	void *buf	= m_memory;

	m_cursor 	= 0;
	m_size 		= 0;

	m_memory 	= nullptr;
	m_current 	= nullptr;

	return buf;
}

// ----------------------------------------------------------------------//

void* CFileStream::Read( u32 size )
{
	void *ptr = nullptr;

	if( m_cursor + size > m_size ) {
		size = m_size - m_cursor;
	}

	ptr = &m_memory[ m_cursor ];

	m_cursor += size;

	return ptr;
}

// ----------------------------------------------------------------------//

bool CFileStream::Eof( void )
{
	return m_cursor == m_size;
}

// ----------------------------------------------------------------------//

void CFileStream::Seek( SeekFrom seekFrom, u32 offset)
{
	switch(seekFrom)
	{
		case SeekFrom::Start:
		{
			m_cursor = offset;
			break;
		}
		case SeekFrom::Current:
		{
			m_cursor += offset;
			break;
		}
		case SeekFrom::End:
		{
			m_cursor -= offset;
			break;
		}
	}
}

// ----------------------------------------------------------------------//
}; // namespace io
}; // namespace sim

