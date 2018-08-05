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
		Matrix_Bone,

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
		Material_Shininess,
		Material_Reflectivity,

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

	enum { k_Uniform_Count = 67  };
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
	typedef enum
	{
		k_Type_None,

		k_Type_Vertex		= GL_VERTEX_SHADER,
		k_Type_Fragment		= GL_FRAGMENT_SHADER,

		k_Type_Count

	} K_TYPE;
	// ------------------------------------------------------------------//
	CShader( K_TYPE type );
	CShader( std::string name, K_TYPE type );
	virtual ~CShader( );

	// ------------------------------------------------------------------//
	// ------------------------------------------------------------------//
	void						Load( const s8 *source );
	inline u32 		            GetID() { return m_iD; }

	static const TAttrib*		FindAttrib( const std::string& name );
	static const TUniform*		FindUniform( const std::string& name );

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
