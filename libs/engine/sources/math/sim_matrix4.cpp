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

#include <math/sim_matrix4.h>

namespace sim
{
namespace mat
{
// ----------------------------------------------------------------------//

void Matrix4FromPerspective( TMatrix4 *_this, const sim::f32 fov, const sim::f32 aspect, const sim::f32 znear, const sim::f32 zfar )
{
	const sim::f32 d = znear - zfar;
	const sim::f32 r = SIM_DEG2RAD( fov * 0.5f );
	const sim::f32 c = cosf( r ) / sinf( r );

	_this->raw[  0 ] = c / aspect;	_this->raw[  1 ] = 0.0f;		_this->raw[  2 ] = 0.0f;						_this->raw[  3 ] = 0.0f;
	_this->raw[  4 ] = 0.0f;		_this->raw[  5 ] = c;			_this->raw[  6 ] = 0.0f;						_this->raw[  7 ] = 0.0f;
	_this->raw[  8 ] = 0.0f;		_this->raw[  9 ] = 0.0f;		_this->raw[ 10 ] = ( znear + zfar ) / d;		_this->raw[ 11 ] = 2.0f * zfar * znear / d;
	_this->raw[ 12 ] = 0.0f;		_this->raw[ 13 ] = 0.0f;		_this->raw[ 14 ] = -1.0f;						_this->raw[ 15 ] = 0.0f;
}

// ----------------------------------------------------------------------//

void Matrix4FromOrtho( TMatrix4 *_this, sim::f32 left, sim::f32 right, sim::f32 bottom, sim::f32 top, sim::f32 znear, const sim::f32 zfar )
{
	const sim::f32 rml = right - left;
	const sim::f32 tmb = top - bottom;
	const sim::f32 fmn = zfar - znear;

	_this->raw[  0 ] = 2.0f / rml;		_this->raw[  1 ] = 0.0f;			_this->raw[  2 ] = 0.0f;			_this->raw[  3 ] = -( right + left ) / rml;
	_this->raw[  4 ] = 0.0f;			_this->raw[  5 ] = 2.0f / tmb;		_this->raw[  6 ] = 0.0f;			_this->raw[  7 ] = -( top + bottom ) / tmb;
	_this->raw[  8 ] = 0.0f;			_this->raw[  9 ] = 0.0f;			_this->raw[ 10 ] = -2.0f / fmn;		_this->raw[ 11 ] = -( zfar + znear) / fmn;
	_this->raw[ 12 ] = 0.0f;			_this->raw[ 13 ] = 0.0f;			_this->raw[ 14 ] = 0.0f;			_this->raw[ 15 ] = 1.0f;
}

// ----------------------------------------------------------------------//

void Matrix4FromLookAt( TMatrix4 *_this, const TVec3 *eye, const TVec3 *center, const TVec3 *up )
{
 	TVec3 f, s, u;

	Vec3Diff( &f, center, eye );

	Vec3Normalize( &f );

	Vec3Cross( &s, &f, up );

	Vec3Normalize( &s );

	Vec3Cross( &u, &s, &f );

    _this->raw[  0 ] = s.x;
    _this->raw[  1 ] = s.y;
    _this->raw[  2 ] = s.z;

    _this->raw[  4 ] = u.x;
    _this->raw[  5 ] = u.y;
    _this->raw[  6 ] = u.z;

    _this->raw[  8 ] = -f.x;
    _this->raw[  9 ] = -f.y;
    _this->raw[ 10 ] = -f.z;

	_this->raw[ 12 ] =
	_this->raw[ 13 ] =
	_this->raw[ 14 ] = 0.0f;

	_this->raw[ 15 ] = 1.0f;

	_this->raw[  3 ] = -eye->x * _this->raw[ 0 ] - eye->y * _this->raw[ 1 ] - eye->z * _this->raw[  2 ];
	_this->raw[  7 ] = -eye->x * _this->raw[ 4 ] - eye->y * _this->raw[ 5 ] - eye->z * _this->raw[  6 ];
	_this->raw[ 11 ] = -eye->x * _this->raw[ 8 ] - eye->y * _this->raw[ 9 ] - eye->z * _this->raw[ 10 ];
}

// ----------------------------------------------------------------------//

void Matrix4FromAxisAngle( TMatrix4 *_this, const sim::f32 angle, const TVec3 *axis )
{
	const sim::f32 radians = SIM_DEG2RAD( angle );
	const sim::f32 c = cosf( radians );
	const sim::f32 s = sinf( radians );
	const sim::f32 _1mc = 1.0f - c;
	const sim::f32 xy1mc = axis->x * axis->y * _1mc;
	const sim::f32 xz1mc = axis->x * axis->z * _1mc;
	const sim::f32 yz1mc = axis->y * axis->z * _1mc;
	const sim::f32 xs = axis->x * s;
	const sim::f32 ys = axis->y * s;
	const sim::f32 zs = axis->z * s;

	_this->raw[  0 ] = axis->x * axis->x * _1mc + c;
	_this->raw[  1 ] = xy1mc - zs;
	_this->raw[  2 ] = xz1mc + ys;

	_this->raw[  4 ] = xy1mc + zs;
	_this->raw[  5 ] = axis->y * axis->y * _1mc + c;
	_this->raw[  6 ] = yz1mc - xs;

	_this->raw[  8 ] = xz1mc - ys;
	_this->raw[  9 ] = yz1mc + xs;
	_this->raw[ 10 ] = axis->z * axis->z * _1mc + c;

	_this->raw[  3 ] = 0.0f;
	_this->raw[  7 ] = 0.0f;
	_this->raw[ 11 ] = 0.0f;
	_this->raw[ 12 ] = 0.0f;
	_this->raw[ 13 ] = 0.0f;
	_this->raw[ 14 ] = 0.0f;
	_this->raw[ 15 ] = 1.0f;
}

// ----------------------------------------------------------------------//

void Matrix4Rotate( TMatrix4 *_this, const sim::f32 angle, const TVec3 *rot )
{
	TMatrix4 rotMat;

	Matrix4FromAxisAngle( &rotMat, angle, rot );
	Matrix4Multiply( _this, &rotMat );
}

// ----------------------------------------------------------------------//

void Matrix4Rotate( TMatrix4 *_this, f32 angle, const f32 x, const f32 y, const f32 z )
{
	TMatrix4 rotMat;
	TVec3 rot = { x, y, z };

	Matrix4FromAxisAngle( &rotMat, angle, &rot );
	Matrix4Multiply( _this, &rotMat );
}

// ----------------------------------------------------------------------//

void Matrix4Invert( const TMatrix4 *_this, TMatrix4 *dst )
{
#define SWAP( a, b )		{ sim::f32 *_tmp = a; (a) = (b); (b) = _tmp; }
#define MAT( src, r, c )	( src->raw )[ (c) * 4 + (r) ]

	sim::f32 wtmp[4][8];
	sim::f32 m0, m1, m2, m3, s;
	sim::f32 *r0, *r1, *r2, *r3;

	r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

	r0[0] = MAT(_this,0,0), r0[1] = MAT(_this,0,1),
	r0[2] = MAT(_this,0,2), r0[3] = MAT(_this,0,3),
	r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,

	r1[0] = MAT(_this,1,0), r1[1] = MAT(_this,1,1),
	r1[2] = MAT(_this,1,2), r1[3] = MAT(_this,1,3),
	r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,

	r2[0] = MAT(_this,2,0), r2[1] = MAT(_this,2,1),
	r2[2] = MAT(_this,2,2), r2[3] = MAT(_this,2,3),
	r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,

	r3[0] = MAT(_this,3,0), r3[1] = MAT(_this,3,1),
	r3[2] = MAT(_this,3,2), r3[3] = MAT(_this,3,3),
	r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

	if(fabs(r3[0])>fabs(r2[0])) SWAP(r3, r2);
	if(fabs(r2[0])>fabs(r1[0])) SWAP(r2, r1);
	if(fabs(r1[0])>fabs(r0[0])) SWAP(r1, r0);

	SIM_ASSERT( 0.0f != r0[ 0 ] );

	m1 = r1[0]/r0[0]; m2 = r2[0]/r0[0]; m3 = r3[0]/r0[0];
	s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
	s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
	s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;
	s = r0[4];
	if(s != 0.0) { r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s; }
	s = r0[5];
	if(s != 0.0) { r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s; }
	s = r0[6];
	if(s != 0.0) { r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s; }
	s = r0[7];
	if(s != 0.0) { r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s; }

	if(fabs(r3[1])>fabs(r2[1])) SWAP(r3, r2);
	if(fabs(r2[1])>fabs(r1[1])) SWAP(r2, r1);

	SIM_ASSERT( 0.0f != r1[ 1 ] );

	m2 = r2[1]/r1[1]; m3 = r3[1]/r1[1];
	r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2];
	r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];
	s = r1[4]; if(0.0 != s) { r2[4] -= m2 * s; r3[4] -= m3 * s; }
	s = r1[5]; if(0.0 != s) { r2[5] -= m2 * s; r3[5] -= m3 * s; }
	s = r1[6]; if(0.0 != s) { r2[6] -= m2 * s; r3[6] -= m3 * s; }
	s = r1[7]; if(0.0 != s) { r2[7] -= m2 * s; r3[7] -= m3 * s; }

