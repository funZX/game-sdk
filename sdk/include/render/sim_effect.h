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
#include <render/sim_material.h>
#include <render/sim_driver.h>

namespace sim
{
namespace ren
{
// ----------------------------------------------------------------------//
class CTexture;
class CShader;

class CEffect: public IRenderable, public IEngineItem
{
	friend class CDriver;

public:
	// ------------------------------------------------------------------//
	typedef struct _TTechnique
	{
		bool					depthtest;
		bool					depthmask;
		bool					cullface;
		bool					alphatest;

		bool					blending;
		CDriver::TBlendFunc		blendfunc;
		Vec4					blendColor;

		CDriver::TDepthFunc		depthfunc;

		_TTechnique()
		{
            depthtest = true;
            depthmask = true;
            cullface = true;
            alphatest = false;

            blending = false;
            blendfunc.equation = GL_FUNC_ADD;
            blendfunc.src = GL_SRC_ALPHA;
            blendfunc.dst = GL_ONE_MINUS_SRC_ALPHA;
            blendColor = col::White;

            depthfunc.equation = GL_LESS;
		}

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

	void						InitUniforms( u32 numUniform );
	void						AddUniform( const std::string& name, int index);

    void						AddAttribute( const std::string& name );

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
        isUsingProjectionMatrix				= ( 0),
        isUsingViewMatrix					= ( 1),
		isUsingWorldMatrix					= ( 2),
        isUsingNormalMatrix					= ( 3),
        isUsingWorldViewMatrix				= ( 4),
        isUsingViewProjectionMatrix			= ( 5),
        isUsingWorldViewProjectionMatrix	= ( 6),
		isUsingWorldInverseMatrix			= ( 7),
        isUsingWorldInverseTMatrix          = ( 8),
        isUsingViewInverseMatrix            = ( 9),
        isUsingViewInverseTMatrix           = (10),
    };

	// ------------------------------------------------------------------//
	u32							m_iD;

	CTexture*					m_textures[ CMaterial::k_Texture_Channels_Count ];

	s32							m_numUniforms;
	CShader::TUniform*			m_uniforms;
    u32                         m_uniformMatrixMask;

    u32                         m_attributeMask;

	CShader*					m_vshader;
	CShader*					m_pshader;

public:
	TTechnique					m_technique;
    // ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace ren
}; // namespace sim
#endif // __SIM_EFFECT_H
