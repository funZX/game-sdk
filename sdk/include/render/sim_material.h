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

#include <core/sim_core.h>
#include <core/sim_interfaces.h>

#include <render/sim_render.h>

namespace sim
{
namespace ren
{
// ----------------------------------------------------------------------//

class CTexture;
class CEffect;
class CDriver;

class CMaterial : public IUpdatable, public IRenderable, public IEngineItem
{
public:
    // ------------------------------------------------------------------//
    enum class TextureChannel : u32
    {
        Texture_0 = 0,
        Texture_1 = 1,
        Texture_2 = 2,
        Texture_3 = 3,
        Texture_4 = 4,
    };
    enum { k_Texture_Channels_Count = 5 };

	CMaterial();
	CMaterial( const std::string &name );
    CMaterial( const CMaterial& m );
	virtual ~CMaterial();

    inline CTexture*                GetTexture(u32 channel);
    inline void						SetTexture(CTexture* tex, u32 channel);

    inline Vec4				        GetAmbient();
    inline void						SetAmbient(Vec4 ambient);

    inline Vec4				        GetDiffuse();
    inline void						SetDiffuse(Vec4 diffuse);

    inline Vec4				        GetEmissive();
    inline void						SetEmissive(Vec4 emissive);

    inline Vec4				        GetSpecular();
    inline void						SetSpecular(Vec4 specular);

    inline Vec4				        GetReflective();
    inline void						SetReflective(Vec4 reflective);

    inline f32					    GetShininess();
    inline void						SetShininess(f32 shininess);

    inline f32					    GetRefraction();
    inline void						SetRefraction(f32 refraction);

    inline CEffect*                 GetEffect();
    inline void						SetEffect(CEffect* effect);

	virtual void					Update( f32 dt, void *userData );
	virtual void					Render( CDriver *driver );

    virtual bool					Load(io::CMemStream* ms);
    virtual bool					Save(io::CMemStream* ms);
	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	Vec4							m_ambient;
	Vec4							m_diffuse;
	Vec4							m_specular;
	Vec4							m_emissive;
	Vec4							m_reflective;

	f32							    m_shininess;
	f32							    m_refraction;

    CEffect*                        m_effect;
    CTexture*						m_textures[ k_Texture_Channels_Count ];
};

inline CTexture* CMaterial::GetTexture( u32 channel ) 
{
    return m_textures[channel]; 
}

inline void CMaterial::SetTexture(CTexture* tex, u32 channel)
{ 
    m_textures[channel] = tex;
}

inline Vec4 CMaterial::GetAmbient() 
{ 
    return m_ambient;
}

inline void CMaterial::SetAmbient(Vec4 ambient)
{
    m_ambient = ambient;
}

inline Vec4 CMaterial::GetDiffuse()
{ 
    return m_diffuse; 
}

inline void CMaterial::SetDiffuse(Vec4 diffuse) 
{ 
    m_diffuse = diffuse;
}

inline Vec4 CMaterial::GetEmissive()
{ 
    return m_emissive;
}

inline void CMaterial::SetEmissive(Vec4 emissive)
{
    m_emissive = emissive;
}

inline Vec4 CMaterial::GetSpecular() 
{ 
    return m_specular;
}

inline void CMaterial::SetSpecular(Vec4 specular) 
{ 
    m_specular = specular;
}

inline Vec4 CMaterial::GetReflective() 
{ 
    return m_reflective;
}

inline void CMaterial::SetReflective(Vec4 reflective)
{ 
    m_reflective = reflective; 
}

inline f32 CMaterial::GetShininess()
{
    return m_shininess;
}

inline void CMaterial::SetShininess(f32 shine)
{ 
    m_shininess = shine; 
}

inline f32 CMaterial::GetRefraction() 
{ 
    return m_refraction;
}

inline void CMaterial::SetRefraction(f32 refraction)
{
    m_refraction = refraction; 
}

inline CEffect* CMaterial::GetEffect()
{ 
    return m_effect;
}

inline void CMaterial::SetEffect(CEffect* effect)
{ 
    m_effect = effect;
}
// ----------------------------------------------------------------------//
}; // namespace ren
}; // namespace sim

#endif // __SIM_MATERIAL_H