	if(fabs(r3[2])>fabs(r2[2])) SWAP(r3, r2);

	SIM_ASSERT( 0.0f != r2[ 2 ] );

	m3 = r3[2]/r2[2];
	r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
	r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6],
	r3[7] -= m3 * r2[7];

	SIM_ASSERT( 0.0f != r3[ 3 ] );

	s = 1.0f/r3[3];
	r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;

	m2 = r2[3];
	s  = 1.0f/r2[2];
	r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
	r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
	m1 = r1[3];
	r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
	r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
	m0 = r0[3];
	r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
	r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;

	m1 = r1[2];
	s  = 1.0f/r1[1];
	r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
	r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
	m0 = r0[2];
	r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
	r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;

	m0 = r0[1];
	s  = 1.0f/r0[0];
	r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
	r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);

	MAT(dst,0,0) = (sim::f32) r0[4]; MAT(dst,0,1) = (sim::f32) r0[5],
	MAT(dst,0,2) = (sim::f32) r0[6]; MAT(dst,0,3) = (sim::f32) r0[7],
	MAT(dst,1,0) = (sim::f32) r1[4]; MAT(dst,1,1) = (sim::f32) r1[5],
	MAT(dst,1,2) = (sim::f32) r1[6]; MAT(dst,1,3) = (sim::f32) r1[7],
	MAT(dst,2,0) = (sim::f32) r2[4]; MAT(dst,2,1) = (sim::f32) r2[5],
	MAT(dst,2,2) = (sim::f32) r2[6]; MAT(dst,2,3) = (sim::f32) r2[7],
	MAT(dst,3,0) = (sim::f32) r3[4]; MAT(dst,3,1) = (sim::f32) r3[5],
	MAT(dst,3,2) = (sim::f32) r3[6]; MAT(dst,3,3) = (sim::f32) r3[7];

