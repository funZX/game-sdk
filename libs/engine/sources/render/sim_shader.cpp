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

#include <render/sim_driver.h>
#include <render/sim_shader.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CShader::TAttrib CShader::Attributes[] =
{
	{ CShader::k_Attribute_Position,				"a_PositionL",	-1,		CVertexSource::k_Vertex_Attribute_Format_Position,		CVertexSource::k_Vertex_Attribute_Size_Position,	CVertexSource::k_Vertex_Attribute_Offset_Position,		CVertexSource::k_Vertex_Attribute_Type_Position,	 },
	{ CShader::k_Attribute_TexCoord_0,				"a_TexCoord_0",	-1,		CVertexSource::k_Vertex_Attribute_Format_TexCoord_0,	CVertexSource::k_Vertex_Attribute_Size_TexCoord_0,	CVertexSource::k_Vertex_Attribute_Offset_TexCoord_0,	CVertexSource::k_Vertex_Attribute_Type_TexCoord_0,	 },
	{ CShader::k_Attribute_TexCoord_1,				"a_TexCoord_1",	-1,		CVertexSource::k_Vertex_Attribute_Format_TexCoord_1,	CVertexSource::k_Vertex_Attribute_Size_TexCoord_1,	CVertexSource::k_Vertex_Attribute_Offset_TexCoord_1,	CVertexSource::k_Vertex_Attribute_Type_TexCoord_1,	 },
	{ CShader::k_Attribute_TexCoord_2,				"a_TexCoord_2",	-1,		CVertexSource::k_Vertex_Attribute_Format_TexCoord_2,	CVertexSource::k_Vertex_Attribute_Size_TexCoord_2,	CVertexSource::k_Vertex_Attribute_Offset_TexCoord_2,	CVertexSource::k_Vertex_Attribute_Type_TexCoord_2,	 },
	{ CShader::k_Attribute_TexCoord_3,				"a_TexCoord_3",	-1,		CVertexSource::k_Vertex_Attribute_Format_TexCoord_3,	CVertexSource::k_Vertex_Attribute_Size_TexCoord_3,	CVertexSource::k_Vertex_Attribute_Offset_TexCoord_3,	CVertexSource::k_Vertex_Attribute_Type_TexCoord_3,	 },
	{ CShader::k_Attribute_Color,					"a_Color",		-1,		CVertexSource::k_Vertex_Attribute_Format_Color,			CVertexSource::k_Vertex_Attribute_Size_Color,		CVertexSource::k_Vertex_Attribute_Offset_Color,			CVertexSource::k_Vertex_Attribute_Type_Color,		 },
	{ CShader::k_Attribute_Normal,					"a_NormalL",	-1,		CVertexSource::k_Vertex_Attribute_Format_Normal,		CVertexSource::k_Vertex_Attribute_Size_Normal,		CVertexSource::k_Vertex_Attribute_Offset_Normal,		CVertexSource::k_Vertex_Attribute_Type_Normal,		 },
	{ CShader::k_Attribute_Tangent,					"a_TangentL",	-1,		CVertexSource::k_Vertex_Attribute_Format_Tangent,		CVertexSource::k_Vertex_Attribute_Size_Tangent,		CVertexSource::k_Vertex_Attribute_Offset_Tangent,		CVertexSource::k_Vertex_Attribute_Type_Tangent,		 },
	{ CShader::k_Attribute_Binormal,				"a_BinormalL",	-1,		CVertexSource::k_Vertex_Attribute_Format_Binormal,		CVertexSource::k_Vertex_Attribute_Size_Binormal,	CVertexSource::k_Vertex_Attribute_Offset_Binormal,		CVertexSource::k_Vertex_Attribute_Type_Binormal,	 },
	{ CShader::k_Attribute_Bone,					"a_Bone",		-1,		CVertexSource::k_Vertex_Attribute_Format_Bone,			CVertexSource::k_Vertex_Attribute_Size_Bone,		CVertexSource::k_Vertex_Attribute_Offset_Bone,			CVertexSource::k_Vertex_Attribute_Type_Bone,		 },
	{ CShader::k_Attribute_Weight,					"a_Weight",		-1,		CVertexSource::k_Vertex_Attribute_Format_Weight,		CVertexSource::k_Vertex_Attribute_Size_Weight,		CVertexSource::k_Vertex_Attribute_Offset_Weight,		CVertexSource::k_Vertex_Attribute_Type_Weight,		 },
};

// ----------------------------------------------------------------------//

