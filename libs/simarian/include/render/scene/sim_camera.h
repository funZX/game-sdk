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

#ifndef __SIM_CAMERA_H
#define __SIM_CAMERA_H

#include <render/scene/sim_scene_node.h>

#include <math/sim_matrix4.h>
#include <math/sim_plane.h>
#include <math/sim_vec3.h>

#include <render/sim_render.h>


namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

class CRect2D;
class CDriver;
class CShader;

class CCamera : public CSceneNode
{
public:
	// ------------------------------------------------------------------//
	CCamera( const std::string &name );
	virtual ~CCamera();
	// ------------------------------------------------------------------//
	void							SetPerspective( CRect2D *canvas );
	void							SetOrthographic( CRect2D *canvas );

	inline TMatrix4*				GetPerspectiveMatrix() { return &m_perspectiveMatrix; }
	inline TMatrix4*				GetOrthographicMatrix() { return &m_orthographicMatrix; }

	inline TMatrix4*				GetOrientationMatrix() { return &m_orientationMatrix; }
	inline TMatrix4*				GetViewMatrix() { return &m_viewMatrix; }

	inline void						SetDirection( TVec3 *dir )
									{  m_direction.x = dir->x; m_direction.y = dir->y; m_direction.z = dir->z; };

	inline TVec3*					GetDirection() { return &m_direction; };

	inline void						SetPosition( TVec3 *pos )
									{  m_position.x = pos->x; m_position.y = pos->y; m_position.z = pos->z; };

	inline TVec3*					GetPosition()	{ return &m_position; };

	inline void						SetFieldOfView( f32 fov ) { m_fieldOfView = fov ; }
	inline f32						GetFieldOfView()		{ return m_fieldOfView; }

	inline void						SetSpeed( f32 spd ) { m_speed = spd ; }
	inline f32						GetSpeed()		{ return m_speed; }

	inline void						SetNearPlane( f32 nearPlane ) { m_nearPlane = nearPlane; }
	inline f32						GetNearPlane()	{ return m_nearPlane; }

	inline void						SetFarPlane( f32 farPlane ) { m_farPlane = farPlane; }
	inline f32						GetFarPlane()		{ return m_farPlane; };

	void							Move( f32 dt, bool forward );
	void							Strafe( f32 dt, bool left );

	void							LookAt( TVec3 *look )		{ Vec3Copy( &m_lookAt, look ); }
	void							LookAt( f32 x, f32 y, f32 z )	{ Vec3Set( &m_lookAt, x, y, z ); }

	void							ExtractClipPlanes();
	bool							SphereIn( const TVec3 *pos, const f32 rad ) const;
	bool							PointIn( const TVec3 *pos ) const;

	bool							BoxIn( const TVec3 *pos, const TVec3 *bounds );
	bool							BoxIn( const TVec3 *pos, const TVec3 *bounds, const TMatrix4 *orientMat );

	virtual void					Update( f32 dt, void *userData );
	virtual void					Render( CDriver *driver );
	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	TPlane						m_leftClipPlane;
	TPlane						m_rightClipPlane;
	TPlane						m_topClipPlane;
	TPlane						m_bottomClipPlane;
	TPlane						m_nearClipPlane;
	TPlane						m_farClipPlane;

	TMatrix4					m_perspectiveMatrix;
	TMatrix4					m_orthographicMatrix;

	TMatrix4					m_orientationMatrix;
	TMatrix4					m_viewMatrix;

	TVec3						m_position;
	TVec3						m_direction;
	TVec3						m_lookAt;

	f32								m_fieldOfView;
	f32								m_nearPlane;
	f32								m_farPlane;

	f32								m_speed;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim

#endif // __SIM_CAMERA_H
