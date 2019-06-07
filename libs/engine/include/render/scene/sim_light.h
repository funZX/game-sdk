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
	CLight();
	CLight( const std::string &name );
	virtual ~CLight();

	// ------------------------------------------------------------------//
	inline TVec4*						GetAmbient()		{ return &m_ambient; }
	inline void							SetAmbient( TVec4 *col ) { Vec4Copy( &m_ambient, col ); }

	inline TVec4*						GetDiffuse()		{ return &m_diffuse; }
	inline  void						SetDiffuse( TVec4 *col ) { Vec4Copy( &m_diffuse, col ); }

	inline TVec4*						GetSpecular()		{ return &m_specular; }
	inline void							SetSpecular( TVec4 *col ) { Vec4Copy( &m_specular, col ); }

	inline f32							GetIntensity() { return m_intensity; }
	inline void							SetIntensity( f32 intens ) { m_intensity = intens; }

	inline void							SetChannel( CDriver::LightChannel sel ) { m_channel = sel; };
	inline CDriver::LightChannel		GetChanel() { return m_channel; }

	virtual void						Update( f32 dt, void *userData );
	virtual void						Render( CDriver *driver );
	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	CDriver::LightChannel				m_channel;

	TVec4								m_ambient;
	TVec4								m_diffuse;
	TVec4								m_specular;

	f32									m_intensity;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim

#endif // __SIM_LIGHT_H
