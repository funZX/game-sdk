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

#include <core/sim_core.h>

#include <render/scene/sim_camera.h>
#include <render/sim_rect_2d.h>
#include <render/sim_driver.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CCamera::CCamera()
	: CSceneNode()
{
	zpl_mat4_identity( &m_perspectiveMatrix );
    zpl_mat4_identity( &m_orthographicMatrix );

    m_fieldOfView	= 60.0f;
	m_nearPlane		= 0.1f;
	m_farPlane		= 100.0f;
}

// ----------------------------------------------------------------------//

CCamera::CCamera( const std::string &name )
	: CCamera()
{
	m_name = name;
}

// ----------------------------------------------------------------------//

CCamera::~CCamera()
{

}

// ----------------------------------------------------------------------//

void CCamera::SetPerspective( CRect2D *rect )
{
	Vec2 size;
	size.x = ( f32 ) rect->Width();
	size.y = ( f32 ) rect->Height();

    zpl_mat4_perspective( &m_perspectiveMatrix, m_fieldOfView, size.x / size.y, m_nearPlane, m_farPlane );
}

// ----------------------------------------------------------------------//

void CCamera::SetOrthographic( CRect2D *rect)
{
	Vec2 size;
	size.x = ( f32 ) rect->Width();
	size.y = ( f32 ) rect->Height();

    zpl_mat4_ortho2d( &m_orthographicMatrix, rect->Left(), rect->Right(), rect->Bottom(), rect->Top() );
}

// ----------------------------------------------------------------------//

void CCamera::Update( f32 dt, void *userData )
{
	Vec3 minusPos;

	minusPos = m_transform.translation;
    zpl_vec3_mul( &minusPos, minusPos, -1.0f );
    zpl_mat4_from_quat( &m_matrix, m_transform.quaternion );
    
    m_matrix.w.xyz = minusPos;
    m_matrix.w.w   = 1;

	ExtractClipPlanes();
}

// ----------------------------------------------------------------------//

void CCamera::Render( CDriver *driver )
{
	driver->SetEyePosition( m_transform.translation );
	driver->SetEyeDirection( m_matrix.y.xyz );
}

// ----------------------------------------------------------------------//

void CCamera::ExtractClipPlanes( void )
{
	static Mat4 projViewMatrix;

    zpl_mat4_mul( &projViewMatrix, &m_perspectiveMatrix, &m_matrix );

	register Plane *fp;
	register f32 *pv = (f32*) &projViewMatrix;
    register f32 rmag;

	fp = &m_leftClipPlane;

    fp->a = pv[ 12 ] + pv[  0 ];
    fp->b = pv[ 13 ] + pv[  1 ];
    fp->c = pv[ 14 ] + pv[  2 ];
    fp->d = pv[ 15 ] + pv[  3 ];

    rmag = zpl_rsqrt( zpl_square( fp->a ) + zpl_square( fp->b ) + zpl_square( fp->c ) );

	fp->a *= rmag;
	fp->b *= rmag;
	fp->c *= rmag;
	fp->d *= rmag;

	fp = &m_rightClipPlane;

    fp->a = pv[ 12 ] - pv[  0 ];
    fp->b = pv[ 13 ] - pv[  1 ];
    fp->c = pv[ 14 ] - pv[  2 ];
    fp->d = pv[ 15 ] - pv[  3 ];

    rmag = zpl_rsqrt(zpl_square( fp->a ) + zpl_square( fp->b ) + zpl_square( fp->c ) );

	fp->a *= rmag;
	fp->b *= rmag;
	fp->c *= rmag;
	fp->d *= rmag;

	fp = &m_topClipPlane;

    fp->a = pv[ 12 ] - pv[  4 ];
    fp->b = pv[ 13 ] - pv[  5 ];
    fp->c = pv[ 14 ] - pv[  6 ];
    fp->d = pv[ 15 ] - pv[  7 ];

    rmag = zpl_rsqrt(zpl_square( fp->a ) + zpl_square( fp->b ) + zpl_square( fp->c ) );

	fp->a *= rmag;
	fp->b *= rmag;
	fp->c *= rmag;
	fp->d *= rmag;

	fp = &m_bottomClipPlane;

    fp->a = pv[ 12 ] + pv[  4 ];
    fp->b = pv[ 13 ] + pv[  5 ];
    fp->c = pv[ 14 ] + pv[  6 ];
    fp->d = pv[ 15 ] + pv[  7 ];

    rmag = zpl_rsqrt(zpl_square( fp->a ) + zpl_square( fp->b ) + zpl_square( fp->c ) );

	fp->a *= rmag;
	fp->b *= rmag;
	fp->c *= rmag;
	fp->d *= rmag;;

	fp = &m_nearClipPlane;

    fp->a = pv[ 12 ] + pv[  8 ];
    fp->b = pv[ 13 ] + pv[  9 ];
    fp->c = pv[ 14 ] + pv[ 10 ];
    fp->d = pv[ 15 ] + pv[ 11 ];

    rmag = zpl_rsqrt(zpl_square( fp->a ) + zpl_square( fp->b ) + zpl_square( fp->c ) );

	fp->a *= rmag;
	fp->b *= rmag;
	fp->c *= rmag;
	fp->d *= rmag;

	fp = &m_farClipPlane;

    fp->a = pv[ 12 ] - pv[  8 ];
    fp->b = pv[ 13 ] - pv[  9 ];
    fp->c = pv[ 14 ] - pv[ 10 ];
    fp->d = pv[ 15 ] - pv[ 11 ];

    rmag = zpl_rsqrt(zpl_square( fp->a ) + zpl_square( fp->b ) + zpl_square( fp->c ) );

	fp->a *= rmag;
	fp->b *= rmag;
	fp->c *= rmag;
	fp->d *= rmag;
}

