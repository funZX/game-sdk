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

#ifndef __SIM_MATH_LIB_H
#define __SIM_MATH_LIB_H

#include <math.h>
#include <core/sim_core.h>

#define SIM_2PI 				 6.2831853071795862f
#define SIM_PI 					 3.1415926535897931f
#define SIM_PI_R2 				 1.5707963267948966f
#define SIM_PI_R3				 1.0471975511965977f
#define SIM_PI_R4				 0.7853981633974483f
#define SIM_PI_R5				 0.6283185307179586f
#define SIM_PI_R6				 0.5235987755982988f
#define SIM_PI_R12				 0.2617993877991490f
#define SIM_PI_R180				 0.0174532925199432f
#define SIM_PI_R360				 0.0087266462599716f
#define SIM_RPI_180				57.2957795130823208f
// ----------------------------------------------------------------------//
#define SIM_MIN( a, b )					( ( a ) < ( b ) ? ( a ) : ( b ) )
#define SIM_MAX( a, b )					( ( a ) > ( b ) ? ( a ) : ( b ) )
#define SIM_CLAMP( x, low, high ) 		( ( (x) > (high) ) ? (high) : ( ( (x) < (low) ) ? (low) : (x) ) )
#define SIM_SQR( x )					( ( x ) * ( x ) )
#define SIM_DEG2RAD( x )				( ( x ) * SIM_PI_R180 )
#define SIM_RAD2DEG( x )				( ( x ) * SIM_RPI_180 )
// ----------------------------------------------------------------------//
#define SIM_DEG5				  5.0f
#define SIM_DEG10				 10.0f
#define SIM_DEG15				 15.0f
#define SIM_DEG30				 30.0f
#define SIM_DEG45				 45.0f
#define SIM_DEG60				 60.0f
#define SIM_DEG90				 90.0f
#define SIM_DEG180				180.0f
#define SIM_DEG270				270.0f
#define SIM_DEG360				360.0f
// ----------------------------------------------------------------------//

namespace sim
{
namespace mat
{
static inline void BitSet( u32 *_var, u32  _flag )
                    {  *_var = *_var | _flag; }


static inline void BitReset( u32 *_var, u32  _flag )
                    { *_var = *_var & ~_flag; }


static inline bool BitGet( u32 _var, u32 _flag )
                    { return ( _var & _flag ) != 0; }

static inline bool IsPow2( s32 nr )
                    { return (!(nr & (nr - 1))); }

static inline f32	Lerp( const f32 a, const f32 b, const f32 t )
                    { return a + ( ( b - a ) * t ); }


static inline f32	Sqrt( f32 number )
{
	s32 i;
    f32 x, y;
    const f32 threehalfs = 1.5f;

    x = number * 0.5f;
    y  = number;
    i  = * ( s32 * ) &y;
    i  = 0x5F3759DF - ( i >> 1 );
    y  = * ( f32 * ) &i;
    y  = y * ( threehalfs - ( x * y * y ) );
    y  = y * ( threehalfs - ( x * y * y ) );
    return number * y;
}

static inline f32	RSqrt( f32 number )
{
	s32 i;
	f32 x, y;
	const f32 threehalfs = 1.5f;

	x = number * 0.5f;
	y  = number;
	i  = * ( s32 * ) &y;
	i  = 0x5F3759DF - ( i >> 1 );
	y  = * ( f32 * ) &i;
	y  = y * ( threehalfs - ( x * y * y ) );
	y  = y * ( threehalfs - ( x * y * y ) );
	return y;
}

static inline u32 nextPowerOfTwo(int x)
{
	x--;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	return x + 1;
}

// ----------------------------------------------------------------------//
}; // namespace mat
}; // namespace sim
#endif // __SIM_MATH_LIB_H
