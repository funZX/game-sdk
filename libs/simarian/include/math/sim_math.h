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
