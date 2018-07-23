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

#include <math/sim_vec2.h>
#include <math/sim_vec3.h>
#include <math/sim_matrix4.h>

#include <render/scene/sim_camera.h>

#include <render/sim_rect_2d.h>
#include <render/sim_driver.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CCamera::CCamera( const std::string &name )
	: CSceneNode( name )
{
	Matrix4ToIdentity( &m_perspectiveMatrix );
	Matrix4ToIdentity( &m_orthographicMatrix );

	Matrix4ToIdentity( &m_orientationMatrix );
	Matrix4ToIdentity( &m_viewMatrix );

	Matrix4GetFront( &m_orientationMatrix, &m_direction );

	Vec3Invert( &m_direction );
	Vec3ToZero( &m_position );
	Vec3Copy( &m_lookAt, &Vec3Front );

	m_speed			= 1.0f;

	m_fieldOfView	= 60.0f;
	m_nearPlane		= 0.1f;
	m_farPlane		= 100.0f;
}

// ----------------------------------------------------------------------//

CCamera::~CCamera()
{

}

// ----------------------------------------------------------------------//

void CCamera::SetPerspective( CRect2D *rect )
{
	TVec2 size;
	size.x = ( f32 ) rect->Width();
	size.y = ( f32 ) rect->Height();

	Matrix4FromPerspective( &m_perspectiveMatrix, m_fieldOfView, size.x / size.y, m_nearPlane, m_farPlane );
}

// ----------------------------------------------------------------------//

void CCamera::SetOrthographic( CRect2D *rect)
{
	TVec2 size;
	size.x = ( f32 ) rect->Width();
	size.y = ( f32 ) rect->Height();

	Matrix4FromOrtho( &m_orthographicMatrix, 0.0f, size.x, size.y, 0.0f, -1.0f, 1.0f );
}

// ----------------------------------------------------------------------//

void CCamera::Move( f32 dt, bool forward )
{
	TVec3 v;
	f32 speed = m_speed * dt;

	if( !forward ) {
	    speed = -speed;
	}

	Vec3Scale( &v, &m_direction, speed );
	Vec3Add( &m_position, &v );
}

// ----------------------------------------------------------------------//

void CCamera::Strafe( f32 dt, bool left )
{
	TVec3 v;
	f32 speed = m_speed * dt;

	if( left ) {
	    speed = -speed;
	}

	Matrix4GetSide( &m_orientationMatrix, &v );
	Vec3Scale( &v, speed );
	Vec3Add( &m_position, &m_position, &v );
}

// ----------------------------------------------------------------------//

void CCamera::Update( f32 dt, void *userData )
{
	TVec3 minusPos;

	Vec3Copy( &minusPos, &m_position );
	Vec3Invert( &minusPos );

	Matrix4FromDirectionNoRoll( &m_orientationMatrix, &m_direction );
	Matrix4Copy( &m_viewMatrix, &m_orientationMatrix );
	Matrix4Translate( &m_viewMatrix, &minusPos );

	ExtractClipPlanes();
}

// ----------------------------------------------------------------------//

void CCamera::Render( CDriver *driver )
{
	driver->SetEyePosition( &m_position );
	driver->SetEyeDirection( &m_direction );
}

// ----------------------------------------------------------------------//

