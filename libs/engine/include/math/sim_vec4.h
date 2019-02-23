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

#ifndef __SIM_VEC4_H
#define __SIM_VEC4_H

#include <math/sim_math.h>
#include <jansson.h>

namespace sim
{
namespace mat
{
// ----------------------------------------------------------------------//

union TVec4
{
	f32 raw[ 4 ];
	
	struct 
	{
		f32 x;
		f32 y;
		f32 z;
		f32 w;
	};
};

static inline void Vec4ToZero( TVec4 *_this )
{
	_this->x = 0.0f;
	_this->y = 0.0f;
	_this->z = 0.0f;
	_this->w = 0.0f;
}

static inline void Vec4ToOne(TVec4 *_this)
{
	_this->x = 1.0f;
	_this->y = 1.0f;
	_this->z = 1.0f;
	_this->w = 1.0f;
}

static inline void Vec4ToWhite(TVec4 *_this)
{
	_this->x = 1.0f;
	_this->y = 1.0f;
	_this->z = 1.0f;
	_this->w = 1.0f;
}

static inline void Vec4ToBlack(TVec4 *_this)
{
	_this->x = 0.0f;
	_this->y = 0.0f;
	_this->z = 0.0f;
	_this->w = 1.0f;
}

static inline void Vec4Copy( TVec4 *_this, const TVec4 *src )
{
	_this->x = src->x;
	_this->y = src->y;
	_this->z = src->z;
	_this->w = src->w;
}

static inline void Vec4Set( TVec4 *_this, const f32 x, const f32 y, const f32 z, const f32 w )
{
	_this->x = x;
	_this->y = y;
	_this->z = z;
	_this->w = w;
}

static inline void Vec4Add( TVec4 *_this, const TVec4 *src1, const TVec4 *src2 )
{
	_this->x = src1->x + src2->x;
	_this->y = src1->y + src2->y;
	_this->z = src1->z + src2->z;
	_this->w = src1->w + src2->w;
}

static inline void Vec4Diff( TVec4*_this, const TVec4 *src1, const TVec4 *src2 )
{
	_this->x = src1->x - src2->x;
	_this->y = src1->y - src2->y;
	_this->z = src1->z - src2->z;
	_this->w = src1->w - src2->w;
}

static inline void Vec4Scale( TVec4 *_this, const TVec4 *src, const f32 k )
{
	_this->x = k * src->x;
	_this->y = k * src->y;
	_this->z = k * src->z;
	_this->w = k * src->w;
}

static inline void Vec4Multiply( const TVec4 *src1, const TVec4 *src2, TVec4 *dst )
{
	dst->x = src1->x * src2->x;
	dst->y = src1->y * src2->y;
	dst->z = src1->z * src2->z;
	dst->w = src1->w * src2->w;
}

static inline void Vec4Multiply( TVec4 *_this, const TVec4 *src )
{
	_this->x *= src->x;
	_this->y *= src->y;
	_this->z *= src->z;
	_this->w *= src->w;
}

static inline f32 Vec4Magnitude( const TVec4 *_this )
{
	return Sqrt( ( _this->x * _this->x ) +
				 ( _this->y * _this->y ) +
				 ( _this->z * _this->z ) +
				 ( _this->w * _this->w ) );
}

static inline f32 Vec4RMagnitude( const TVec4 *_this )
{
	return RSqrt( ( _this->x * _this->x ) +
				  ( _this->y * _this->y ) +
				  ( _this->z * _this->z ) +
				  ( _this->w * _this->w ) );
}

static inline f32 Vec4Dot( const TVec4 *src1, const TVec4 *src2 )
{
	return  ( src1->x * src2->x ) +
			( src1->y * src2->y ) +
			( src1->z * src2->z ) +
			( src1->w * src2->w );
}

static inline f32 Vec4Dist( const TVec4 *src1, const TVec4 *src2 )
{
	TVec4 v3;

	Vec4Diff( &v3, src1, src2 );

	return Vec4Magnitude( &v3 );
}

static inline f32 Vec4Dist2( const TVec4 *src1, const TVec4 *src2 )
{
	TVec4 v3;

	Vec4Diff( &v3, src1, src2 );

	return Vec4Dot( &v3, &v3 );
}

static inline void Vec4Normalize( TVec4 *_this )
{
	const f32 mag = Vec4RMagnitude( _this );

	_this->x = _this->x * mag;
	_this->y = _this->y * mag;
	_this->z = _this->z * mag;
	_this->w = _this->w * mag;
}

// ----------------------------------------------------------------------//
}; // namespace mat
}; // namespace sim

#endif // __SIM_VEC4_H