#undef MAT
#undef SWAP
}

// ----------------------------------------------------------------------//

void Matrix4InvertNoScaling( const TMatrix4 *_this, TMatrix4 *dst )
{
	TMatrix4 tmp;

	tmp.raw[  0 ] = _this->raw[ 0 ];		tmp.raw[  1 ] = _this->raw[ 4 ];		tmp.raw[  2 ] = _this->raw[ 8 ];
	tmp.raw[  4 ] = _this->raw[ 1 ];		tmp.raw[  5 ] = _this->raw[ 5 ];		tmp.raw[  6 ] = _this->raw[ 9 ];
	tmp.raw[  8 ] = _this->raw[ 2 ];		tmp.raw[  9 ] = _this->raw[ 6 ];		tmp.raw[ 10 ] = _this->raw[ 10 ];

	tmp.raw[  3 ] = -( _this->raw[ 3 ] * tmp.raw[ 0 ] + _this->raw[ 7 ] * tmp.raw[ 1 ] + _this->raw[ 11 ] * tmp.raw[  2 ] );
	tmp.raw[  7 ] = -( _this->raw[ 3 ] * tmp.raw[ 4 ] + _this->raw[ 7 ] * tmp.raw[ 5 ] + _this->raw[ 11 ] * tmp.raw[  6 ] );
	tmp.raw[ 11 ] = -( _this->raw[ 3 ] * tmp.raw[ 8 ] + _this->raw[ 7 ] * tmp.raw[ 9 ] + _this->raw[ 11 ] * tmp.raw[ 10 ] );

	tmp.raw[ 12 ] = 0.0f;
	tmp.raw[ 13 ] = 0.0f;
	tmp.raw[ 14 ] = 0.0f;
	tmp.raw[ 15 ] = 1.0f;

	Matrix4Copy( dst, &tmp );
}

