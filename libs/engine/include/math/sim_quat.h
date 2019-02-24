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

#ifndef __SIM_QUAT_H
#define __SIM_QUAT_H

#include <math/sim_math.h>
#include <math/sim_vec3.h>

namespace sim
{
namespace mat
{
// ----------------------------------------------------------------------//
union TQuat
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

void QuatSlerp( const TQuat *_this, TQuat *q, const f32 t, TQuat *dst );
void QuatSlerp( const TQuat *_this, TQuat *q, const f32 t, f32 *_cosOmega,  TQuat *dst );

static inline void QuatReset( TQuat *_this )
{
	_this->x = 0.0f;
	_this->y = 0.0f;
	_this->z = 0.0f;
	_this->w = 1.0f;
}

static inline void QuatInvert( TQuat *_this )
{
	_this->x = -_this->x;
	_this->y = -_this->y;
	_this->z = -_this->z;
}

static inline void QuatNormalize( TQuat *_this )
{
	f32 mag =
		SIM_SQR( _this->w ) +
		SIM_SQR( _this->x ) +
		SIM_SQR( _this->y ) +
		SIM_SQR( _this->z );

	if( fabsf( mag ) < 0.99999f )
	{
		mag = RSqrt(mag);
		_this->x *= mag;
		_this->y *= mag;
		_this->z *= mag;
		_this->w *= mag;
	}
}

static inline void QuatCopy( TQuat *_this, const TQuat *q )
{
	_this->x = q->x;
	_this->y = q->y;
	_this->z = q->z;
	_this->w = q->w;
}

static inline void QuatSet( TQuat *_this, f32 x, f32 y, f32 z, f32 w )
{
	_this->x = x;
	_this->y = y;
	_this->z = z;
	_this->w = w;
}

static inline void QuatMultiply( TQuat *_this, const TQuat *q )
{
	TQuat ql;

	ql.x = ( _this->w * q->x ) + ( _this->x * q->w ) + ( _this->y * q->z ) - ( _this->z * q->y );
	ql.y = ( _this->w * q->y ) + ( _this->y * q->w ) + ( _this->z * q->x ) - ( _this->x * q->z );
	ql.z = ( _this->w * q->z ) + ( _this->z * q->w ) + ( _this->x * q->y ) - ( _this->y * q->x );
	ql.w = ( _this->w * q->w ) - ( _this->x * q->x ) - ( _this->y * q->y ) - ( _this->z * q->z );

	QuatCopy( _this, &ql );
}

static inline void QuatMultiply( const TQuat *_this, const TQuat *q, TQuat *dst )
{
	dst->x = ( _this->w * q->x ) + ( _this->x * q->w ) + ( _this->y * q->z ) - ( _this->z * q->y );
	dst->y = ( _this->w * q->y ) + ( _this->y * q->w ) + ( _this->z * q->x ) - ( _this->x * q->z );
	dst->z = ( _this->w * q->z ) + ( _this->z * q->w ) + ( _this->x * q->y ) - ( _this->y * q->x );
	dst->w = ( _this->w * q->w ) - ( _this->x * q->x ) - ( _this->y * q->y ) - ( _this->z * q->z );
}

static inline void QuatRotateX( TQuat *_this, const f32 angle )
{
	TQuat q;

	const f32 ha = SIM_DEG2RAD( angle ) * 0.5f;
	const f32 _x = sinf( ha );
	const f32 _w = cosf( ha );

	q.x = ( _this->w * _x ) + ( _this->x * _w );
	q.y = ( _this->y * _w ) + ( _this->z * _x );
	q.z = ( _this->z * _w ) - ( _this->y * _x );
	q.w = ( _this->w * _w ) - ( _this->x * _x );

	QuatCopy( _this, &q );
}

static inline void QuatRotateX( const TQuat *_this, const f32 angle, TQuat *dst )
{
	const f32 ha = SIM_DEG2RAD( angle ) * 0.5f;
	const f32 _x = sinf( ha );
	const f32 _w = cosf( ha );

	dst->x = ( _this->w * _x ) + ( _this->x * _w );
	dst->y = ( _this->y * _w ) + ( _this->z * _x );
	dst->z = ( _this->z * _w ) - ( _this->y * _x );
	dst->w = ( _this->w * _w ) - ( _this->x * _x );
}

static inline void QuatRotateY( TQuat *_this, const f32 angle )
{
	TQuat q;

	const f32 ha = SIM_DEG2RAD( angle ) * 0.5f;
	const f32 _y = sinf( ha );
	const f32 _w = cosf( ha );

	q.x = ( _this->x * _w ) - ( _this->z * _y );
	q.y = ( _this->w * _y ) + ( _this->y * _w );
	q.z = ( _this->z * _w ) + ( _this->x * _y );
	q.w = ( _this->w * _w ) - ( _this->y * _y );

	QuatCopy( _this, &q );
}

static inline void QuatRotateY( const TQuat *_this, const f32 angle, TQuat *dst )
{
	const f32 ha = SIM_DEG2RAD( angle ) * 0.5f;
	const f32 _y = sinf( ha );
	const f32 _w = cosf( ha );

	dst->x = ( _this->x * _w ) - ( _this->z * _y );
	dst->y = ( _this->w * _y ) + ( _this->y * _w );
	dst->z = ( _this->z * _w ) + ( _this->x * _y );
	dst->w = ( _this->w * _w ) - ( _this->y * _y );
}

static inline void QuatRotateZ( TQuat *_this, const f32 angle )
{
	TQuat q;

	const f32 ha = SIM_DEG2RAD( angle ) * 0.5f;
	const f32 _z = sinf( ha );
	const f32 _w = cosf( ha );

	q.x = ( _this->x * _w ) + ( _this->y * _z );
	q.y = ( _this->y * _w ) - ( _this->x * _z );
	q.z = ( _this->w * _z ) + ( _this->z * _w );
	q.w = ( _this->w * _w ) - ( _this->z * _z );

	QuatCopy( _this, &q );
}

static inline void QuatRotateZ( const TQuat *_this, const f32 angle, TQuat *dst )
{
	const f32 ha = SIM_DEG2RAD( angle ) * 0.5f;
	const f32 _z = sinf( ha );
	const f32 _w = cosf( ha );

	dst->x = ( _this->x * _w ) + ( _this->y * _z );
	dst->y = ( _this->y * _w ) - ( _this->x * _z );
	dst->z = ( _this->w * _z ) + ( _this->z * _w );
	dst->w = ( _this->w * _w ) - ( _this->z * _z );
}

static inline void QuatFromAxisAngle( TQuat *_this, const f32 angle, const f32 ax, const f32 ay, const f32 az )
{
	const f32 ha = SIM_DEG2RAD( angle ) * 0.5f;
	const f32 _sin = sinf( ha );

	_this->x = ax * _sin;
	_this->y = ay * _sin;
	_this->z = az * _sin;
	_this->w = cosf( ha );
}

static inline void QuatFromAxisAngle( TQuat *_this, const f32 angle, const TVec3 *axis )
{
	const f32 ha = SIM_DEG2RAD( angle ) * 0.5f;
	const f32 _sin = sinf( ha );

	_this->x = axis->x * _sin;
	_this->y = axis->y * _sin;
	_this->z = axis->z * _sin;
	_this->w = cosf( ha );
}

static inline f32 QuatGetDirX( const TQuat *_this )
{
	return -2.0f * ( ( _this->z * _this->x ) + ( _this->y * _this->w ) );
}

static inline f32 QuatGetDirY( const TQuat *_this )
{
	return 2.0f * ( ( _this->x * _this->w ) - (_this->y * _this->z ) );
}

static inline f32 QuatGetDirZ( const TQuat *_this )
{
	return ( 2.0f * ( ( _this->x * _this->x ) + ( _this->y * _this->y ) ) ) - 1.0f;
}

static inline void QuatGetDir( const TQuat *_this, TVec3 *dir )
{
	dir->x = -2.0f * ( ( _this->z * _this->x ) + ( _this->y * _this->w ) );
	dir->y =  2.0f * ( ( _this->x * _this->w ) - ( _this->y * _this->z ) );
	dir->z =( 2.0f * ( ( _this->x * _this->x ) + ( _this->y * _this->y ) ) ) - 1.0f;
}

static inline void QuatGetDir( const TQuat *_this, f32 *dirx, f32 *diry, f32 *dirz )
{
	*dirx = -2.0f * ( ( _this->z * _this->x ) + ( _this->y * _this->w ) );
	*diry =  2.0f * ( ( _this->x * _this->w ) - ( _this->y * _this->z ) );
	*dirz =( 2.0f * ( ( _this->x * _this->x ) + ( _this->y * _this->y ) ) ) - 1.0f;
}

static inline f32 QuatGetRotX( const TQuat *_this )
{
	return asinf ( 2.0f * ( ( _this->x * _this->w ) - ( _this->y * _this->z ) ) );
}

static inline f32 QuatGetRotY( const TQuat *_this )
{
	return atan2f( 2.0f * ( ( _this->z * _this->x ) + ( _this->y * _this->w ) ),
				 ( 2.0f * ( ( _this->x * _this->x ) + ( _this->y * _this->y ) ) ) - 1.0f);
}

static inline f32 QuatGetRotZ( const TQuat *_this )
{
	return atan2f( 2.0f * ( ( _this->x * _this->y ) + ( _this->w * _this->z ) ),
				   1.0f - ( 2.0f * ( ( _this->x * _this->x ) + ( _this->z * _this->z) ) ) );
}

static inline void QuatGetRot( const TQuat *_this, TVec3 *rot )
{
	rot->x = asinf ( 2.0f * ( ( _this->x * _this->w ) - ( _this->y * _this->z ) ) );

	rot->y = atan2f( 2.0f * ( ( _this->z * _this->x ) + ( _this->y * _this->w ) ),
				   ( 2.0f * ( ( _this->x * _this->x ) + ( _this->y * _this->y ) ) ) - 1.0f);

	rot->z = atan2f( 2.0f * ( ( _this->x * _this->y ) + ( _this->w * _this->z ) ),
					 1.0f - ( 2.0f * ( ( _this->x * _this->x ) + ( _this->z * _this->z) ) ) );
}

static inline void QuatGetRot( const TQuat *_this, f32 *rotx, f32 *roty, f32 *rotz )
{
	*rotx = asinf ( 2.0f * ( ( _this->x * _this->w ) - ( _this->y * _this->z ) ) );

	*roty = atan2f( 2.0f * ( ( _this->z * _this->x ) + ( _this->y * _this->w ) ),
					( 2.0f * ( ( _this->x * _this->x ) + ( _this->y * _this->y ) ) ) - 1.0f);

	*rotz = atan2f( 2.0f * ( ( _this->x * _this->y ) + ( _this->w * _this->z ) ),
					1.0f - ( 2.0f * ( ( _this->x * _this->x ) + ( _this->z * _this->z) ) ) );
}

static inline void QuatGetDirRot( const TQuat *_this, TVec3 *dir, TVec3 *rot )
{
	const f32  xx = _this->x * _this->x;

	dir->x = -2.0f * ( ( _this->z * _this->x ) + ( _this->y * _this->w ) );
	dir->y =  2.0f * ( ( _this->x * _this->w ) - ( _this->y * _this->z ) );
	dir->z =( 2.0f * ( xx + ( _this->y * _this->y ) ) ) - 1.0f;

	rot->x = asinf( dir->y );
	rot->y = atan2f( -dir->x, dir->z );
	rot->z = atan2f( 2.0f * ( ( _this->x * _this->y ) + ( _this->w * _this->z ) ),
					 1.0f - ( 2.f * ( xx + ( _this->z * _this->z ) ) ) );
}

static inline void QuatTransformVec3( const TQuat *_this, TVec3 *v )
{
	const f32 tx =  _this->w *v->x + _this->y * v->z - _this->z * v->y;
	const f32 ty =  _this->w *v->y - _this->x * v->z + _this->z * v->x;
	const f32 tz =  _this->w *v->z + _this->x * v->y - _this->y * v->x;

	const f32 tw = -( _this->x * v->x + _this->y * v->y + _this->z * v->z );

	v->x = -tw * _this->x + tx * _this->w - ty * _this->z + tz * _this->y;
	v->y = -tw * _this->y + ty * _this->w - tz * _this->x + tx * _this->z;
	v->z = -tw * _this->z + tz * _this->w - tx * _this->y + ty * _this->x;
}

static inline void QuatTransformVec3Inverse( const TQuat *_this, TVec3 *v )
{
	const f32 tx = _this->w * v->x - _this->y * v->z + _this->z * v->y;
	const f32 ty = _this->w * v->y + _this->x * v->z - _this->z * v->x;
	const f32 tz = _this->w * v->z - _this->x * v->y + _this->y * v->x;
	const f32 tw = _this->x * v->x + _this->y * v->y + _this->z * v->z;

	v->x = tw * _this->x + tx * _this->w + ty * _this->z - tz * _this->y;
	v->y = tw * _this->y + ty * _this->w + tz * _this->x - tx * _this->z;
	v->z = tw * _this->z + tz * _this->w + tx * _this->y - ty * _this->x;
}

// ----------------------------------------------------------------------//
}; // namespace mat
}; // namespace sim

#endif // __SIM_QUAT_H
