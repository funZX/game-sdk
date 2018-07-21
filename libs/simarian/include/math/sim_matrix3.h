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

#ifndef __SIM_MATRIX3_H
#define __SIM_MATRIX3_H

#include <core/sim_core.h>
#include <math/sim_math.h>
#include <math/sim_vec3.h>

namespace sim
{
namespace mat
{
// ----------------------------------------------------------------------//

union TMatrix3
{
	f32			raw[ 9 ];

	struct
	{
		f32 m00, m01, m02;
		f32 m10, m11, m12;
		f32 m20, m21, m22;
	};
};

void Matrix3Multiply( TMatrix3 *_this, const TMatrix3 * src );
void Matrix3Multiply( const TMatrix3 *_this, const TMatrix3 * src1, TMatrix3 *src2 );

void Matrix3TransformVec3( const TMatrix3 *_this, const TVec3 *src, TVec3 *dst );
void Matrix3Print( const TMatrix3 *_this );

static inline void Matrix3GetRight( const TMatrix3 *_this, TVec3 *dst )
{
	dst->x = _this->raw[ 0 ];
	dst->y = _this->raw[ 1 ];
	dst->z = _this->raw[ 2 ];
}

static inline void Matrix3GetUp( const TMatrix3 *_this, TVec3 *dst )
{
	dst->x = _this->raw[ 3 ];
	dst->y = _this->raw[ 4 ];
	dst->z = _this->raw[ 5 ];
}

static inline void Matrix3GetForward( const TMatrix3 *_this, TVec3 *dst )
{
	dst->x = _this->raw[ 6 ];
	dst->y = _this->raw[ 7 ];
	dst->z = _this->raw[ 8 ];
}

static inline void Matrix3ToIdentity( TMatrix3 *_this )
{
	_this->raw[ 0 ] = 1.0f;		_this->raw[ 1 ] = 0.0f;		_this->raw[ 2 ] = 0.0f;
	_this->raw[ 3 ] = 0.0f;		_this->raw[ 4 ] = 1.0f;		_this->raw[ 5 ] = 0.0f;
	_this->raw[ 6 ] = 0.0f;		_this->raw[ 7 ] = 0.0f;		_this->raw[ 8 ] = 1.0f;
}

static inline void Matrix3Set( TMatrix3 *_this,
							f32 m00, f32 m01, f32 m02,
							f32 m10, f32 m11, f32 m12,
							f32 m20, f32 m21, f32 m22 )
{
	_this->raw[ 0 ] = m00;		_this->raw[ 1 ] = m01;		_this->raw[ 2 ] = m02;
	_this->raw[ 3 ] = m10;		_this->raw[ 4 ] = m11;		_this->raw[ 5 ] = m12;
	_this->raw[ 6 ] = m20;		_this->raw[ 7 ] = m21;		_this->raw[ 8 ] = m22;
}


static inline void Matrix3Copy( TMatrix3 *_this, const TMatrix3 *src )
{
	SIM_MEMCPY( _this->raw, src->raw, sizeof( TMatrix3 ) );
}

static inline void Matrix3Transpose( const TMatrix3 *_this, TMatrix3 *dst )
{
	TMatrix3 tmp;

	tmp.raw[  0 ] = _this->raw[ 0 ];		tmp.raw[  1 ] = _this->raw[ 3 ];		tmp.raw[ 2 ] = _this->raw[ 6 ];
	tmp.raw[  3 ] = _this->raw[ 1 ];		tmp.raw[  4 ] = _this->raw[ 4 ];		tmp.raw[ 5 ] = _this->raw[ 7 ];
	tmp.raw[  6 ] = _this->raw[ 2 ];		tmp.raw[  7 ] = _this->raw[ 5 ];		tmp.raw[ 8 ] = _this->raw[ 8 ];

	Matrix3Copy( dst, &tmp );
}

static inline void Matrix3ToRotateX( TMatrix3 *_this, const f32 angle )
{

	const f32 radians = SIM_DEG2RAD( angle );
	const f32 sine	= sinf( radians );
	const f32 cosine	= cosf( radians );

	_this->raw[ 0 ] = 1.0f;		_this->raw[ 1 ] = 0.0f;		_this->raw[ 2 ] = 0.0f;
	_this->raw[ 3 ] = 0.0f;		_this->raw[ 4 ] = cosine;	_this->raw[ 5 ] = -sine;
	_this->raw[ 6 ] = 0.0f;		_this->raw[ 7 ] = sine;		_this->raw[ 8 ] = cosine;
}

static inline void Matrix3ToRotateY( TMatrix3 *_this, const f32 angle )
{
	const f32 radians = SIM_DEG2RAD( angle );
	const f32 sine	= sinf( radians );
	const f32 cosine	= cosf( radians );

	_this->raw[ 0 ] = cosine;		_this->raw[ 1 ] = 0.0f;		_this->raw[ 2 ] = sine;
	_this->raw[ 3 ] = 0.0f;			_this->raw[ 4 ] = 1.0f;		_this->raw[ 5 ] = 0.0f;
	_this->raw[ 6 ] = -sine;		_this->raw[ 7 ] = 0.0f;		_this->raw[ 8 ] = cosine;
}

static inline void Matrix3ToRotateZ( TMatrix3 *_this, const f32 angle )
{
	const f32 radians = SIM_DEG2RAD( angle );
	const f32 sine	= sinf( radians );
	const f32 cosine	= cosf( radians );

	_this->raw[ 0 ] = cosine;	_this->raw[ 1 ] = -sine;	_this->raw[ 2 ] = 0.0f;
	_this->raw[ 3 ] = sine;		_this->raw[ 4 ] = cosine;	_this->raw[ 5 ] = 0.0f;
	_this->raw[ 6 ] = 0.0f;		_this->raw[ 7 ] = 0.0f;		_this->raw[ 8 ] = 1.0f;
}

static inline void Matrix3ToScalef( TMatrix3 *_this, const f32 sclx, const f32 scly, const f32 sclz )
{
	_this->raw[ 0 ] = sclx;		_this->raw[ 1 ] = 0.0f;		_this->raw[ 2 ] = 0.0f;		_this->raw[  3 ] = 0.0f;
	_this->raw[ 3 ] = 0.0f;		_this->raw[ 4 ] = scly;		_this->raw[ 5 ] = 0.0f;		_this->raw[  7 ] = 0.0f;
	_this->raw[ 6 ] = 0.0f;		_this->raw[ 7 ] = 0.0f;		_this->raw[ 8 ] = sclz;		_this->raw[ 11 ] = 0.0f;
}

static inline void Matrix3ToScalev( TMatrix3 *_this, TVec3 *scl )
{
	_this->raw[ 0 ] = scl->x;	_this->raw[ 1 ] = 0.0f;		_this->raw[ 2 ] = 0.0f;
	_this->raw[ 3 ] = 0.0f;		_this->raw[ 4 ] = scl->y;	_this->raw[ 5 ] = 0.0f;
	_this->raw[ 6 ] = 0.0f;		_this->raw[ 7 ] = 0.0f;		_this->raw[ 8 ] = scl->z;
}

static inline void Matrix3RotateX( TMatrix3 *_this, const f32 angle )
{
	/*
	const f32 radians = SIM_DEG2RAD( angle );
	const f32 sine	= sinf( radians );
	const f32 cosine	= cosf( radians );

	TVec3 v;

	v.x = _this->raw[  1 ];
	v.y = _this->raw[  5 ];
	v.z = _this->raw[  9 ];
	v.w = _this->raw[ 13 ];

	_this->raw[  1 ] = v.x * cosine + _this->raw[  2 ] * sine;
	_this->raw[  5 ] = v.y * cosine + _this->raw[  6 ] * sine;
	_this->raw[  9 ] = v.z * cosine + _this->raw[ 10 ] * sine;
	_this->raw[ 13 ] = v.w * cosine + _this->raw[ 14 ] * sine;

	_this->raw[  2 ] = _this->raw[  2 ] * cosine - v.x * sine;
	_this->raw[  6 ] = _this->raw[  6 ] * cosine - v.y * sine;
	_this->raw[ 10 ] = _this->raw[ 10 ] * cosine - v.z * sine;
	_this->raw[ 14 ] = _this->raw[ 14 ] * cosine - v.w * sine;
	*/
}

static inline void Matrix3RotateY( TMatrix3 *_this, const f32 angle )
{
	/*
	const f32 radians = SIM_DEG2RAD( angle );
	const f32 sine	= sinf( radians );
	const f32 cosine	= cosf( radians );

	TVec3 v;

	v.x = _this->raw[  0 ];
	v.y = _this->raw[  4 ];
	v.z = _this->raw[  8 ];
	v.w = _this->raw[ 12 ];

	_this->raw[  0 ] = v.x * cosine - _this->raw[  2 ] * sine;
	_this->raw[  4 ] = v.y * cosine - _this->raw[  6 ] * sine;
	_this->raw[  8 ] = v.z * cosine - _this->raw[ 10 ] * sine;
	_this->raw[ 12 ] = v.w * cosine - _this->raw[ 14 ] * sine;

	_this->raw[  2 ] = _this->raw[  2 ] * cosine + v.x * sine;
	_this->raw[  6 ] = _this->raw[  6 ] * cosine + v.y * sine;
	_this->raw[ 10 ] = _this->raw[ 10 ] * cosine + v.z * sine;
	_this->raw[ 14 ] = _this->raw[ 14 ] * cosine + v.w * sine;
	*/
}

static inline void Matrix3RotateZ( TMatrix3 *_this, const f32 angle )
{
	/*
	const f32 radians = SIM_DEG2RAD( angle );
	const f32 sine	= sinf( radians );
	const f32 cosine	= cosf( radians );

	TVec3 v;

	v.x = _this->raw[  0 ];
	v.y = _this->raw[  4 ];
	v.z = _this->raw[  8 ];
	v.w = _this->raw[ 12 ];

	_this->raw[  0 ] = v.x * cosine + _this->raw[  1 ] * sine;
	_this->raw[  4 ] = v.y * cosine + _this->raw[  5 ] * sine;
	_this->raw[  8 ] = v.z * cosine + _this->raw[  9 ] * sine;
	_this->raw[ 12 ] = v.w * cosine + _this->raw[ 13 ] * sine;

	_this->raw[  1 ] = _this->raw[  1 ] * cosine - v.x * sine;
	_this->raw[  5 ] = _this->raw[  5 ] * cosine - v.y * sine;
	_this->raw[  9 ] = _this->raw[  9 ] * cosine - v.z * sine;
	_this->raw[ 13 ] = _this->raw[ 13 ] * cosine - v.w * sine;
	*/
}

static inline void Matrix3ScaleX( TMatrix3 *_this, const f32 sx )
{
	_this->raw[ 0 ] *= sx;
	_this->raw[ 3 ] *= sx;
	_this->raw[ 6 ] *= sx;
}

static inline void Matrix3ScaleY( TMatrix3 *_this, const f32 sy )
{
	_this->raw[ 1 ] *= sy;
	_this->raw[ 4 ] *= sy;
	_this->raw[ 7 ] *= sy;
}

static inline void Matrix3ScaleZ( TMatrix3 *_this, const f32 sz )
{
	_this->raw[ 2 ] *= sz;
	_this->raw[ 5 ] *= sz;
	_this->raw[ 8 ] *= sz;
}

static inline void Matrix3Scale( TMatrix3 *_this, const f32 sx, const f32 sy, const f32 sz )
{
	_this->raw[ 0 ] *= sx;
	_this->raw[ 3 ] *= sx;
	_this->raw[ 6 ] *= sx;

	_this->raw[ 1 ] *= sy;
	_this->raw[ 4 ] *= sy;
	_this->raw[ 7 ] *= sy;

	_this->raw[ 2 ] *= sz;
	_this->raw[ 5 ] *= sz;
	_this->raw[ 8 ] *= sz;
}

static inline void Matrix3Scale( TMatrix3 *_this, const TVec3 *scl )
{
	_this->raw[ 0 ] *= scl->x;
	_this->raw[ 3 ] *= scl->x;
	_this->raw[ 6 ] *= scl->x;

	_this->raw[ 1 ] *= scl->y;
	_this->raw[ 4 ] *= scl->y;
	_this->raw[ 7 ] *= scl->y;

	_this->raw[ 2 ] *= scl->z;
	_this->raw[ 5 ] *= scl->z;
	_this->raw[ 8 ] *= scl->z;
}

// ----------------------------------------------------------------------//
}; // namespace mat
}; // namespace sim

#endif // __SIM_MATRIX3_H