// ----------------------------------------------------------------------//

void Matrix4Multiply( TMatrix4 *_this, const TMatrix4 *src )
{
	TMatrix4 tmp;

	for( u32 i = 0; i < 4; i++ )
		for( u32 j = 0; j < 4; j++ )
			tmp.raw[ i * 4 + j ] =  _this->raw[ i * 4 + 0 ] * src->raw[ 0 * 4 + j ] +
									_this->raw[ i * 4 + 1 ] * src->raw[ 1 * 4 + j ] +
									_this->raw[ i * 4 + 2 ] * src->raw[ 2 * 4 + j ] +
									_this->raw[ i * 4 + 3 ] * src->raw[ 3 * 4 + j ];

	Matrix4Copy( _this, &tmp );
}

// ----------------------------------------------------------------------//

void Matrix4Multiply( const TMatrix4 *src1, const TMatrix4 *src2, TMatrix4 *dst )
{
	SIM_ASSERT( src1 != dst );

	for( u32 i = 0; i < 4; i++ )
		for( u32 j = 0; j < 4; j++ )
			dst->raw[ i * 4 + j ] =	 src1->raw[ i * 4 + 0 ] * src2->raw[ 0 * 4 + j ] +
									 src1->raw[ i * 4 + 1 ] * src2->raw[ 1 * 4 + j ] +
									 src1->raw[ i * 4 + 2 ] * src2->raw[ 2 * 4 + j ] +
									 src1->raw[ i * 4 + 3 ] * src2->raw[ 3 * 4 + j ];
}

// ----------------------------------------------------------------------//

void Matrix4TransformHomogen( const TMatrix4 *_this, const TVec3 *src, TVec3 *dst )
{
	sim::f32 w;
	TVec4 tmp;

	tmp.x =		_this->raw[  0 ] * src->x +
				_this->raw[  1 ] * src->y +
				_this->raw[  2 ] * src->z +
				_this->raw[  3 ];

	tmp.y =		_this->raw[  4 ] * src->x +
				_this->raw[  5 ] * src->y +
				_this->raw[  6 ] * src->z +
				_this->raw[  7 ];

	tmp.z =		_this->raw[  8 ] * src->x +
				_this->raw[  9 ] * src->y +
				_this->raw[ 10 ] * src->z +
				_this->raw[ 11 ];

	tmp.w =		_this->raw[ 12 ] * src->x +
				_this->raw[ 13 ] * src->y +
				_this->raw[ 14 ] * src->z +
				_this->raw[ 15 ];

	w		= 1.0f / tmp.w;

	dst->x	= tmp.x * w;
	dst->y	= tmp.y * w;
	dst->z	= tmp.z * w;
}

// ----------------------------------------------------------------------//

void Matrix4Transform( const TMatrix4 *_this, const TVec3 *src, TVec3 *dst )
{
	TVec3 tmp;

	tmp.x =		_this->raw[  0 ] * src->x +
				_this->raw[  1 ] * src->y +
				_this->raw[  2 ] * src->z +
				_this->raw[  3 ];

	tmp.y =		_this->raw[  4 ] * src->x +
				_this->raw[  5 ] * src->y +
				_this->raw[  6 ] * src->z +
				_this->raw[  7 ];

	tmp.z =		_this->raw[  8 ] * src->x +
				_this->raw[  9 ] * src->y +
				_this->raw[ 10 ] * src->z +
				_this->raw[ 11 ];

	Vec3Copy( dst, &tmp );
}

// ----------------------------------------------------------------------//

