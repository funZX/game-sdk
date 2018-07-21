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

#ifndef __SIM_VEC3_H
#define __SIM_VEC3_H

#include <math/sim_math.h>
#include <math/sim_plane.h>

namespace sim
{
namespace mat
{
// ----------------------------------------------------------------------//

union TVec3
{
	f32 raw[ 3 ];

	struct
	{
		f32 x;
		f32 y;
		f32 z;
	};
};

static const TVec3 Vec3Up		= { 0.0f,  1.0f,  0.0f };
static const TVec3 Vec3Front	= { 0.0f,  0.0f, -1.0f };
static const TVec3 Vec3Side		= { 1.0f,  0.0f,  0.0f };

static inline void Vec3ToZero( TVec3 *_this )
{
	_this->x = 0.0f;
	_this->y = 0.0f;
	_this->z = 0.0f;
}

static inline void Vec3ToOne( TVec3 *_this )
{
	_this->x = 1.0f;
	_this->y = 1.0f;
	_this->z = 1.0f;
}

static inline f32 Vec3Max( const TVec3 *_this )
{
	return SIM_MAX( _this->x, SIM_MAX( _this->y, _this->z ) );
}

static inline f32 Vec3Min( const TVec3 *_this )
{
	return SIM_MIN( _this->x, SIM_MIN( _this->y, _this->z ) );
}

static inline void Vec3ToMinusOne( TVec3 *_this )
{
	_this->x = -1.0f;
	_this->y = -1.0f;
	_this->z = -1.0f;
}

static inline void Vec3ToDirX( TVec3 *_this )
{
	_this->x = 1.0f;
	_this->y = 0.0f;
	_this->z = 0.0f;
}

static inline void Vec3ToRDirX( TVec3 *_this )
{
	_this->x = -1.0f;
	_this->y =  0.0f;
	_this->z =  0.0f;
}

static inline void Vec3ToDirY( TVec3 *_this )
{
	_this->x = 0.0f;
	_this->y = 1.0f;
	_this->z = 0.0f;
}

static inline void Vec3ToRDirY( TVec3 *_this )
{
	_this->x =  0.0f;
	_this->y = -1.0f;
	_this->z =  0.0f;
}

static inline void Vec3ToDirZ( TVec3 *_this )
{
	_this->x = 0.0f;
	_this->y = 0.0f;
	_this->z = 1.0f;
}

static inline void Vec3ToRDirZ( TVec3 *_this )
{
	_this->x =  0.0f;
	_this->y =  0.0f;
	_this->z = -1.0f;
}

static inline void Vec3Copy( TVec3 *_this, const TVec3 *src )
{
	_this->x = src->x;
	_this->y = src->y;
	_this->z = src->z;
}

static inline void Vec3Invert( TVec3 *_this )
{
	_this->x = -_this->x;
	_this->y = -_this->y;
	_this->z = -_this->z;
}


static inline void Vec3Set( TVec3 *_this, const f32 x, const f32 y, const f32 z )
{
	_this->x = x;
	_this->y = y;
	_this->z = z;
}

static inline void Vec3Add( TVec3 *_this, const TVec3 *src )
{
	_this->x += src->x;
	_this->y += src->y;
	_this->z += src->z;
}


static inline void Vec3Add( TVec3 *_this, const TVec3 *src1, const TVec3 *src2 )
{
	_this->x = src1->x + src2->x;
	_this->y = src1->y + src2->y;
	_this->z = src1->z + src2->z;
}

static inline void Vec3Diff( TVec3 *_this, const TVec3 *src1, const TVec3 *src2 )
{
	_this->x = src1->x - src2->x;
	_this->y = src1->y - src2->y;
	_this->z = src1->z - src2->z;
}

static inline void Vec3Diff( TVec3 *_this, const TVec3 *src )
{
	_this->x -= src->x;
	_this->y -= src->y;
	_this->z -= src->z;
}

static inline void Vec3Scale( TVec3 *_this, const f32 k )
{
	_this->x *= k;
	_this->y *= k;
	_this->z *= k;
}

static inline void Vec3Scale( TVec3 *_this, const TVec3 *src, const f32 k )
{
	_this->x = k * src->x;
	_this->y = k * src->y;
	_this->z = k * src->z;
}

static inline void Vec3Multiply( const TVec3 *src1, const TVec3 *src2, TVec3 *dst )
{
	dst->x = src1->x * src2->x;
	dst->y = src1->y * src2->y;
	dst->z = src1->z * src2->z;
}

static inline void Vec3Multiply( TVec3 *_this, const TVec3 *src )
{
	_this->x *= src->x;
	_this->y *= src->y;
	_this->z *= src->z;
}

static inline f32 Vec3Magnitude( const TVec3 *_this )
{
	return Sqrt( ( _this->x * _this->x ) +
				 ( _this->y * _this->y ) +
				 ( _this->z * _this->z ) );
}

static inline f32 Vec3RMagnitude( const TVec3 *_this )
{
	return RSqrt( ( _this->x * _this->x ) +
				  ( _this->y * _this->y ) +
				  ( _this->z * _this->z ) );
}

static inline f32 Vec3Dot( const TVec3 *src1, const TVec3 *src2 )
{
	return ( src1->x * src2->x ) + ( src1->y * src2->y ) + ( src1->z * src2->z );
}

static inline f32 Vec3Dist( const TVec3 *src1, const TVec3 *src2 )
{
	TVec3 v3;

	Vec3Diff( &v3, src1, src2 );

	return Vec3Magnitude( &v3 );
}

static inline f32 Vec3Dist2( const TVec3 *src1, const TVec3 *src2 )
{
	TVec3 v3;

	Vec3Diff( &v3, src1, src2 );

	return Vec3Dot( &v3, &v3 );
}

static inline void Vec3Normalize( TVec3 *_this )
{
	const f32 mag = Vec3RMagnitude( _this );

	_this->x = _this->x * mag;
	_this->y = _this->y * mag;
	_this->z = _this->z * mag;
}

static inline f32 Vec3DistToPlane( const TVec3 *_this, const TPlane *p )
{
	return ( p->a * _this->x +
			 p->b * _this->y +
			 p->c * _this->z +
			 p->d );
}

static inline f32 Vec3DotToDeg( const TVec3 *src1, const TVec3 *src2 )
{
	f32 v = Vec3Dot( src1, src2 );

	if( v ) {
		return SIM_RAD2DEG( acosf( v ) );
	}

	return 0.0f;
}

static inline void Vec3Cross( TVec3 *_this, const TVec3 *src1, const TVec3 *src2 )
{
	_this->x = ( src1->y * src2->z ) - ( src2->y * src1->z );
	_this->y = ( src1->z * src2->x ) - ( src2->z * src1->x );
	_this->z = ( src1->x * src2->y ) - ( src2->x * src1->y );
}

static inline f32 Vec3GetVectorAngle( const TVec3 *src1, const TVec3 *src2 )
{
	const f32 dot = Vec3Dot( src1, src2 ) * Vec3RMagnitude( src1 ) * Vec3RMagnitude( src2 );

	const f32 ang = acosf( dot > 1.0f ? 1.0f : dot );

    return ang;
}


static inline f32 Vec3GetDirectionAngle( const TVec3 *dir1, const TVec3 *dir2 )
{
	const f32 dot = Vec3Dot( dir1, dir2 );

	const f32 ang = acosf( dot > 1.0f ? 1.0f : dot );

    return ang;
}

static inline f32 Vec3GetPitch( const TVec3 *_this )
{
	return SIM_RAD2DEG( atan2( _this->x, _this->y ) );
}


static inline f32 Vec3GetYaw( const TVec3 *_this )
{
	return SIM_DEG2RAD( atan(  Sqrt( _this->x * _this->x + _this->y * _this->y ) / _this->z ) );
}

static inline void Vec3DirFromRot( TVec3 *_this, const TVec3 *rot )
{
	const f32 yaw	= SIM_DEG2RAD( rot->y );
	const f32 pitch	= SIM_DEG2RAD( rot->x );

	_this->x = -sinf( yaw );
	_this->y = -sinf( pitch );
	_this->z = -cosf( yaw );
}

// ----------------------------------------------------------------------//
}; // namespace mat
}; // namespace sim

#endif // __SIM_VEC3_H
