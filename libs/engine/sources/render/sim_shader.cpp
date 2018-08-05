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
#include <render/sim_vertex_source.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CShader::TAttrib CShader::Attributes[CVertexSource::k_Vertex_Attributes_Count] =
{
	{ "a_PositionL",	-1,		CVertexSource::AttributeIndex::Position,	CVertexSource::AttributeFormat::Position,	CVertexSource::AttributeSize::Position,		CVertexSource::AttributeStride::Position,		CVertexSource::AttributeType::Position, },
	{ "a_TexCoord_0",	-1,		CVertexSource::AttributeIndex::TexCoord_0,	CVertexSource::AttributeFormat::TexCoord_0,	CVertexSource::AttributeSize::TexCoord_0,	CVertexSource::AttributeStride::TexCoord_0,		CVertexSource::AttributeType::TexCoord_0, },
	{ "a_TexCoord_1",	-1,		CVertexSource::AttributeIndex::TexCoord_1,	CVertexSource::AttributeFormat::TexCoord_1,	CVertexSource::AttributeSize::TexCoord_1,	CVertexSource::AttributeStride::TexCoord_1,		CVertexSource::AttributeType::TexCoord_1, },
	{ "a_TexCoord_2",	-1,		CVertexSource::AttributeIndex::TexCoord_2,	CVertexSource::AttributeFormat::TexCoord_2,	CVertexSource::AttributeSize::TexCoord_2,	CVertexSource::AttributeStride::TexCoord_2,		CVertexSource::AttributeType::TexCoord_2, },
	{ "a_TexCoord_3",	-1,		CVertexSource::AttributeIndex::TexCoord_3,	CVertexSource::AttributeFormat::TexCoord_3,	CVertexSource::AttributeSize::TexCoord_3,	CVertexSource::AttributeStride::TexCoord_3,		CVertexSource::AttributeType::TexCoord_3, },
	{ "a_Color",		-1,		CVertexSource::AttributeIndex::Color,		CVertexSource::AttributeFormat::Color,		CVertexSource::AttributeSize::Color,		CVertexSource::AttributeStride::Color,			CVertexSource::AttributeType::Color, },
	{ "a_NormalL",		-1,		CVertexSource::AttributeIndex::Normal,		CVertexSource::AttributeFormat::Normal,		CVertexSource::AttributeSize::Normal,		CVertexSource::AttributeStride::Normal,			CVertexSource::AttributeType::Normal, },
	{ "a_TangentL",		-1,		CVertexSource::AttributeIndex::Tangent,		CVertexSource::AttributeFormat::Tangent,	CVertexSource::AttributeSize::Tangent,		CVertexSource::AttributeStride::Tangent,		CVertexSource::AttributeType::Tangent, },
	{ "a_BinormalL",	-1,		CVertexSource::AttributeIndex::Binormal,	CVertexSource::AttributeFormat::Binormal,	CVertexSource::AttributeSize::Binormal,		CVertexSource::AttributeStride::Binormal,		CVertexSource::AttributeType::Binormal, },
	{ "a_Bone",			-1,		CVertexSource::AttributeIndex::Bone,		CVertexSource::AttributeFormat::Bone,		CVertexSource::AttributeSize::Bone,			CVertexSource::AttributeStride::Bone,			CVertexSource::AttributeType::Bone, },
	{ "a_Weight",		-1,		CVertexSource::AttributeIndex::Weight,		CVertexSource::AttributeFormat::Weight,		CVertexSource::AttributeSize::Weight,		CVertexSource::AttributeStride::Weight,			CVertexSource::AttributeType::Weight,		},
};

// ----------------------------------------------------------------------//

