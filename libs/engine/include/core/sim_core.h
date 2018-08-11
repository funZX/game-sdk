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

#ifndef __SIM_CORE_H
#define __SIM_CORE_H

#include <iostream>
#include <map>
#include <vector>
#include <functional>
#include <string>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <stdarg.h>


#ifdef WIN32
#	include <windows.h>
#else
#	include <sys/time.h>
#endif

#include <sigcxx/sigcxx.hpp>

namespace sim
{
	typedef			 char 			s8;
	typedef unsigned char 			u8;
	typedef			 short 			s16;
	typedef unsigned short			u16;
	typedef			 int			s32;
	typedef unsigned int			u32;
	typedef			 long long		s64;
	typedef unsigned long long		u64;

	typedef float					f32;
	typedef double					f64;

	typedef size_t 					handle;
	typedef signed int 				result;

	namespace sys
	{
		u32							GetTime( void );
		u64							GetTimeMicro( void );
		void						Wait( u64 micro );
	};

	namespace hash
	{
		u32							Get( const std::string& str );
		u32							Get( const s8* buffer, u32 len );
	};

	namespace id
	{
		u32							Get();
	};


	
	static const int k_Pool_Size = 64;
	
	// ----------------------------------------------------------------------//
	// easy cast typed enums to underlying type
	template <typename E>
	constexpr typename std::underlying_type<E>::type Value(E e) noexcept {
		static_assert(std::is_enum<E>::value, "template parameter is not an enum type");
		return static_cast<typename std::underlying_type<E>::type>(e);
	};

	// ----------------------------------------------------------------------//

}; // namespace sim

#define SIM_ASSERT( x )				assert(x)

#define SIM_PRINT( ... )			printf( __VA_ARGS__ )
#define SIM_WARN( ... )				printf( __VA_ARGS__ )
#define SIM_ERROR( ... )			printf( __VA_ARGS__ )
#define SIM_INFO( ... )				printf( __VA_ARGS__ )

#define SIM_NEW						new
#define SIM_DELETE					delete

#define SIM_MEMCPY					memcpy
#define SIM_MEMCMP					memcmp
#define SIM_MEMSET					memset

#define SIM_SAFE_DELETE( x )		{ if( x ) { SIM_DELETE x; x = nullptr; } }
#define SIM_SAFE_DELETE_ARRAY( x )	{ if( x ) { SIM_DELETE [] x; x = nullptr; } }

#endif // __SIM_CORE_H
