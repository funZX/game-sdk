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

#ifndef __SIM_LIGHT_H
#define __SIM_LIGHT_H

#include <render/scene/sim_scene_node.h>
#include <render/sim_render.h>

#include <math/sim_vec4.h>
#include <math/sim_vec3.h>

#include <render/sim_driver.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

class CDriver;

class CLight : public CSceneNode
{
public:
	CLight( const std::string &name );
	virtual ~CLight();

	// ------------------------------------------------------------------//
	inline bool							IsVisible()			{ return m_isVisible; }

	inline TVec4*						GetAmbient()		{ return &m_ambient; }
	inline void							SetAmbient( TVec4 *col ) { Vec4Copy( &m_ambient, col ); }

	inline TVec4*						GetDiffuse()		{ return &m_diffuse; }
	inline  void						SetDiffuse( TVec4 *col ) { Vec4Copy( &m_diffuse, col ); }

	inline TVec4*						GetSpecular()		{ return &m_specular; }
	inline void							SetSpecular( TVec4 *col ) { Vec4Copy( &m_specular, col ); }

	inline f32							GetIntensity() { return m_intensity; }
	inline void							SetIntensity( f32 intens ) { m_intensity = intens; }

	inline void							SetChannel( CDriver::K_SELECT_LIGHT sel ) { m_channel = sel; };
	inline CDriver::K_SELECT_LIGHT		GetChanel() { return m_channel; }

	inline void							SetDirection( TVec3 *dir ) { Vec3Copy( &m_direction, dir ); }
	inline TVec3*						GetDirection() { return &m_direction; };

	inline void							SetPosition( TVec3 *pos ) { Vec3Copy( &m_position, pos ); }

	inline TVec3*						GetPosition() { return &m_position; };

	virtual void						Update( f32 dt, void *userData );
	virtual void						Render( CDriver *driver );
	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	CDriver::K_SELECT_LIGHT		m_channel;

	TVec4						m_ambient;
	TVec4						m_diffuse;
	TVec4						m_specular;

	f32							    m_intensity;

	TVec3						m_position;
	TVec3						m_direction;

	bool							m_isVisible;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim

#endif // __SIM_LIGHT_H
