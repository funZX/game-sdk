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

#ifndef __SIM_EFFECT_H
#define __SIM_EFFECT_H

#include <core/sim_interfaces.h>
#include <core/sim_list.h>

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

	void						AddDefine( const std::string& define );
	// ------------------------------------------------------------------//
protected:
	// ------------------------------------------------------------------//
	void						ApplyTechnique( CDriver* driver );
	void						ApplyTextures( CDriver* driver );

	void						SetAttributes();
	void						SetUniforms();
	// ------------------------------------------------------------------//
protected:

	// ------------------------------------------------------------------//
	u32							m_iD;

	CTexture*					m_textures[ CDriver::k_Texture_Channels_Count ];

	bool						m_isUsingWorldInverseMatrix;
	bool						m_isUsingWorldInverseTMatrix;
	bool						m_isUsingViewInverseMatrix;
	bool						m_isUsingViewInverseTMatrix;
	bool						m_isUsingNormalMatrix;
	bool						m_isUsingWorldViewMatrix;
	bool						m_isUsingViewProjectionMatrix;
	bool						m_isUsingWorldViewProjectionMatrix;

	s32							m_numUniforms;
	CShader::TUniform*			m_uniforms;

	s32							m_numAttrib;
	CShader::TAttrib*			m_attributes;

	stl::CList<std::string>		m_defines;

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
