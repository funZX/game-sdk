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

#ifndef __SIM_CORE_H
#define __SIM_CORE_H

#ifdef WIN32
#	include <windows.h>
#else
#	include <sys/time.h>
#endif

#include <iostream>
#include <map>
#include <vector>
#include <functional>
#include <string>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <stdarg.h>

#include <sigcxx/sigcxx.hpp>
#include <librg.h>

namespace sim
{
	typedef	char 			s8;
	typedef zpl_u8 			u8;
	typedef	zpl_i16			s16;
	typedef zpl_u16			u16;
	typedef	zpl_i32			s32;
	typedef zpl_u32			u32;
	typedef	zpl_i64		    s64;
	typedef zpl_u64		    u64;

	typedef zpl_f32			f32;
	typedef zpl_f64			f64;

	typedef size_t 			handle;
	typedef zpl_i32 		result;

	namespace sys
	{
		u32					GetTime( void );
		u64					GetTimeMicro( void );
		void				Wait( u64 micro );
	};

	namespace util
	{
		std::vector<std::string>	StringSplit(const std::string& source, const std::string& delimiter);
	};

	namespace hash
	{
		u32					Get( const std::string& str );
		u32					Get( const s8* buffer, u32 len );
	};

	namespace id
	{
		u32					Get();
	};

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

#define SIM_PRINT( ... )			zpl_printf( __VA_ARGS__ )
#define SIM_WARN( ... )				zpl_printf( __VA_ARGS__ )
#define SIM_ERROR( ... )			zpl_printf_err( __VA_ARGS__ )
#define SIM_INFO( ... )				zpl_printf( __VA_ARGS__ )

#define SIM_NEW						new
#define SIM_DELETE					delete

#define SIM_MEMCPY					zpl_memcopy
#define SIM_MEMCMP					zpl_memcompare
#define SIM_MEMSET					zpl_memset

#define SIM_SAFE_DELETE( x )		{ if( x ) { SIM_DELETE x; x = nullptr; } }
#define SIM_SAFE_DELETE_ARRAY( x )	{ if( x ) { SIM_DELETE [] x; x = nullptr; } }

#include "sim_math.h"

namespace sim
{
    namespace col
    {
    // ----------------------------------------------------------------------//

    extern Vec4 White;
    extern Vec4 Black;

    extern Vec4 Red;
    extern Vec4 Green;
    extern Vec4 Blue;

    extern Vec4 Cyan;
    extern Vec4 Magenta;
    extern Vec4 Yellow;

    extern Vec4 Orange;
    extern Vec4 Blueish;

    // ----------------------------------------------------------------------//
};

namespace axis
{
    // ----------------------------------------------------------------------//

    extern Vec4 Origin;
    extern Vec4 Unit;

    extern Vec4 Up;
    extern Vec4 Front;
    extern Vec4 Side;

    extern Quat Look;
    // ----------------------------------------------------------------------//
};
}; // namespace sim
#endif // __SIM_CORE_H
