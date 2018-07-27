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

#include <core/sim_core.h>

// ----------------------------------------------------------------------//

namespace sim
{
// ----------------------------------------------------------------------//
namespace hash
{
// ----------------------------------------------------------------------//
u32 Get( const s8* buffer, u32 len )
{
	const u32 PRIME = 16777619u;
	const u32 OFFSET = 2166136261u;

	u32 h = OFFSET;

	for (u32 k = 0; k < len; k++)
	{
		h *= PRIME;
		h ^= buffer[ k ];
	}

	return h;
}
// ----------------------------------------------------------------------//
u32 Get( const std::string& str )
{
	return Get( str.c_str(), str.size() );
}
// ----------------------------------------------------------------------//
} // namespace hash
// ----------------------------------------------------------------------//
	
namespace id
{
// ----------------------------------------------------------------------//
u32 Get()
{
	static u32 idCnt = 0;
	return ++idCnt;
}
// ----------------------------------------------------------------------//
} // namespace id
// ----------------------------------------------------------------------//

namespace sys
{
// ----------------------------------------------------------------------//
#ifdef WIN32
// ----------------------------------------------------------------------//
u32 GetTime( void )
{
    LARGE_INTEGER ticks;
    LARGE_INTEGER time;

    QueryPerformanceFrequency ( &ticks );
    QueryPerformanceCounter ( &time );

	static LONGLONG old_time = time.QuadPart;

    return ( u32 ) ( ( ( time.QuadPart - old_time ) * 1000 ) / ticks.QuadPart );
}
// ----------------------------------------------------------------------//
u64 GetTimeMicro( void )
{
    LARGE_INTEGER ticks;
    LARGE_INTEGER time;

    QueryPerformanceFrequency ( &ticks );
    QueryPerformanceCounter ( &time );

	static LONGLONG old_time = time.QuadPart;

    return ( u64 ) ( ( ( time.QuadPart - old_time ) * 1000000 ) / ticks.QuadPart );
}

void Wait( u64 micro )
{
	Sleep( (u32) ( micro / 1000 ) );
}

// ----------------------------------------------------------------------//
#else
// ----------------------------------------------------------------------//
u32 GetTime( void )
{
    struct timeval    tp;
    gettimeofday(&tp, 0);

    static u32 old_tv_sec_1 = tp.tv_sec;
    return ( ( tp.tv_sec - old_tv_sec_1 ) * 1000 + tp.tv_usec / 1000 );
}
// ----------------------------------------------------------------------//
u32 GetTimeMicro( void )
{
    struct timeval    tp;
    gettimeofday( &tp, 0 );

    static u32 old_tv_sec_2 = tp.tv_sec;
    return ( ( tp.tv_sec - old_tv_sec_2 ) * 1000000 + tp.tv_usec );
}

void Wait( u64 micro )
{
	usleep( micro );
}
// ----------------------------------------------------------------------//

#endif

// ----------------------------------------------------------------------//
}; // namespace sys
}; // namespace sim;