CShader::TUniform CShader::Uniforms[] =
{
	{ "u_Time",						-1,		CShader::UniformIndex::Time },
	{ "u_Color",					-1,		CShader::UniformIndex::Color },
	{ "u_Point_Size",				-1,		CShader::UniformIndex::Point_Size },

	{ "u_Matrix_World",				-1,		CShader::UniformIndex::Matrix_World },
	{ "u_Matrix_World_Inverse",		-1,		CShader::UniformIndex::Matrix_World_Inverse },
	{ "u_Matrix_World_InverseT",	-1,		CShader::UniformIndex::Matrix_World_InverseT },

	{ "u_Matrix_View",				-1,		CShader::UniformIndex::Matrix_View },
	{ "u_Matrix_View_Inverse",		-1,		CShader::UniformIndex::Matrix_View_Inverse },
	{ "u_Matrix_View_InverseT",		-1,		CShader::UniformIndex::Matrix_View_InverseT },

	{ "u_Matrix_Projection",		-1,		CShader::UniformIndex::Matrix_Projection },

	{ "u_Matrix_WorldView",			-1,		CShader::UniformIndex::Matrix_WorldView },
	{ "u_Matrix_ViewProjection",	-1,		CShader::UniformIndex::Matrix_ViewProjection },
	{ "u_Matrix_WorldViewProjection",-1,	CShader::UniformIndex::Matrix_WorldViewProjection },
	{ "u_Matrix_Normal",			-1,		CShader::UniformIndex::Matrix_Normal },
	{ "u_Matrix_Skeleton",			-1,		CShader::UniformIndex::Matrix_Bone },

	{ "u_Matrix_Tex_0",				-1,		CShader::UniformIndex::Matrix_Tex_0 },
	{ "u_Matrix_Tex_1",				-1,		CShader::UniformIndex::Matrix_Tex_1 },
	{ "u_Matrix_Tex_2",				-1,		CShader::UniformIndex::Matrix_Tex_2 },
	{ "u_Matrix_Tex_3",				-1,		CShader::UniformIndex::Matrix_Tex_3 },
	{ "u_Matrix_Tex_4",				-1,		CShader::UniformIndex::Matrix_Tex_4 },

	{ "u_Eye_Position",				-1,		CShader::UniformIndex::Eye_Position },
	{ "u_Eye_Direction",			-1,		CShader::UniformIndex::Eye_Direction },

	{ "u_Sampler_Tex_0",			-1,		CShader::UniformIndex::Sampler_Tex_0 },
	{ "u_Sampler_Tex_1",			-1,		CShader::UniformIndex::Sampler_Tex_1 },
	{ "u_Sampler_Tex_2",			-1,		CShader::UniformIndex::Sampler_Tex_2 },
	{ "u_Sampler_Tex_3",			-1,		CShader::UniformIndex::Sampler_Tex_3 },
	{ "u_Sampler_Tex_4",			-1,		CShader::UniformIndex::Sampler_Tex_4 },
	{ "u_Sampler_Cube",				-1,		CShader::UniformIndex::Sampler_Cube },

	{ "u_Fog_Color",				-1,		CShader::UniformIndex::Fog_Color },
	{ "u_Fog_Density",				-1,		CShader::UniformIndex::Fog_Density },
	{ "u_Fog_Select",				-1,		CShader::UniformIndex::Fog_Select },
	{ "u_Fog_Start",				-1,		CShader::UniformIndex::Fog_Start },
	{ "u_Fog_End",					-1,		CShader::UniformIndex::Fog_End },

	{ "u_Material_Ambient",			-1,		CShader::UniformIndex::Material_Ambient },
	{ "u_Material_Diffuse",			-1,		CShader::UniformIndex::Material_Diffuse },
	{ "u_Material_Specular",		-1,		CShader::UniformIndex::Material_Specular },
	{ "u_Material_Emissive",		-1,		CShader::UniformIndex::Material_Emissive },
	{ "u_Material_Shininess",		-1,		CShader::UniformIndex::Material_Shininess },
	{ "u_Material_Reflectivity",	-1,		CShader::UniformIndex::Material_Reflectivity },

	{ "u_Light_Enable_0",			-1,		CShader::UniformIndex::Light_Enable_0 },
	{ "u_Light_Position_0",			-1,		CShader::UniformIndex::Light_Position_0 },
	{ "u_Light_Direction_0",		-1,		CShader::UniformIndex::Light_Direction_0 },
	{ "u_Light_Ambient_0",			-1,		CShader::UniformIndex::Light_Ambient_0 },
	{ "u_Light_Diffuse_0",			-1,		CShader::UniformIndex::Light_Diffuse_0 },
	{ "u_Light_Specular_0",			-1,		CShader::UniformIndex::Light_Specular_0 },
	{ "u_Light_Intensity_0",		-1,		CShader::UniformIndex::Light_Intensity_0 },

	{ "u_Light_Enable_1",			-1,		CShader::UniformIndex::Light_Enable_1 },
	{ "u_Light_Position_1",			-1,		CShader::UniformIndex::Light_Position_1 },
	{ "u_Light_Direction_1",		-1,		CShader::UniformIndex::Light_Direction_1 },
	{ "u_Light_Ambient_1",			-1,		CShader::UniformIndex::Light_Ambient_1 },
	{ "u_Light_Diffuse_1",			-1,		CShader::UniformIndex::Light_Diffuse_1 },
	{ "u_Light_Specular_1",			-1,		CShader::UniformIndex::Light_Specular_1 },
	{ "u_Light_Intensity_1",		-1,		CShader::UniformIndex::Light_Intensity_1 },

	{ "u_Light_Enable_2",			-1,		CShader::UniformIndex::Light_Enable_2 },
	{ "u_Light_Position_2",			-1,		CShader::UniformIndex::Light_Position_2 },
	{ "u_Light_Direction_2",		-1,		CShader::UniformIndex::Light_Direction_2 },
	{ "u_Light_Ambient_2",			-1,		CShader::UniformIndex::Light_Ambient_2 },
	{ "u_Light_Diffuse_2",			-1,		CShader::UniformIndex::Light_Diffuse_2 },
	{ "u_Light_Specular_2",			-1,		CShader::UniformIndex::Light_Specular_2 },
	{ "u_Light_Intensity_2",		-1,		CShader::UniformIndex::Light_Intensity_2 },

	{ "u_Light_Enable_3",			-1,		CShader::UniformIndex::Light_Enable_3 },
	{ "u_Light_Position_3",			-1,		CShader::UniformIndex::Light_Position_3 },
	{ "u_Light_Direction_3",		-1,		CShader::UniformIndex::Light_Direction_3 },
	{ "u_Light_Ambient_3",			-1,		CShader::UniformIndex::Light_Ambient_3 },
	{ "u_Light_Diffuse_3",			-1,		CShader::UniformIndex::Light_Diffuse_3 },
	{ "u_Light_Specular_3",			-1,		CShader::UniformIndex::Light_Specular_3 },
	{ "u_Light_Intensity_3",		-1,		CShader::UniformIndex::Light_Intensity_3 },
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
	for (s32 k = 0; k < CVertexSource::k_Vertex_Attributes_Count; k++)
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
