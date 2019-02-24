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

	inline TVec4*				    GetReflective()				{ return &m_reflective; }
	inline void						SetReflective(TVec4 *col)	{ Vec4Copy(&m_reflective, col); }

	inline f32					    GetShininess()				{ return m_shininess; }
	inline void						SetShininess( f32 shine )	{ m_shininess = shine; }

	inline f32					    GetRefraction() { return m_refraction; }
	inline void						SetRefraction(f32 refraction) { m_refraction = refraction; }

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
	TVec4							m_reflective;

	f32							    m_shininess;
	f32							    m_refraction;

	CTexture*						m_textures[CDriver::k_Texture_Channels_Count];
	CEffect*						m_effect;
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim

#endif // __SIM_MATERIAL_H