// ----------------------------------------------------------------------//

bool CCamera::SphereIn( Vec3 *pos, const f32 rad )
{
	if(zpl_plane_distance( &m_leftClipPlane, pos )	<= -rad ) return false;
	if(zpl_plane_distance( &m_rightClipPlane, pos)	<= -rad ) return false;
	if(zpl_plane_distance( &m_nearClipPlane, pos)	<= -rad ) return false;
	if(zpl_plane_distance( &m_farClipPlane, pos)	<= -rad ) return false;
	if(zpl_plane_distance( &m_topClipPlane, pos)	<= -rad ) return false;
	if(zpl_plane_distance( &m_bottomClipPlane, pos)	<= -rad ) return false;

	return true;
}

// ----------------------------------------------------------------------//

bool CCamera::PointIn( Vec3 *pos )
{
	if(zpl_plane_distance( &m_leftClipPlane, pos)	<= 0.0f ) return false;
	if(zpl_plane_distance( &m_rightClipPlane, pos)	<= 0.0f ) return false;
	if(zpl_plane_distance( &m_nearClipPlane, pos)	<= 0.0f ) return false;
	if(zpl_plane_distance( &m_farClipPlane, pos)	<= 0.0f ) return false;
	if(zpl_plane_distance( &m_topClipPlane, pos)	<= 0.0f ) return false;
	if(zpl_plane_distance( &m_bottomClipPlane, pos)	<= 0.0f ) return false;

	return true;
}

// ----------------------------------------------------------------------//

bool CCamera::BoxIn( Vec3 *pos, Vec3 *bounds )
{
	static Vec3 v, b;

    b = zpl_vec3f( -bounds->x, -bounds->y, -bounds->z );
    zpl_vec3_add( &v, v, *pos );

	if( PointIn( &v ) ) {
		return true;
	}

    b = zpl_vec3f( +bounds->x, -bounds->y, -bounds->z );
    zpl_vec3_add(&v, v, *pos);

	if( PointIn( &v ) ) {
		return true;
	}

    b = zpl_vec3f( -bounds->x, +bounds->y, -bounds->z );
    zpl_vec3_add(&v, v, *pos);

	if( PointIn( &v ) ) {
		return true;
	}

    b = zpl_vec3f( +bounds->x, +bounds->y, -bounds->z );
    zpl_vec3_add(&v, v, *pos);

	if( PointIn( &v ) ) {
		return true;
	}

    b = zpl_vec3f( +bounds->x, +bounds->y, +bounds->z );
    zpl_vec3_add(&v, v, *pos);

	if( PointIn( &v ) ) {
		return true;
	}

    b = zpl_vec3f( -bounds->x, +bounds->y, +bounds->z );
    zpl_vec3_add(&v, v, *pos);

	if( PointIn( &v ) ) {
		return true;
	}

    b = zpl_vec3f( -bounds->x, -bounds->y, +bounds->z );
    zpl_vec3_add(&v, v, *pos);

	if( PointIn( &v ) ) {
		return true;
	}

    b = zpl_vec3f( +bounds->x, -bounds->y, +bounds->z );
    zpl_vec3_add(&v, v, *pos);

	if( PointIn( &v ) ) {
		return true;
	}

	return false;
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