CShader::TUniform CShader::Uniforms[] =
{
	{ CShader::k_Uniform_Time,						"u_Time",						-1 },
	{ CShader::k_Uniform_Color,						"u_Color",						-1 },
	{ CShader::k_Uniform_Point_Size,				"u_Point_Size",					-1 },

	{ CShader::k_Uniform_Matrix_World,				"u_Matrix_World",				-1 },
	{ CShader::k_Uniform_Matrix_World_Inverse,		"u_Matrix_World_Inverse",		-1 },
	{ CShader::k_Uniform_Matrix_World_InverseT,		"u_Matrix_World_InverseT",		-1 },

	{ CShader::k_Uniform_Matrix_View,				"u_Matrix_View",				-1 },
	{ CShader::k_Uniform_Matrix_View_Inverse,		"u_Matrix_View_Inverse",		-1 },
	{ CShader::k_Uniform_Matrix_View_InverseT,		"u_Matrix_View_InverseT",		-1 },
	
	{ CShader::k_Uniform_Matrix_Projection,			"u_Matrix_Projection",			-1 },

	{ CShader::k_Uniform_Matrix_WorldView,			"u_Matrix_WorldView",			-1 },
	{ CShader::k_Uniform_Matrix_ViewProjection,		"u_Matrix_ViewProjection",		-1 },
	{ CShader::k_Uniform_Matrix_WorldViewProjection,"u_Matrix_WorldViewProjection",	-1 },
	{ CShader::k_Uniform_Matrix_Normal,				"u_Matrix_Normal",				-1 },
	{ CShader::k_Uniform_Matrix_Skeleton,			"u_Matrix_Skeleton",			-1 },

	{ CShader::k_Uniform_Matrix_Tex_0,				"u_Matrix_Tex_0",				-1 },
	{ CShader::k_Uniform_Matrix_Tex_1,				"u_Matrix_Tex_1",				-1 },
	{ CShader::k_Uniform_Matrix_Tex_2,				"u_Matrix_Tex_2",				-1 },
	{ CShader::k_Uniform_Matrix_Tex_3,				"u_Matrix_Tex_3",				-1 },
	{ CShader::k_Uniform_Matrix_Tex_4,				"u_Matrix_Tex_4",				-1 },

	{ CShader::k_Uniform_Eye_Position,				"u_Eye_Position",				-1 },
	{ CShader::k_Uniform_Eye_Direction,				"u_Eye_Direction",				-1 },

	{ CShader::k_Uniform_Sampler_Tex_0,				"u_Sampler_Tex_0",				-1 },
	{ CShader::k_Uniform_Sampler_Tex_1,				"u_Sampler_Tex_1",				-1 },
	{ CShader::k_Uniform_Sampler_Tex_2,				"u_Sampler_Tex_2",				-1 },
	{ CShader::k_Uniform_Sampler_Tex_3,				"u_Sampler_Tex_3",				-1 },
	{ CShader::k_Uniform_Sampler_Tex_4,				"u_Sampler_Tex_4",				-1 },
	{ CShader::k_Uniform_Sampler_Cube,				"u_Sampler_Cube",				-1 },

	{ CShader::k_Uniform_Fog_Color,					"u_Fog_Color",					-1 },
	{ CShader::k_Uniform_Fog_Density,				"u_Fog_Density",				-1 },
	{ CShader::k_Uniform_Fog_Select,				"u_Fog_Select",					-1 },
	{ CShader::k_Uniform_Fog_Start,					"u_Fog_Start",					-1 },
	{ CShader::k_Uniform_Fog_End,					"u_Fog_End",					-1 },

	{ CShader::k_Uniform_Material_Ambient,			"u_Material_Ambient",			-1 },
	{ CShader::k_Uniform_Material_Diffuse,			"u_Material_Diffuse",			-1 },
	{ CShader::k_Uniform_Material_Specular,			"u_Material_Specular",			-1 },
	{ CShader::k_Uniform_Material_Emissive,			"u_Material_Emissive",			-1 },
	{ CShader::k_Uniform_Material_Shininess,		"u_Material_Shininess",			-1 },
	{ CShader::k_Uniform_Material_Reflectivity,		"u_Material_Reflectivity",		-1 },

	{ CShader::k_Uniform_Light_Enable_0,			"u_Light_Enable_0",				-1 },
	{ CShader::k_Uniform_Light_Position_0,			"u_Light_Position_0",			-1 },
	{ CShader::k_Uniform_Light_Direction_0,			"u_Light_Direction_0",			-1 },
	{ CShader::k_Uniform_Light_Ambient_0,			"u_Light_Ambient_0",			-1 },
	{ CShader::k_Uniform_Light_Diffuse_0,			"u_Light_Diffuse_0",			-1 },
	{ CShader::k_Uniform_Light_Specular_0,			"u_Light_Specular_0",			-1 },
	{ CShader::k_Uniform_Light_Intensity_0,			"u_Light_Intensity_0",			-1 },

	{ CShader::k_Uniform_Light_Enable_1,			"u_Light_Enable_1",				-1 },
	{ CShader::k_Uniform_Light_Position_1,			"u_Light_Position_1",			-1 },
	{ CShader::k_Uniform_Light_Direction_1,			"u_Light_Direction_1",			-1 },
	{ CShader::k_Uniform_Light_Ambient_1,			"u_Light_Ambient_1",			-1 },
	{ CShader::k_Uniform_Light_Diffuse_1,			"u_Light_Diffuse_1",			-1 },
	{ CShader::k_Uniform_Light_Specular_1,			"u_Light_Specular_1",			-1 },
	{ CShader::k_Uniform_Light_Intensity_1,			"u_Light_Intensity_1",			-1 },

	{ CShader::k_Uniform_Light_Enable_2,			"u_Light_Enable_2",				-1 },
	{ CShader::k_Uniform_Light_Position_2,			"u_Light_Position_2",			-1 },
	{ CShader::k_Uniform_Light_Direction_2,			"u_Light_Direction_2",			-1 },
	{ CShader::k_Uniform_Light_Ambient_2,			"u_Light_Ambient_2",			-1 },
	{ CShader::k_Uniform_Light_Diffuse_2,			"u_Light_Diffuse_2",			-1 },
	{ CShader::k_Uniform_Light_Specular_2,			"u_Light_Specular_2",			-1 },
	{ CShader::k_Uniform_Light_Intensity_2,			"u_Light_Intensity_2",			-1 },

	{ CShader::k_Uniform_Light_Enable_3,			"u_Light_Enable_3",				-1 },
	{ CShader::k_Uniform_Light_Position_3,			"u_Light_Position_3",			-1 },
	{ CShader::k_Uniform_Light_Direction_3,			"u_Light_Direction_3",			-1 },
	{ CShader::k_Uniform_Light_Ambient_3,			"u_Light_Ambient_3",			-1 },
	{ CShader::k_Uniform_Light_Diffuse_3,			"u_Light_Diffuse_3",			-1 },
	{ CShader::k_Uniform_Light_Specular_3,			"u_Light_Specular_3",			-1 },
	{ CShader::k_Uniform_Light_Intensity_3,			"u_Light_Intensity_3",			-1 },
};