void CCamera::ExtractClipPlanes( void )
{
	static TMatrix4 projViewMatrix;

	Matrix4Multiply( &m_perspectiveMatrix, &m_viewMatrix, &projViewMatrix );

	register TPlane *fp;
	register f32 *pv = (f32*) &projViewMatrix;
    register f32 rmag;

	fp = &m_leftClipPlane;

    fp->a = pv[ 12 ] + pv[  0 ];
    fp->b = pv[ 13 ] + pv[  1 ];
    fp->c = pv[ 14 ] + pv[  2 ];
    fp->d = pv[ 15 ] + pv[  3 ];

    rmag = RSqrt( SIM_SQR( fp->a ) + SIM_SQR( fp->b ) + SIM_SQR( fp->c ) );

	fp->a *= rmag;
	fp->b *= rmag;
	fp->c *= rmag;
	fp->d *= rmag;

	fp = &m_rightClipPlane;

    fp->a = pv[ 12 ] - pv[  0 ];
    fp->b = pv[ 13 ] - pv[  1 ];
    fp->c = pv[ 14 ] - pv[  2 ];
    fp->d = pv[ 15 ] - pv[  3 ];

    rmag = RSqrt( SIM_SQR( fp->a ) + SIM_SQR( fp->b ) + SIM_SQR( fp->c ) );

	fp->a *= rmag;
	fp->b *= rmag;
	fp->c *= rmag;
	fp->d *= rmag;

	fp = &m_topClipPlane;

    fp->a = pv[ 12 ] - pv[  4 ];
    fp->b = pv[ 13 ] - pv[  5 ];
    fp->c = pv[ 14 ] - pv[  6 ];
    fp->d = pv[ 15 ] - pv[  7 ];

    rmag = RSqrt( SIM_SQR( fp->a ) + SIM_SQR( fp->b ) + SIM_SQR( fp->c ) );

	fp->a *= rmag;
	fp->b *= rmag;
	fp->c *= rmag;
	fp->d *= rmag;

	fp = &m_bottomClipPlane;

    fp->a = pv[ 12 ] + pv[  4 ];
    fp->b = pv[ 13 ] + pv[  5 ];
    fp->c = pv[ 14 ] + pv[  6 ];
    fp->d = pv[ 15 ] + pv[  7 ];

    rmag = RSqrt( SIM_SQR( fp->a ) + SIM_SQR( fp->b ) + SIM_SQR( fp->c ) );

	fp->a *= rmag;
	fp->b *= rmag;
	fp->c *= rmag;
	fp->d *= rmag;;

	fp = &m_nearClipPlane;

    fp->a = pv[ 12 ] + pv[  8 ];
    fp->b = pv[ 13 ] + pv[  9 ];
    fp->c = pv[ 14 ] + pv[ 10 ];
    fp->d = pv[ 15 ] + pv[ 11 ];

    rmag = RSqrt( SIM_SQR( fp->a ) + SIM_SQR( fp->b ) + SIM_SQR( fp->c ) );

	fp->a *= rmag;
	fp->b *= rmag;
	fp->c *= rmag;
	fp->d *= rmag;

	fp = &m_farClipPlane;

    fp->a = pv[ 12 ] - pv[  8 ];
    fp->b = pv[ 13 ] - pv[  9 ];
    fp->c = pv[ 14 ] - pv[ 10 ];
    fp->d = pv[ 15 ] - pv[ 11 ];

    rmag = RSqrt( SIM_SQR( fp->a ) + SIM_SQR( fp->b ) + SIM_SQR( fp->c ) );

	fp->a *= rmag;
	fp->b *= rmag;
	fp->c *= rmag;
	fp->d *= rmag;
}

// ----------------------------------------------------------------------//

bool CCamera::SphereIn( const TVec3 *pos, const f32 rad ) const
{
	if( Vec3DistToPlane( pos, &m_leftClipPlane )	<= -rad ) return false;
	if( Vec3DistToPlane( pos, &m_rightClipPlane )	<= -rad ) return false;
	if( Vec3DistToPlane( pos, &m_nearClipPlane )	<= -rad ) return false;
	if( Vec3DistToPlane( pos, &m_farClipPlane )	<= -rad ) return false;
	if( Vec3DistToPlane( pos, &m_topClipPlane )	<= -rad ) return false;
	if( Vec3DistToPlane( pos, &m_bottomClipPlane )	<= -rad ) return false;

	return true;
}

// ----------------------------------------------------------------------//

bool CCamera::PointIn( const TVec3 *pos ) const
{
	if( Vec3DistToPlane( pos, &m_leftClipPlane )	<= 0.0f ) return false;
	if( Vec3DistToPlane( pos, &m_rightClipPlane )	<= 0.0f ) return false;
	if( Vec3DistToPlane( pos, &m_nearClipPlane )	<= 0.0f ) return false;
	if( Vec3DistToPlane( pos, &m_farClipPlane )	<= 0.0f ) return false;
	if( Vec3DistToPlane( pos, &m_topClipPlane )	<= 0.0f ) return false;
	if( Vec3DistToPlane( pos, &m_bottomClipPlane )	<= 0.0f ) return false;

	return true;
}

