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

#ifndef __SIM_EFFECT_H
#define __SIM_EFFECT_H

#include <core/sim_interfaces.h>
#include <core/sim_list.h>
#include <core/sim_core.h>

#include <render/sim_render.h>
#include <render/sim_driver.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//
class CTexture;
class CShader;

class CEffect: public IRenderable, public IEngineItem
{
	friend class CDriver;

public:
	// ------------------------------------------------------------------//
	typedef struct
	{
		bool					depthtest;
		bool					depthmask;
		bool					cullface;
		bool					alphatest;

		bool					blending;
		CDriver::TBlendFunc		blendfunc;

		CDriver::TDepthFunc		depthfunc;
	} TTechnique;

	// ------------------------------------------------------------------//
	CEffect();
	CEffect( std::string name );
	virtual ~CEffect( );
	// ------------------------------------------------------------------//
	virtual void				Render(CDriver *driver);
	void						Bind(CDriver *driver, CVertexSource *vertexSource);

	void						Load( const s8* vsource, const s8* psource );

	// ------------------------------------------------------------------//
	inline CTexture*			GetTexture( u32 channel )	{ return m_textures[ channel ]; }
	inline void					SetTexture( CTexture *tex, u32 channel ) { m_textures[ channel ] = tex; }

	inline void					CopyTechnique( TTechnique* technique ) { SIM_MEMCPY(technique, &m_technique, sizeof(m_technique)); }
	inline TTechnique*			GetTechnique() { return &m_technique; }
	inline void					SetTechnique( const TTechnique* technique ) { SIM_MEMCPY( &m_technique, technique, sizeof(m_technique)); }

	void						InitAttributes( unsigned int numAttrib );
	void						InitUniforms( unsigned int numUniform );

	void						AddAttribute( const std::string& name, int index );
	void						AddUniform( const std::string& name, int index);

    virtual bool				Load(io::CMemStream* ms);
    virtual bool				Save(io::CMemStream* ms);
	// ------------------------------------------------------------------//
protected:
	// ------------------------------------------------------------------//
	void						ApplyTechnique( CDriver* driver );
	void						ApplyTextures( CDriver* driver );

	void						SetAttributes();
	void						SetUniforms();
	// ------------------------------------------------------------------//
protected:
    enum UniformMask{
        isUsingWorldInverseMatrix           = (1 << 0),
        isUsingWorldInverseTMatrix          = (1 << 1),
        isUsingViewInverseMatrix            = (1 << 2),
        isUsingViewInverseTMatrix           = (1 << 3),
        isUsingNormalMatrix                 = (1 << 4),
        isUsingWorldViewMatrix              = (1 << 5),
        isUsingViewProjectionMatrix         = (1 << 6),
        isUsingWorldViewProjectionMatrix    = (1 << 7),
    };

    enum AttributeMask {
        isUsingScreenPos                    = (1 <<  0), //CVertexSource::ScreenPos
        isUsingWorldPos                     = (1 <<  1), //CVertexSource::WorldPos
        isUsingTexCoord_0                   = (1 <<  2), //CVertexSource::TexCoord_0
        isUsingTexCoord_1                   = (1 <<  3), //CVertexSource::TexCoord_1
        isUsingTexCoord_2                   = (1 <<  4), //CVertexSource::TexCoord_2
        isUsingTexCoord_3                   = (1 <<  5), //CVertexSource::TexCoord_3
        isUsingColor                        = (1 <<  6), //CVertexSource::Color
        isUsingNormal                       = (1 <<  7), //CVertexSource::Normal
        isUsingTangent                      = (1 <<  8), //CVertexSource::Tangent
        isUsingBinormal                     = (1 <<  9), //CVertexSource::Binormal
        isUsingWeightArraySize              = (1 << 10), //CVertexSource::WeightArraySize
        isUsingWeightArray                  = (1 << 11), //CVertexSource::WeightArray
    };

	// ------------------------------------------------------------------//
	u32							m_iD;

	CTexture*					m_textures[ CDriver::k_Texture_Channels_Count ];

	s32							m_numUniforms;
	CShader::TUniform*			m_uniforms;
    u32                         m_uniformMask;

	s32							m_numAttrib;
	CShader::TAttrib*			m_attributes;
    u32                         m_attributeMask;

	CShader*					m_vshader;
	CShader*					m_pshader;

public:
	TTechnique					m_technique;
    // ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_EFFECT_H
