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

#ifndef __SIM_SHADER_H
#define __SIM_SHADER_H

#include <core/sim_interfaces.h>

#include <render/sim_render.h>
#include <render/sim_vertex_source.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

class CDriver;

class CShader : public IEngineItem
{
	friend class CEffect;

public:
	// ------------------------------------------------------------------//
	enum class UniformIndex
	{
		Time,
		Color,
		Point_Size,

		Matrix_World,
		Matrix_World_Inverse,
		Matrix_World_InverseT,

		Matrix_View,
		Matrix_View_Inverse,
		Matrix_View_InverseT,

		Matrix_Projection,

		Matrix_WorldView,
		Matrix_ViewProjection,
		Matrix_WorldViewProjection,
		Matrix_Normal,
		Matrix_BoneArray,

		Matrix_Tex_0,
		Matrix_Tex_1,
		Matrix_Tex_2,
		Matrix_Tex_3,
		Matrix_Tex_4,

		Eye_Position,
		Eye_Direction,

		Sampler_Tex_0,
		Sampler_Tex_1,
		Sampler_Tex_2,
		Sampler_Tex_3,
		Sampler_Tex_4,

		Sampler_Cube,

		Fog_Color,
		Fog_Density,
		Fog_Select,
		Fog_Start,
		Fog_End,

		Material_Ambient,
		Material_Diffuse,
		Material_Specular,
		Material_Emissive,
		Material_Reflective,
		Material_Shininess,
		Material_Refraction,

		Light_Enable_0,
		Light_Position_0,
		Light_Direction_0,
		Light_Ambient_0,
		Light_Diffuse_0,
		Light_Specular_0,
		Light_Intensity_0,

		Light_Enable_1,
		Light_Position_1,
		Light_Direction_1,
		Light_Ambient_1,
		Light_Diffuse_1,
		Light_Specular_1,
		Light_Intensity_1,

		Light_Enable_2,
		Light_Position_2,
		Light_Direction_2,
		Light_Ambient_2,
		Light_Diffuse_2,
		Light_Specular_2,
		Light_Intensity_2,

		Light_Enable_3,
		Light_Position_3,
		Light_Direction_3,
		Light_Ambient_3,
		Light_Diffuse_3,
		Light_Specular_3,
		Light_Intensity_3,
	};

	enum { k_Uniform_Count = 69  }; //yeah... sixtynine
	// ------------------------------------------------------------------//
	struct TUniform
	{
		const char*									m_name;
		s32											m_location;
		UniformIndex								m_index;
	};
	// ------------------------------------------------------------------//
	struct TAttrib
	{
		const char*									m_name;
		s32											m_location;

		CVertexSource::AttributeIndex				m_compIndex;
		CVertexSource::AttributeFormat				m_compFormat;
		CVertexSource::AttributeSize				m_compSize;
		CVertexSource::AttributeStride				m_compStride;
		CVertexSource::AttributeType				m_compType;
	};
	// ------------------------------------------------------------------//
	enum class Type : u32
	{
		Vertex		= GL_VERTEX_SHADER,
		Pixel		= GL_FRAGMENT_SHADER,
	};
	// ------------------------------------------------------------------//
	CShader( Type type );
	CShader( std::string name, Type type );
	virtual ~CShader( );

	// ------------------------------------------------------------------//
	// ------------------------------------------------------------------//
	void						Load( const s8* source );
	inline u32 		            GetID() { return m_iD; }

	static const TAttrib*		FindAttrib( const std::string& name );
	static const TUniform*		FindUniform( const std::string& name );

    virtual bool	            Load(io::CMemStream* ms);
    virtual bool	            Save(io::CMemStream* ms);
protected:

	// ------------------------------------------------------------------//
	u32							m_iD;

	static TAttrib				Attributes[];
	static TUniform				Uniforms[];
	// ----------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_SHADER_H