// ----------------------------------------------------------------------//

bool CCamera::BoxIn( const TVec3 *pos, const TVec3 *bounds )
{
	static TVec3 v;

	Vec3Set( &v, -bounds->x, -bounds->y, -bounds->z );
	Vec3Add( &v, pos );

	if( PointIn( &v ) ) {
		return true;
	}

	Vec3Set( &v, +bounds->x, -bounds->y, -bounds->z );
	Vec3Add( &v, pos );

	if( PointIn( &v ) ) {
		return true;
	}

	Vec3Set( &v, -bounds->x, +bounds->y, -bounds->z );
	Vec3Add( &v, pos );

	if( PointIn( &v ) ) {
		return true;
	}

	Vec3Set( &v, +bounds->x, +bounds->y, -bounds->z );
	Vec3Add( &v, pos );

	if( PointIn( &v ) ) {
		return true;
	}

	Vec3Set( &v, +bounds->x, +bounds->y, +bounds->z );
	Vec3Add( &v, pos );

	if( PointIn( &v ) ) {
		return true;
	}

	Vec3Set( &v, -bounds->x, +bounds->y, +bounds->z );
	Vec3Add( &v, pos );

	if( PointIn( &v ) ) {
		return true;
	}

	Vec3Set( &v, -bounds->x, -bounds->y, +bounds->z );
	Vec3Add( &v, pos );

	if( PointIn( &v ) ) {
		return true;
	}

	Vec3Set( &v, +bounds->x, -bounds->y, +bounds->z );
	Vec3Add( &v, pos );

	if( PointIn( &v ) ) {
		return true;
	}

	return false;
}

// ----------------------------------------------------------------------//

bool CCamera::BoxIn( const TVec3 *pos, const TVec3 *bounds, const TMatrix4 *orientMat )
{
	static TVec3		    v;
	static TMatrix4		m;

	Matrix4Transpose( orientMat, &m );

	Vec3Set( &v, -bounds->x, -bounds->y, -bounds->z );
	Matrix4Transform( &m, &v, &v );
	Vec3Add( &v, pos );

	if( PointIn( &v ) ) {
		return true;
	}

	Vec3Set( &v, +bounds->x, -bounds->y, -bounds->z );
	Matrix4Transform( &m, &v, &v );
	Vec3Add( &v, pos );

	if( PointIn( &v ) ) {
		return true;
	}

	Vec3Set( &v, -bounds->x, +bounds->y, -bounds->z );
	Matrix4Transform( &m, &v, &v );
	Vec3Add( &v, pos );

	if( PointIn( &v ) ) {
		return true;
	}

	Vec3Set( &v, +bounds->x, +bounds->y, -bounds->z );
	Matrix4Transform( &m, &v, &v );
	Vec3Add( &v, pos );

	if( PointIn( &v ) ) {
		return true;
	}

	Vec3Set( &v, +bounds->x, +bounds->y, +bounds->z );
	Matrix4Transform( &m, &v, &v );
	Vec3Add( &v, pos );

	if( PointIn( &v ) ) {
		return true;
	}

	Vec3Set( &v, -bounds->x, +bounds->y, +bounds->z );
	Matrix4Transform( &m, &v, &v );
	Vec3Add( &v, pos );

	if( PointIn( &v ) ) {
		return true;
	}

	Vec3Set( &v, -bounds->x, -bounds->y, +bounds->z );
	Matrix4Transform( &m, &v, &v );
	Vec3Add( &v, pos );

	if( PointIn( &v ) ) {
		return true;
	}

	Vec3Set( &v, +bounds->x, -bounds->y, +bounds->z );
	Matrix4Transform( &m, &v, &v );
	Vec3Add( &v, pos );

	if( PointIn( &v ) ) {
		return true;
	}

	return false;
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
