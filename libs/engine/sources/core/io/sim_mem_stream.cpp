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

namespace sim
{
namespace io
{
// ----------------------------------------------------------------------//

CMemStream::CMemStream( void *mem, u32 maxsize )
{
	m_memory		= (s8*) mem;
	m_maxsize		= maxsize;
	m_cursor		= 0;
	m_isMemOwn		= false;
}

// ----------------------------------------------------------------------//

CMemStream::CMemStream( u32 maxsize )
{
	m_memory		= SIM_NEW s8[ maxsize ];
	m_maxsize		= maxsize;
	m_cursor		= 0;
	m_isMemOwn		= true;
}

// ----------------------------------------------------------------------//

CMemStream::~CMemStream()
{
	if( m_isMemOwn ) {
		SIM_SAFE_DELETE( m_memory );
	}
}

// ----------------------------------------------------------------------//

s8 CMemStream::ReadS8()
{
	SIM_ASSERT( CanRead( sizeof( s8 ) ) );

	s8 c = *( m_memory + m_cursor );
	m_cursor++;

	return c;
}

// ----------------------------------------------------------------------//

u8 CMemStream::ReadU8()
{
	SIM_ASSERT( CanRead( sizeof( u8 ) ) );

	u8 c = (u8)( *( m_memory + m_cursor ) );
	m_cursor++;

	return c;
}

// ----------------------------------------------------------------------//

s16 CMemStream::ReadS16()
{
	SIM_ASSERT( CanRead( sizeof( s16 ) ) );

	s16 h = 0;

	h  = ( m_memory[ m_cursor++ ] & 0xFF) << 8;
    h |= ( m_memory[ m_cursor++ ] & 0xFF);

    return h;
}

// ----------------------------------------------------------------------//

u16 CMemStream::ReadU16()
{
	SIM_ASSERT( CanRead( sizeof( u16 ) ) );

	u16 h;

	h  = ( m_memory[ m_cursor++ ] & 0xFF) << 8;
    h |= ( m_memory[ m_cursor++ ] & 0xFF);

    return h;
}

// ----------------------------------------------------------------------//

s32 CMemStream::ReadS32()
{
	SIM_ASSERT( CanRead( sizeof( s32 ) ) );

	s32 i = 0;

	i  = ( m_memory[ m_cursor++ ] & 0xFF ) << 24;
    i |= ( m_memory[ m_cursor++ ] & 0xFF ) << 16;
    i |= ( m_memory[ m_cursor++ ] & 0xFF ) << 8;
    i |= ( m_memory[ m_cursor++ ] & 0xFF );

    return i;
}

// ----------------------------------------------------------------------//

u32 CMemStream::ReadU32()
{
	SIM_ASSERT( CanRead( sizeof( u32 ) ) );

	u32 i = 0;

	i  = ( m_memory[ m_cursor++ ] & 0xFF ) << 24;
    i |= ( m_memory[ m_cursor++ ] & 0xFF ) << 16;
    i |= ( m_memory[ m_cursor++ ] & 0xFF ) << 8;
    i |= ( m_memory[ m_cursor++ ] & 0xFF );

    return i;
}

// ----------------------------------------------------------------------//

f32 CMemStream::ReadF32()
{
	SIM_ASSERT( CanRead( sizeof( f32 ) ) );

	f32 f  = 0.0f;
	s8 *p = ( s8 * )&f;

	*( p + 0 )	= m_memory[ m_cursor++ ];
	*( p + 1 )	= m_memory[ m_cursor++ ];
	*( p + 2 )	= m_memory[ m_cursor++ ];
	*( p + 3 )	= m_memory[ m_cursor++ ];

	return f;
}

// ----------------------------------------------------------------------//

std::string CMemStream::ReadString()
{
	std::string s( &m_memory[ m_cursor ] );

	while  ( m_memory[ m_cursor++ ] );

	return s;
}

// ----------------------------------------------------------------------//

void* CMemStream::Read( u32 nBytes )
{
	SIM_ASSERT( CanRead( nBytes * sizeof( s8 ) ) );

	s8 *c = nullptr;

	if( nBytes >= 0 )
	{
		c = &m_memory[ m_cursor ];
		m_cursor += nBytes * sizeof( s8 );
	}

	return c;
}

// ----------------------------------------------------------------------//

void CMemStream::WriteS8( s8 c )
{
	SIM_ASSERT( CanWrite( sizeof( s8 ) ) );

	m_memory[ m_cursor++ ] = c;
}

// ----------------------------------------------------------------------//

void CMemStream::WriteU8( u8 c )
{
	SIM_ASSERT( CanWrite( sizeof( u8 ) ) );

	m_memory[ m_cursor++ ] = (s8) c;
}

// ----------------------------------------------------------------------//

void CMemStream::WriteS16( s16 h )
{
	SIM_ASSERT( CanWrite( sizeof( s16 ) ) );

	m_memory[ m_cursor++ ] = ( s8 )( ( h & 0x0000FF00 ) >> 8 );
	m_memory[ m_cursor++ ] = ( s8 )( ( h & 0x000000FF ) );
}

// ----------------------------------------------------------------------//

void CMemStream::WriteU16( u16 h )
{
	SIM_ASSERT( CanWrite( sizeof( u16 ) ) );

	m_memory[ m_cursor++ ] = ( s8 )( ( h & 0x0000FF00 ) >> 8 );
	m_memory[ m_cursor++ ] = ( s8 )( ( h & 0x000000FF ) );
}

// ----------------------------------------------------------------------//

void CMemStream::WriteS32( s32 i )
{
	SIM_ASSERT( CanWrite( sizeof( s32 ) ) );

	m_memory[ m_cursor++ ] = ( s8 )( ( i & 0xFF000000 ) >> 24 );
	m_memory[ m_cursor++ ] = ( s8 )( ( i & 0x00FF0000 ) >> 16 );
	m_memory[ m_cursor++ ] = ( s8 )( ( i & 0x0000FF00 ) >> 8 );
	m_memory[ m_cursor++ ] = ( s8 )( ( i & 0x000000FF ) );
}

// ----------------------------------------------------------------------//

void CMemStream::WriteU32( u32 i )
{
	SIM_ASSERT( CanWrite( sizeof( u32 ) ) );

	m_memory[ m_cursor++ ] = ( s8 )( ( i & 0xFF000000 ) >> 24 );
	m_memory[ m_cursor++ ] = ( s8 )( ( i & 0x00FF0000 ) >> 16 );
	m_memory[ m_cursor++ ] = ( s8 )( ( i & 0x0000FF00 ) >> 8 );
	m_memory[ m_cursor++ ] = ( s8 )( ( i & 0x000000FF ) );
}

// ----------------------------------------------------------------------//

void CMemStream::WriteF32( f32 f )
{
	SIM_ASSERT( CanWrite( sizeof( f32 ) ) );

	f32 data = f;
	s8 *p = ( s8* ) &data;

	m_memory[ m_cursor++ ] = *( p + 0 );
	m_memory[ m_cursor++ ] = *( p + 1 );
	m_memory[ m_cursor++ ] = *( p + 2 );
	m_memory[ m_cursor++ ] = *( p + 3 );
}

// ----------------------------------------------------------------------//

void CMemStream::WriteString( const std::string& s )
{
	u32 len = s.length() + 1;

	SIM_MEMCPY( &m_memory[ m_cursor ], s.c_str(), len );

	m_cursor += len;
}

// ----------------------------------------------------------------------//

void CMemStream::Write( void *p, u32 nBytes )
{
	SIM_ASSERT( CanWrite( nBytes * sizeof( s8 ) ) );

	if( nBytes > 0 )
	{
		SIM_MEMCPY( &m_memory[ m_cursor ], p, nBytes * sizeof( s8 ) );
		m_cursor += nBytes * sizeof( s8 );
	}
}

// ----------------------------------------------------------------------//

bool CMemStream::CanWrite( u32 nBytes )
{
	return ( m_cursor + nBytes <= m_maxsize );
}

// ----------------------------------------------------------------------//

bool CMemStream::CanRead( u32 nBytes )
{
	return ( m_cursor + nBytes <= m_maxsize );
}

// ----------------------------------------------------------------------//

void* CMemStream::Drop()
{
	void *buf		= m_memory;

	m_memory		= nullptr;
	m_maxsize		= 0;
	m_cursor		= 0;
	m_isMemOwn		= false;

	return buf;
}

// ----------------------------------------------------------------------//
}; // namespace io
}; // namespace sim
