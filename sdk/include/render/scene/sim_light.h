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

#include <core/sim_core.h>

#include <render/scene/sim_scene_node.h>
#include <render/sim_render.h>
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

    enum class Type : u32
    {
        Directional,
        Point,
        Spot
    };

    enum class Kind : u32
    {
        Linear,
        Constant,
        Quadratic
    };

	// ------------------------------------------------------------------//
    inline Vec4					        GetAmbient();
    inline void							SetAmbient(Vec4 ambient);

    inline Vec4					        GetDiffuse();
    inline  void						SetDiffuse(Vec4 diffuse);

    inline Vec4					        GetSpecular();
    inline void							SetSpecular(Vec4 specular);

    inline f32							GetAttenuation();
    inline void							SetAttenuation(f32 attenuation);

    inline f32							GetFallOffAngle();
    inline void							SetFallOffAngle(f32 fallOffAngle);

    inline f32							GetFallOffExponent();
    inline void							SetFallOffExponent(f32 fallOffExp);

    inline void							SetChannel(CDriver::LightChannel channel);
	inline CDriver::LightChannel		GetChanel();

	virtual void						Update( f32 dt, void *userData );
	virtual void						Render( CDriver *driver );

    inline Type                         GetType();
    inline void                         SetType(Type type);

    inline Kind                         GetKind();
    inline void                         SetKind(Kind kind);

    virtual bool	                    Load(io::CMemStream* ms);
    virtual bool	                    Save(io::CMemStream* ms);
	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	CDriver::LightChannel				m_channel;

	Vec4								m_ambient;
	Vec4								m_diffuse;
	Vec4								m_specular;

    Type                                m_type;
    Kind                                m_kind;
	f32									m_attenuation;
    f32									m_fallOffAngle;
    f32									m_fallOffExponent;
	// ------------------------------------------------------------------//
};


inline Vec4 CLight::GetAmbient() 
{
    return m_ambient; 
}

inline void CLight::SetAmbient(Vec4 ambient)
{
    m_ambient = ambient; 
}

inline Vec4 CLight::GetDiffuse()
{
    return m_diffuse;
}

inline void CLight::SetDiffuse(Vec4 diffuse) 
{ 
    m_diffuse = diffuse;
}

inline Vec4 CLight::GetSpecular() 
{
    return m_specular;
}

inline void CLight::SetSpecular(Vec4 specular)
{ 
    m_specular = specular;
}

inline f32 CLight::GetAttenuation()
{ 
    return m_attenuation; 
}

inline void CLight::SetAttenuation(f32 attenuation)
{ 
    m_attenuation = attenuation;
}

inline f32 CLight::GetFallOffAngle()
{
    return m_fallOffAngle;
}

inline void CLight::SetFallOffAngle(f32 fallOffAngle)
{
    m_fallOffAngle = fallOffAngle;
}

inline f32 CLight::GetFallOffExponent()
{
    return m_fallOffExponent;
}

inline void CLight::SetFallOffExponent(f32 fallOffExp)
{
    m_fallOffExponent = fallOffExp;
}

inline void CLight::SetChannel(CDriver::LightChannel channel)
{ 
    m_channel = channel;
}

inline CDriver::LightChannel CLight::GetChanel()
{ 
    return m_channel;
}

inline CLight::Type CLight::GetType()
{
    return m_type;
}

inline void CLight::SetType(CLight::Type type)
{
    m_type = type;
}


inline CLight::Kind CLight::GetKind()
{
    return m_kind;
}

inline void CLight::SetKind(CLight::Kind kind)
{
    m_kind = kind;
}
// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim

#endif // __SIM_LIGHT_H
