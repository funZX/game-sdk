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

#include <render/sim_render.h>
#include <render/sim_vertex_source.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

class CDriver;

class CShader
{
	friend class CEffect;

public:
	// ------------------------------------------------------------------//
	typedef enum
	{
		k_Attribute_Position,
		k_Attribute_TexCoord_0,
		k_Attribute_TexCoord_1,
		k_Attribute_TexCoord_2,
		k_Attribute_TexCoord_3,
		k_Attribute_Color,
		k_Attribute_Normal,
		k_Attribute_Tangent,
		k_Attribute_Binormal,
		k_Attribute_Bone,
		k_Attribute_Weight,

		k_Attribute_Count,

	} K_ATTRIBUTE;

	// ------------------------------------------------------------------//
	typedef enum
	{
		k_Uniform_Time,
		k_Uniform_Color,
		k_Uniform_Point_Size,

		k_Uniform_Matrix_World,
		k_Uniform_Matrix_World_Inverse,
		k_Uniform_Matrix_World_InverseT,

		k_Uniform_Matrix_View,
		k_Uniform_Matrix_View_Inverse,
		k_Uniform_Matrix_View_InverseT,

		k_Uniform_Matrix_Projection,

		k_Uniform_Matrix_WorldView,
		k_Uniform_Matrix_ViewProjection,
		k_Uniform_Matrix_WorldViewProjection,
		k_Uniform_Matrix_Normal,
		k_Uniform_Matrix_Skeleton,

		k_Uniform_Matrix_Tex_0,
		k_Uniform_Matrix_Tex_1,
		k_Uniform_Matrix_Tex_2,
		k_Uniform_Matrix_Tex_3,
		k_Uniform_Matrix_Tex_4,

		k_Uniform_Eye_Position,
		k_Uniform_Eye_Direction,

		k_Uniform_Sampler_Tex_0,
		k_Uniform_Sampler_Tex_1,
		k_Uniform_Sampler_Tex_2,
		k_Uniform_Sampler_Tex_3,
		k_Uniform_Sampler_Tex_4,

		k_Uniform_Sampler_Cube,

		k_Uniform_Fog_Color,
		k_Uniform_Fog_Density,
		k_Uniform_Fog_Select,
		k_Uniform_Fog_Start,
		k_Uniform_Fog_End,

		k_Uniform_Material_Ambient,
		k_Uniform_Material_Diffuse,
		k_Uniform_Material_Specular,
		k_Uniform_Material_Emissive,
		k_Uniform_Material_Shininess,
		k_Uniform_Material_Reflectivity,

		k_Uniform_Light_Enable_0,
		k_Uniform_Light_Position_0,
		k_Uniform_Light_Direction_0,
		k_Uniform_Light_Ambient_0,
		k_Uniform_Light_Diffuse_0,
		k_Uniform_Light_Specular_0,
		k_Uniform_Light_Intensity_0,

		k_Uniform_Light_Enable_1,
		k_Uniform_Light_Position_1,
		k_Uniform_Light_Direction_1,
		k_Uniform_Light_Ambient_1,
		k_Uniform_Light_Diffuse_1,
		k_Uniform_Light_Specular_1,
		k_Uniform_Light_Intensity_1,

		k_Uniform_Light_Enable_2,
		k_Uniform_Light_Position_2,
		k_Uniform_Light_Direction_2,
		k_Uniform_Light_Ambient_2,
		k_Uniform_Light_Diffuse_2,
		k_Uniform_Light_Specular_2,
		k_Uniform_Light_Intensity_2,

		k_Uniform_Light_Enable_3,
		k_Uniform_Light_Position_3,
		k_Uniform_Light_Direction_3,
		k_Uniform_Light_Ambient_3,
		k_Uniform_Light_Diffuse_3,
		k_Uniform_Light_Specular_3,
		k_Uniform_Light_Intensity_3,

		k_Uniform_Count

	} K_UNIFORM;

	// ------------------------------------------------------------------//
	struct TUniform
	{
		K_UNIFORM									m_ID;
		const char*									m_name;
		s32											m_location;
	};
	// ------------------------------------------------------------------//
	struct TAttrib
	{
		K_ATTRIBUTE									m_ID;
		const char*									m_name;
		s32											m_location;

		CVertexSource::K_VERTEX_ATTRIBUTE_FORMAT	m_compFormat;
		CVertexSource::K_VERTEX_ATTRIBUTE_SIZE		m_compSize;
		CVertexSource::K_VERTEX_ATTRIBUTE_OFFSET	m_compOff;
		CVertexSource::K_VERTEX_ATTRIBUTE_TYPE		m_compType;
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
	std::string					m_name;
	u32							m_iD;

	static TAttrib				Attributes[];
	static TUniform				Uniforms[];
	// ----------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_SHADER_H
