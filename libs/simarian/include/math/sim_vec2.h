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

#ifndef __SIM_VEC2_H
#define __SIM_VEC2_H

#include <math/sim_math.h>

namespace sim
{
namespace mat
{
// ----------------------------------------------------------------------//
union TVec2
{
	f32 raw[ 2 ];

	struct
	{
		f32 x;
		f32 y;
	};

	struct
	{
		f32 s;
		f32 t;
	};
};

static inline void Vec2ToZero( TVec2 *_this )
{
	_this->x = 0.0f;
	_this->y = 0.0f;
}

static inline void Vec2ToOne( TVec2 *_this )
{
	_this->x = 1.0f;
	_this->y = 1.0f;
}

static inline void Vec2ToMinusOne( TVec2 *_this )
{
	_this->x = -1.0f;
	_this->y = -1.0f;
}

static inline void Vec2ToDirX( TVec2 *_this )
{
	_this->x = 1.0f;
	_this->y = 0.0f;
}

static inline void Vec2ToDirY( TVec2 *_this )
{
	_this->x = 0.0f;
	_this->y = 1.0f;
}

static inline void Vec2ToRDirX( TVec2 *_this )
{
	_this->x = -1.0f;
	_this->y =  0.0f;
}

static inline void Vec2ToRDirY( TVec2 *_this )
{
	_this->x =  0.0f;
	_this->y = -1.0f;
}

static inline void Vec2Invert( TVec2 *_this )
{
	_this->x = - _this->x;
	_this->y = - _this->x;
}

static inline void Vec2Set( TVec2 *_this, const f32 x, const f32 y )
{
	_this->x = x;
	_this->y = y;
}

static inline void Vec2Copy( TVec2 *_this, const TVec2 *src )
{
	_this->x = src->x;
	_this->y = src->y;
}

static inline void Vec2Add( TVec2 *_this, const TVec2 *src1, const TVec2 *src2 )
{
	_this->x = src1->x + src2->x;
	_this->y = src1->y + src2->y;
}

static inline void Vec2Diff( TVec2 *_this, const TVec2 *src1, const TVec2 *src2 )
{
	_this->x = src1->x - src2->x;
	_this->y = src1->y - src2->y;
}

static inline void Vec2Scale( TVec2 *_this, const TVec2 *src, const f32 k )
{
	_this->x = k * src->x;
	_this->y = k * src->y;
}

static inline void Vec2Multiply( const TVec2 *src1, const TVec2 *src2, TVec2 *dst )
{
	dst->x = src1->x * src2->x;
	dst->y = src1->y * src2->y;
}

static inline void Vec2Multiply( TVec2 *_this, const TVec2 *src )
{
	_this->x *= src->x;
	_this->y *= src->y;
}

static inline f32 Vec2Magnitude( const TVec2 *_this )
{
	return Sqrt( ( _this->x * _this->x ) + ( _this->y * _this->y ) );
}

static inline f32 Vec2RMagnitude( const TVec2 *_this )
{
	return RSqrt( ( _this->x * _this->x ) + ( _this->y * _this->y ) );
}

static inline f32 Vec2Dot( const TVec2 *src1, const TVec2 *src2 )
{
	return  ( src1->x * src2->x ) + ( src1->y * src2->y );
}

static inline f32 Vec2PerpDot( const TVec2 *src1, const TVec2 *src2 )
{
	return  ( src1->x * src2->y ) + ( src1->y * src2->x );
}

static inline f32 Vec2Side( const TVec2 *p, const TVec2 *q, const TVec2* r )
{
	return ( (q->x - p->x) * (r->y - p->y) - (r->x -  p->x) * (q->y - p->y) );
}

static inline f32 Vec2DotToDeg( const TVec2 *src1, const TVec2 *src2 )
{
	f32 v = Vec2Dot( src1, src2 );

	if( v ) {
		return SIM_RAD2DEG( acosf( v ) );
	}

	return 0.0f;
}

static inline f32 Vec2Dist( const TVec2 *src1, const TVec2 *src2 )
{
	TVec2 v3;

	Vec2Diff( &v3, src1, src2 );

	return Vec2Magnitude( &v3 );
}

static inline f32 Vec2Dist2( const TVec2 *src1, const TVec2 *src2 )
{
	TVec2 v3;

	Vec2Diff( &v3, src1, src2 );

	return Vec2Dot( &v3, &v3 );
}

static inline void Vec2Normalize( TVec2 *_this )
{
	const f32 mag = Vec2RMagnitude( _this );

	_this->x = _this->x * mag;
	_this->y = _this->y * mag;
}

static inline void Vec2RotateZ( TVec2 *_this, const f32 _az, const f32 _d )
{
	const f32 a_z = SIM_DEG2RAD( _az );

	_this->x = _this->x + _d * sinf( a_z );
	_this->y = _this->y - _d * cosf( a_z );
}

static inline void Vec2Rotate( TVec2 *_this, const TVec2 *src, const f32 angle )
{
	const f32 ang = SIM_DEG2RAD(angle);

	const f32 s = sinf( ang );
	const f32 c = cosf( ang );

	const f32 _dx = src->x - _this->x;
	const f32 _dy = src->y - _this->y;

	_this->x = _dx * c - _dy * s + src->x;
	_this->y = _dx * s + _dy * c + src->y;
}

static inline f32 Vec2GetAngle( const TVec2* v0, const TVec2* v1 )
{
	f32 l0	= Vec2Magnitude( v0 );
	f32 l1	= Vec2Magnitude( v1 );

	f32 dot = Vec2Dot( v0, v1 );

	return dot == 0.0f ? 0.0f : SIM_RAD2DEG( dot / ( l0 * l1 ) );
}

static inline f32 Vec2GetDirectionAngle( const TVec2* v )
{
	return SIM_RAD2DEG( atanf( v->y / v->x ) );
}

// ----------------------------------------------------------------------//
}; // namespace mat
}; // namespace sim

#endif // __SIM_VEC2_H
