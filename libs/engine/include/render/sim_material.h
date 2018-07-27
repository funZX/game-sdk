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

#ifndef __SIM_MATERIAL_H
#define __SIM_MATERIAL_H

#include <core/sim_interfaces.h>
#include <math/sim_vec4.h>
#include <render/sim_render.h>
#include <render/sim_driver.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

class CTexture;
class CEffect;

class CMaterial : public IUpdatable, public IRenderable, public IEngineItem
{
public:
	CMaterial();
	CMaterial( const std::string &name );
	virtual ~CMaterial();

	inline CTexture*				GetTexture( u32 channel )	{ return m_textures[ channel ]; }
	inline void						SetTexture( CTexture *tex, u32 channel ) { m_textures[ channel ] = tex; }

	inline TVec4*					GetAmbient()				{ return &m_ambient; }
	inline void						SetAmbient( TVec4 *col )	{ Vec4Copy( &m_ambient, col ); }

	inline TVec4*					GetDiffuse()				{ return &m_diffuse; }
	inline void						SetDiffuse( TVec4 *col )	{ Vec4Copy( &m_diffuse, col ); }

	inline TVec4*					GetEmissive()				{ return &m_emissive; }
	inline void						SetEmissive( TVec4 *col )	{ Vec4Copy( &m_emissive, col ); }

	inline TVec4*					GetSpecular()				{ return &m_specular; }
	inline void						SetSpecular( TVec4 *col )	{ Vec4Copy( &m_specular, col ); }

	inline f32					    GetShininess()				{ return m_shininess; }
	inline void						SetShininess( f32 shine )	{ m_shininess = shine; }

	inline f32					    GetReflectivity()			{ return m_reflectivity; }
	inline void						SetReflectivity( f32 refl ) { m_reflectivity = refl; }

	inline CEffect*					GetEffect()					{ return m_effect; }
	inline void						SetEffect( CEffect* effect ){ m_effect = effect; }

	virtual void					Update( f32 dt, void *userData );
	virtual void					Render( CDriver *driver );
	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	TVec4							m_ambient;
	TVec4							m_diffuse;
	TVec4							m_specular;
	TVec4							m_emissive;

	f32							    m_reflectivity;
	f32							    m_shininess;

	CTexture*						m_textures[ CDriver::k_Select_Texture_Count ];
	CEffect*						m_effect;
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim

#endif // __SIM_MATERIAL_H