void Matrix4FromDirection( TMatrix4 *_this, const sim::f32 dx, const sim::f32 dy, const sim::f32 dz , const sim::f32 roll )
{
	const sim::f32 cp		=  Sqrt( 1.0f - ( dy * dy ) );
	const sim::f32 mrn		= -RSqrt( ( dx * dx ) + ( dz * dz ) );
	const sim::f32 sy		= mrn * dx;
	const sim::f32 cy		= mrn * dz;
	const sim::f32 deg		= SIM_DEG2RAD( roll );
	const sim::f32 sr		= sinf( deg );
	const sim::f32 cr		= cosf( deg );
	const sim::f32 srsp	= sr * dy;
	const sim::f32 crsy	= cr * sy;
	const sim::f32 crcy	= cr * cy;

	_this->raw[  0 ] = crcy + srsp * sy;	_this->raw[  4 ] = -sr * cy + crsy * dy;	_this->raw[  8 ] = cp * sy;
	_this->raw[  1 ] = sr * cp;				_this->raw[  5 ] = cr * cp;					_this->raw[  9 ] = -dy;
	_this->raw[  2 ] = -crsy + srsp * cy;	_this->raw[  6 ] = sr * sy + crcy * dy;		_this->raw[ 10 ] = cp * cy;
}

// ----------------------------------------------------------------------//

void Matrix4FromDirection( TMatrix4 *_this, const TVec3 *dir , const sim::f32 roll )
{
	const sim::f32 cp		=  Sqrt( 1.0f - ( dir->y * dir->y ) );
	const sim::f32 mrn		= -RSqrt( ( dir->x * dir->x ) + ( dir->z * dir->z ) );
	const sim::f32 sy		= mrn * dir->x;
	const sim::f32 cy		= mrn * dir->z;
	const sim::f32 deg		= SIM_DEG2RAD( roll );
	const sim::f32 sr		= sinf( deg );
	const sim::f32 cr		= cosf( deg );
	const sim::f32 srsp		= sr * dir->y;
	const sim::f32 crsy		= cr * sy;
	const sim::f32 crcy		= cr * cy;

	_this->raw[  0 ] = crcy + srsp * sy;	_this->raw[  4 ] = -sr * cy + crsy * dir->y;	_this->raw[  8 ] = cp * sy;
	_this->raw[  1 ] = sr * cp;				_this->raw[  5 ] = cr * cp;						_this->raw[  9 ] = -dir->y;
	_this->raw[  2 ] = -crsy + srsp * cy;	_this->raw[  6 ] = sr * sy + crcy * dir->y;		_this->raw[ 10 ] = cp * cy;
}

// ----------------------------------------------------------------------//

void Matrix4FromDirectionNoRoll( TMatrix4 *_this, const sim::f32 dx, const sim::f32 dy, const sim::f32 dz )
{
	const sim::f32 cp	=  Sqrt( 1.0f - ( dy * dy ) );
	const sim::f32 mrn  = -RSqrt( ( dx * dx ) + ( dz * dz ) );
	const sim::f32 sy	=  mrn * dx;
	const sim::f32 cy	=  mrn * dz;

	_this->raw[  0 ] = cy;		_this->raw[  4 ] = dy * sy;		_this->raw[  8 ] = cp * sy;
	_this->raw[  1 ] = 0.0f;	_this->raw[  5 ] = cp;			_this->raw[  9 ] = -dy;
	_this->raw[  2 ] = -sy;		_this->raw[  6 ] = dy * cy;		_this->raw[ 10 ] = cp * cy;
}

// ----------------------------------------------------------------------//

void Matrix4FromDirectionNoRoll( TMatrix4 *_this, const TVec3 *dir )
{
	const sim::f32 cp	=  Sqrt( 1.0f - ( dir->y * dir->y ) );
	const sim::f32 mrn  = -RSqrt( ( dir->x * dir->x ) + ( dir->z * dir->z ) );
	const sim::f32 sy	=  mrn * dir->x;
	const sim::f32 cy	=  mrn * dir->z;

	_this->raw[  0 ] = cy;		_this->raw[  4 ] = dir->y * sy;		_this->raw[  8 ] = cp * sy;
	_this->raw[  1 ] = 0.0f;	_this->raw[  5 ] = cp;				_this->raw[  9 ] = -dir->y;
	_this->raw[  2 ] = -sy;		_this->raw[  6 ] = dir->y * cy;		_this->raw[ 10 ] = cp * cy;
}

// ----------------------------------------------------------------------//