// ----------------------------------------------------------------------//

CShader::CShader( K_TYPE type )
{
	m_iD = glCreateShader( type );
}

// ----------------------------------------------------------------------//

CShader::CShader( std::string name, K_TYPE type )
	: CShader( type )
{
	m_name = name;
}

// ----------------------------------------------------------------------//

CShader::~CShader()
{
	glDeleteShader( m_iD );
}

// ----------------------------------------------------------------------//

void CShader::Load( const s8 *source )
{
	glShaderSource( m_iD, 1, &source, nullptr);
	glCompileShader( m_iD );

#if SIM_DEBUG
	s32 status = GL_TRUE, infologLength = 0, charsWritten  = 0;
	char *infoLog = nullptr;

	glGetShaderiv( m_iD, GL_COMPILE_STATUS, &status );
	glGetShaderiv( m_iD, GL_INFO_LOG_LENGTH, &infologLength );

	if(!status && infologLength > 0 )
	{
		infoLog = SIM_NEW char[ infologLength ];

		glGetShaderInfoLog( m_iD, infologLength, &charsWritten, infoLog );
		SIM_PRINT("\n%s\n", infoLog);

		SIM_SAFE_DELETE_ARRAY( infoLog );
	}

	SIM_ASSERT( status == GL_TRUE );
#endif // SIM_DEBUG
}

// ----------------------------------------------------------------------//

const CShader::TAttrib* CShader::FindAttrib(const std::string& name)
{
	for (s32 k = 0; k < k_Attribute_Count; k++)
	{
		if ( !strcmp( name.c_str(), Attributes[k].m_name ) )
			return &Attributes[ k ];
	}

	return nullptr;
}

// ----------------------------------------------------------------------//

const CShader::TUniform* CShader::FindUniform(const std::string& name)
{
	for (s32 k = 0; k < k_Uniform_Count; k++)
	{
		if ( !strcmp( name.c_str(), Uniforms[k].m_name ) )
			return &Uniforms[k];
	}

	return nullptr;
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
