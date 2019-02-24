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

#include <core/sim_core.h>

// ----------------------------------------------------------------------//

namespace sim
{
namespace util
{
	// ----------------------------------------------------------------------//
	std::vector<std::string> StringSplit(const std::string& source, const std::string& delimiter)
	{
		std::vector<std::string> result;
		std::string s = source;
		std::string token;
		size_t pos = 0;
		
		while (  (pos = s.find( delimiter ) ) != std::string::npos )
		{
			token = s.substr( 0, pos );
			result.push_back( token );
			s.erase( 0, pos + delimiter.length() );
		}

		result.push_back(s);

		return result;
	}
	// ----------------------------------------------------------------------//
} // namespace util
  // ----------------------------------------------------------------------//

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