void Matrix4FromQuat( TMatrix4 *_this, const TQuat *q )
{
	const sim::f32 xx = q->x * q->x;	const sim::f32 yy = q->y * q->y;	const sim::f32 zz = q->z * q->z;
	const sim::f32 xw = q->x * q->w;	const sim::f32 yw = q->y * q->w;	const sim::f32 zw = q->z * q->w;
	const sim::f32 xy = q->x * q->y;	const sim::f32 yz = q->y * q->z;	const sim::f32 zx = q->z * q->x;

	// First column
	_this->raw[  0 ] = 1.0f - 2.0f * ( yy + zz );
	_this->raw[  1 ] =		  2.0f * ( xy - zw );
	_this->raw[  2 ] =		  2.0f * ( zx + yw );
	_this->raw[  3 ] = 0.0f;

	// Second column
	_this->raw[  4 ] =		  2.0f * ( xy + zw );
	_this->raw[  5 ] = 1.0f - 2.0f * ( xx + zz );
	_this->raw[  6 ] =		  2.0f * ( yz - xw );
	_this->raw[  7 ] = 0.0f;

	// Third column
	_this->raw[  8 ] =		 2.f * ( zx - yw );
	_this->raw[  9 ] =		 2.f * ( yz + xw );
	_this->raw[ 10 ] = 1.f - 2.f * ( xx + yy );
	_this->raw[ 11 ] = 0.f;

	// Fourth column
	_this->raw[ 12 ] = 0.f;
	_this->raw[ 13 ] = 0.f;
	_this->raw[ 14 ] = 0.f;
	_this->raw[ 15 ] = 1.f;
}

// ----------------------------------------------------------------------//

void Matrix4FromQuatNoConst( TMatrix4 *_this, const TQuat *q )
{
	const sim::f32 xx = q->x * q->x;	const sim::f32 yy = q->y * q->y;	const sim::f32 zz = q->z * q->z;
	const sim::f32 xw = q->x * q->w;	const sim::f32 yw = q->y * q->w;	const sim::f32 zw = q->z * q->w;
	const sim::f32 xy = q->x * q->y;	const sim::f32 yz = q->y * q->z;	const sim::f32 zx = q->z * q->x;

	// First column
	_this->raw[  0 ] = 1.0f - 2.0f * ( yy + zz );
	_this->raw[  1 ] =		  2.0f * ( xy - zw );
	_this->raw[  2 ] =		  2.0f * ( zx + yw );

	// Second column
	_this->raw[  4 ] =		  2.0f * ( xy + zw );
	_this->raw[  5 ] = 1.0f - 2.0f * ( xx + zz );
	_this->raw[  6 ] =		  2.0f * ( yz - xw );

	// Third column
	_this->raw[  8 ] =		  2.0f * ( zx - yw );
	_this->raw[  9 ] =		  2.0f * ( yz + xw );
	_this->raw[ 10 ] = 1.0f - 2.0f * ( xx + yy );
}

// ----------------------------------------------------------------------//

void Matrix4FromQuatNoConstNoDir( TMatrix4 *_this, const TQuat *q )
{
	const sim::f32 xx = q->x * q->x;	const sim::f32 yy = q->y * q->y;	const sim::f32 zz = q->z * q->z;
	const sim::f32 xw = q->x * q->w;	const sim::f32 yw = q->y * q->w;	const sim::f32 zw = q->z * q->w;
	const sim::f32 xy = q->x * q->y;	const sim::f32 yz = q->y * q->z;	const sim::f32 zx = q->z * q->x;

	// First column
	_this->raw[  0 ] = 1.f - 2.0f * ( yy + zz );
	_this->raw[  1 ] =		 2.0f * ( xy - zw );

	// Second column
	_this->raw[  4 ] =		 2.0f * ( xy + zw );
	_this->raw[  5 ] = 1.f - 2.0f * ( xx + zz );

	// Third column
	_this->raw[  8 ] =		  2.0f * ( zx - yw );
	_this->raw[  9 ] =		  2.0f * ( yz + xw );
}

// ----------------------------------------------------------------------//

void Matrix4Print( const TMatrix4 *_this )
{
	SIM_PRINT( "\n" );
	for( s32 i = 0; i < 4; i++ )
	{
		SIM_PRINT( "\n" );
		for( s32 j = 0; j < 4; j++ )
		{
			SIM_PRINT( "%.3f\t", _this->raw[ j + i * 4 ] );
		}
	}
	SIM_PRINT( "\n" );
}

// ----------------------------------------------------------------------//
}; // namespace mat
}; // namespace sim
