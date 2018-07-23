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

#ifndef __SIM_MATRIX4_H
#define __SIM_MATRIX4_H

#include <math/sim_math.h>
#include <math/sim_matrix3.h>
#include <math/sim_vec4.h>
#include <math/sim_quat.h>

namespace sim
{
namespace mat
{
// ----------------------------------------------------------------------//

union TMatrix4
{
	f32 raw[ 16 ];

	struct
	{
		f32 m00, m01, m02, m03;
		f32 m10, m11, m12, m13;
		f32 m20, m21, m22, m23;
		f32 m30, m31, m32, m33;
	};
};

void Matrix4FromPerspective( TMatrix4 *_this, const f32 fov, const f32 aspectratio, const f32 znear, const f32 zfar );
void Matrix4FromOrtho( TMatrix4 *_this, f32 left, f32 right, f32 bottom, f32 top, f32 znear, const f32 zfar );

void Matrix4FromLookAt( TMatrix4 *_this, const TVec3 *eye, const TVec3 *center, const TVec3 *up );
void Matrix4FromAxisAngle( TMatrix4 *_this, const f32 angle, const TVec3 *axis );
void Matrix4FromDirection( TMatrix4 *_this, const TVec3 *dir, const f32 roll );
void Matrix4FromDirectionNoRoll( TMatrix4 *_this, const TVec3 *dir );

void Matrix4FromQuat( TMatrix4 *_this, const TQuat *q );
void Matrix4FromQuatNoConst( TMatrix4 *_this, const TQuat *q );
void Matrix4FromQuatNoConstNoDir( TMatrix4 *_this, const TQuat *q );

void Matrix4Rotate( TMatrix4 *_this, f32 angle, const TVec3 *rot );
void Matrix4Rotate( TMatrix4 *_this, f32 angle, const f32 x, const f32 y, const f32 z );

void Matrix4Invert( const TMatrix4 *_this, TMatrix4 *dst );
void Matrix4InvertNoScaling( const TMatrix4 *_this, TMatrix4 *dst );

void Matrix4Multiply( TMatrix4 *_this, const TMatrix4 * src );
void Matrix4Multiply( const TMatrix4 *src1, const TMatrix4 * src2, TMatrix4 *dst );

void Matrix4TransformHomogen( const TMatrix4 *_this, const TVec3 *src, TVec3 *dst );
void Matrix4Transform( const TMatrix4 *_this, const TVec3 *src, TVec3 *dst );

void Matrix4Print( const TMatrix4 *_this );

static inline void Matrix4GetSide( const TMatrix4 *_this, TVec3 *dst )
{
	dst->x = _this->raw[ 0 ];
	dst->y = _this->raw[ 1 ];
	dst->z = _this->raw[ 2 ];
}

static inline void Matrix4GetUp( const TMatrix4 *_this, TVec3 *dst )
{
	dst->x = _this->raw[ 4 ];
	dst->y = _this->raw[ 5 ];
	dst->z = _this->raw[ 6 ];
}

static inline void Matrix4GetFront( const TMatrix4 *_this, TVec3 *dst )
{
	dst->x = _this->raw[  8 ];
	dst->y = _this->raw[  9 ];
	dst->z = _this->raw[ 10 ];
}

static inline void Matrix4ToMatrix3( const TMatrix4 *_this, TMatrix3 *dst )
{
	dst->raw[ 0 ] = _this->raw[  0 ];		dst->raw[ 1 ] = _this->raw[  1 ];		dst->raw[ 2 ] = _this->raw[  2 ];
	dst->raw[ 3 ] = _this->raw[  4 ];		dst->raw[ 4 ] = _this->raw[  5 ];		dst->raw[ 5 ] = _this->raw[  6 ];
	dst->raw[ 6 ] = _this->raw[  8 ];		dst->raw[ 7 ] = _this->raw[  9 ];		dst->raw[ 8 ] = _this->raw[ 10 ];
}

static inline void Matrix4ToIdentity( TMatrix4 *_this )
{
	_this->raw[  0 ] = 1.0f;		_this->raw[  1 ] = 0.0f;		_this->raw[  2 ] = 0.0f;		_this->raw[  3 ] = 0.0f;
	_this->raw[  4 ] = 0.0f;		_this->raw[  5 ] = 1.0f;		_this->raw[  6 ] = 0.0f;		_this->raw[  7 ] = 0.0f;
	_this->raw[  8 ] = 0.0f;		_this->raw[  9 ] = 0.0f;		_this->raw[ 10 ] = 1.0f;		_this->raw[ 11 ] = 0.0f;
	_this->raw[ 12 ] = 0.0f;		_this->raw[ 13 ] = 0.0f;		_this->raw[ 14 ] = 0.0f;		_this->raw[ 15 ] = 1.0f;
}

static inline void Matrix4Set( TMatrix4 *_this,
							f32 m00, f32 m01, f32 m02, f32 m03,
							f32 m10, f32 m11, f32 m12, f32 m13,
							f32 m20, f32 m21, f32 m22, f32 m23,
							f32 m30, f32 m31, f32 m32, f32 m33 )
{
	_this->raw[  0 ] = m00;		_this->raw[  1 ] = m01;		_this->raw[  2 ] = m02;		_this->raw[  3 ] = m03;
	_this->raw[  4 ] = m10;		_this->raw[  5 ] = m11;		_this->raw[  6 ] = m12;		_this->raw[  7 ] = m13;
	_this->raw[  8 ] = m20;		_this->raw[  9 ] = m21;		_this->raw[ 10 ] = m22;		_this->raw[ 11 ] = m23;
	_this->raw[ 12 ] = m30;		_this->raw[ 13 ] = m31;		_this->raw[ 14 ] = m32;		_this->raw[ 15 ] = m33;
}


static inline void Matrix4Copy( TMatrix4 *_this, const TMatrix4 *src )
{
	SIM_MEMCPY( _this->raw, src->raw, sizeof( TMatrix4 ) );
}

static inline void Matrix4Transpose( const TMatrix4 *_this, TMatrix4 *dst )
{
	TMatrix4 tmp;

	tmp.raw[  0 ] = _this->raw[ 0 ];		tmp.raw[  1 ] = _this->raw[ 4 ];		tmp.raw[  2 ] = _this->raw[ 8 ];		tmp.raw[  3 ] = _this->raw[ 12 ];
	tmp.raw[  4 ] = _this->raw[ 1 ];		tmp.raw[  5 ] = _this->raw[ 5 ];		tmp.raw[  6 ] = _this->raw[ 9 ];		tmp.raw[  7 ] = _this->raw[ 13 ];
	tmp.raw[  8 ] = _this->raw[ 2 ];		tmp.raw[  9 ] = _this->raw[ 6 ];		tmp.raw[ 10 ] = _this->raw[ 10 ];		tmp.raw[ 11 ] = _this->raw[ 14 ];
	tmp.raw[ 12 ] = _this->raw[ 3 ];		tmp.raw[ 13 ] = _this->raw[ 7 ];		tmp.raw[ 14 ] = _this->raw[ 11 ];		tmp.raw[ 15 ] = _this->raw[ 15 ];

	Matrix4Copy( dst, &tmp );
}

static inline void Matrix4ToTranslate( TMatrix4 *_this, const TVec3 *loc )
{
	_this->raw[  0 ] = 1.0f;		_this->raw[  1 ] = 0.0f;		_this->raw[  2 ] = 0.0f;		_this->raw[  3 ] = loc->x;
	_this->raw[  4 ] = 0.0f;		_this->raw[  5 ] = 1.0f;		_this->raw[  6 ] = 0.0f;		_this->raw[  7 ] = loc->y;
	_this->raw[  8 ] = 0.0f;		_this->raw[  9 ] = 0.0f;		_this->raw[ 10 ] = 1.0f;		_this->raw[ 11 ] = loc->z;
	_this->raw[ 12 ] = 0.0f;		_this->raw[ 13 ] = 0.0f;		_this->raw[ 14 ] = 0.0f;		_this->raw[ 15 ] = 1.0f;
}

static inline void Matrix4ToRotateX( TMatrix4 *_this, const f32 angle )
{

	const f32 radians	= SIM_DEG2RAD( angle );
	const f32 sine		= sinf( radians );
	const f32 cosine	= cosf( radians );

	_this->raw[  0 ] = 1.0f;		_this->raw[  1 ] = 0.0f;		_this->raw[  2 ] = 0.0f;		_this->raw[  3 ] = 0.0f;
	_this->raw[  4 ] = 0.0f;		_this->raw[  5 ] = cosine;		_this->raw[  6 ] = -sine;		_this->raw[  7 ] = 0.0f;
	_this->raw[  8 ] = 0.0f;		_this->raw[  9 ] = sine;		_this->raw[ 10 ] = cosine;		_this->raw[ 11 ] = 0.0f;
	_this->raw[ 12 ] = 0.0f;		_this->raw[ 13 ] = 0.0f;		_this->raw[ 14 ] = 0.0f;		_this->raw[ 15 ] = 1.0f;
}

static inline void Matrix4ToRotateY( TMatrix4 *_this, const f32 angle )
{
	const f32 radians = SIM_DEG2RAD( angle );
	const f32 sine	= sinf( radians );
	const f32 cosine	= cosf( radians );

	_this->raw[  0 ] = cosine;		_this->raw[  1 ] = 0.0f;		_this->raw[  2 ] = sine;		_this->raw[  3 ] = 0.0f;
	_this->raw[  4 ] = 0.0f;		_this->raw[  5 ] = 1.0f;		_this->raw[  6 ] = 0.0f;		_this->raw[  7 ] = 0.0f;
	_this->raw[  8 ] = -sine;		_this->raw[  9 ] = 0.0f;		_this->raw[ 10 ] = cosine;		_this->raw[ 11 ] = 0.0f;
	_this->raw[ 12 ] = 0.0f;		_this->raw[ 13 ] = 0.0f;		_this->raw[ 14 ] = 0.0f;		_this->raw[ 15 ] = 1.0f;
}

static inline void Matrix4ToRotateZ( TMatrix4 *_this, const f32 angle )
{
	const f32 radians	= SIM_DEG2RAD( angle );
	const f32 sine		= sinf( radians );
	const f32 cosine	= cosf( radians );

	_this->raw[  0 ] = cosine;		_this->raw[  1 ] = -sine;		_this->raw[  2 ] = 0.0f;		_this->raw[  3 ] = 0.0f;
	_this->raw[  4 ] = sine;		_this->raw[  5 ] = cosine;		_this->raw[  6 ] = 0.0f;		_this->raw[  7 ] = 0.0f;
	_this->raw[  8 ] = 0.0f;		_this->raw[  9 ] = 0.0f;		_this->raw[ 10 ] = 1.0f;		_this->raw[ 11 ] = 0.0f;
	_this->raw[ 12 ] = 0.0f;		_this->raw[ 13 ] = 0.0f;		_this->raw[ 14 ] = 0.0f;		_this->raw[ 15 ] = 1.0f;
}

static inline void Matrix4ToScale( TMatrix4 *_this, const TVec3 *scl )
{
	_this->raw[  0 ] = scl->x;		_this->raw[  1 ] = 0.0f;		_this->raw[  2 ] = 0.0f;		_this->raw[  3 ] = 0.0f;
	_this->raw[  4 ] = 0.0f;		_this->raw[  5 ] = scl->y;		_this->raw[  6 ] = 0.0f;		_this->raw[  7 ] = 0.0f;
	_this->raw[  8 ] = 0.0f;		_this->raw[  9 ] = 0.0f;		_this->raw[ 10 ] = scl->z;		_this->raw[ 11 ] = 0.0f;
	_this->raw[ 12 ] = 0.0f;		_this->raw[ 13 ] = 0.0f;		_this->raw[ 14 ] = 0.0f;		_this->raw[ 15 ] = 1.0f;
}

static inline void Matrix4Translate( TMatrix4 *_this, const TVec3 *pos )
{
	_this->raw[  3 ] += _this->raw[  0 ] * pos->x + _this->raw[  1 ] * pos->y + _this->raw[  2 ] * pos->z;
	_this->raw[  7 ] += _this->raw[  4 ] * pos->x + _this->raw[  5 ] * pos->y + _this->raw[  6 ] * pos->z;
	_this->raw[ 11 ] += _this->raw[  8 ] * pos->x + _this->raw[  9 ] * pos->y + _this->raw[ 10 ] * pos->z;
	_this->raw[ 15 ] += _this->raw[ 12 ] * pos->x + _this->raw[ 13 ] * pos->y + _this->raw[ 14 ] * pos->z;
}

static inline void Matrix4Translate( TMatrix4 *_this, const f32 x, const f32 y, const f32 z )
{
	_this->raw[  3 ] += _this->raw[  0 ] * x + _this->raw[  1 ] * y + _this->raw[  2 ] * z;
	_this->raw[  7 ] += _this->raw[  4 ] * x + _this->raw[  5 ] * y + _this->raw[  6 ] * z;
	_this->raw[ 11 ] += _this->raw[  8 ] * x + _this->raw[  9 ] * y + _this->raw[ 10 ] * z;
	_this->raw[ 15 ] += _this->raw[ 12 ] * x + _this->raw[ 13 ] * y + _this->raw[ 14 ] * z;
}

static inline void Matrix4TranslateX( TMatrix4 *_this, const f32 tx )
{
	_this->raw[  3 ] += _this->raw[  0 ] * tx;
	_this->raw[  7 ] += _this->raw[  4 ] * tx;
	_this->raw[ 11 ] += _this->raw[  8 ] * tx;
	_this->raw[ 15 ] += _this->raw[ 12 ] * tx;
}

static inline void Matrix4TranslateY( TMatrix4 *_this, const f32 ty )
{
	_this->raw[  3 ] += _this->raw[  1 ] * ty;
	_this->raw[  7 ] += _this->raw[  5 ] * ty;
	_this->raw[ 11 ] += _this->raw[  9 ] * ty;
	_this->raw[ 15 ] += _this->raw[ 13 ] * ty;
}

static inline void Matrix4TranslateZ( TMatrix4 *_this, const f32 tz )
{
	_this->raw[  3 ] += _this->raw[  2 ] * tz;
	_this->raw[  7 ] += _this->raw[  6 ] * tz;
	_this->raw[ 11 ] += _this->raw[ 10 ] * tz;
	_this->raw[ 15 ] += _this->raw[ 14 ] * tz;
}

static inline void Matrix4RotateX( TMatrix4 *_this, const f32 angle )
{
	const f32 radians	= SIM_DEG2RAD( angle );
	const f32 sine		= sinf( radians );
	const f32 cosine	= cosf( radians );

	TVec4 v;

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
}

static inline void Matrix4RotateY( TMatrix4 *_this, const f32 angle )
{
	const f32 radians = SIM_DEG2RAD( angle );
	const f32 sine	= sinf( radians );
	const f32 cosine	= cosf( radians );

	TVec4 v;

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
}

static inline void Matrix4RotateZ( TMatrix4 *_this, const f32 angle )
{
	const f32 radians = SIM_DEG2RAD( angle );
	const f32 sine	= sinf( radians );
	const f32 cosine	= cosf( radians );

	TVec4 v;

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
}

static inline void Matrix4Scale( TMatrix4 *_this, const TVec3 *scl )
{
	_this->raw[  0 ] *= scl->x;
	_this->raw[  4 ] *= scl->x;
	_this->raw[  8 ] *= scl->x;
	_this->raw[ 12 ] *= scl->x;

	_this->raw[  1 ] *= scl->y;
	_this->raw[  5 ] *= scl->y;
	_this->raw[  9 ] *= scl->y;
	_this->raw[ 13 ] *= scl->y;

	_this->raw[  2 ] *= scl->z;
	_this->raw[  6 ] *= scl->z;
	_this->raw[ 10 ] *= scl->z;
	_this->raw[ 14 ] *= scl->z;
}

static inline void Matrix4Scale( TMatrix4 *_this, const f32 x, const f32 y, const f32 z )
{
	_this->raw[  0 ] *= x;
	_this->raw[  4 ] *= x;
	_this->raw[  8 ] *= x;
	_this->raw[ 12 ] *= x;

	_this->raw[  1 ] *= y;
	_this->raw[  5 ] *= y;
	_this->raw[  9 ] *= y;
	_this->raw[ 13 ] *= y;

	_this->raw[  2 ] *= z;
	_this->raw[  6 ] *= z;
	_this->raw[ 10 ] *= z;
	_this->raw[ 14 ] *= z;
}

static inline void Matrix4ScaleX( TMatrix4 *_this, const f32 sx )
{
	_this->raw[  0 ] *= sx;
	_this->raw[  4 ] *= sx;
	_this->raw[  8 ] *= sx;
	_this->raw[ 12 ] *= sx;
}

static inline void Matrix4ScaleY( TMatrix4 *_this, const f32 sy )
{
	_this->raw[  1 ] *= sy;
	_this->raw[  5 ] *= sy;
	_this->raw[  9 ] *= sy;
	_this->raw[ 13 ] *= sy;
}

static inline void Matrix4ScaleZ( TMatrix4 *_this, const f32 sz )
{
	_this->raw[  2 ] *= sz;
	_this->raw[  6 ] *= sz;
	_this->raw[ 10 ] *= sz;
	_this->raw[ 14 ] *= sz;
}

// ----------------------------------------------------------------------//
}; // namespace mat
}; // namespace sim

#endif // __SIM_MATRIX4_H
