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
    { "a_PositionL",	-1,		CVertexSource::AttributeIndex::ScreenPos,	CVertexSource::AttributeFormat::ScreenPos,	CVertexSource::AttributeSize::ScreenPos,		CVertexSource::AttributeStride::ScreenPos,	CVertexSource::AttributeType::ScreenPos, },
	{ "a_PositionL",	-1,		CVertexSource::AttributeIndex::WorldPos,	CVertexSource::AttributeFormat::WorldPos,	CVertexSource::AttributeSize::WorldPos,		CVertexSource::AttributeStride::WorldPos,		CVertexSource::AttributeType::WorldPos, },
	{ "a_TexCoord_0",	-1,		CVertexSource::AttributeIndex::TexCoord_0,	CVertexSource::AttributeFormat::TexCoord_0,	CVertexSource::AttributeSize::TexCoord_0,	CVertexSource::AttributeStride::TexCoord_0,		CVertexSource::AttributeType::TexCoord_0, },
	{ "a_TexCoord_1",	-1,		CVertexSource::AttributeIndex::TexCoord_1,	CVertexSource::AttributeFormat::TexCoord_1,	CVertexSource::AttributeSize::TexCoord_1,	CVertexSource::AttributeStride::TexCoord_1,		CVertexSource::AttributeType::TexCoord_1, },
	{ "a_TexCoord_2",	-1,		CVertexSource::AttributeIndex::TexCoord_2,	CVertexSource::AttributeFormat::TexCoord_2,	CVertexSource::AttributeSize::TexCoord_2,	CVertexSource::AttributeStride::TexCoord_2,		CVertexSource::AttributeType::TexCoord_2, },
	{ "a_TexCoord_3",	-1,		CVertexSource::AttributeIndex::TexCoord_3,	CVertexSource::AttributeFormat::TexCoord_3,	CVertexSource::AttributeSize::TexCoord_3,	CVertexSource::AttributeStride::TexCoord_3,		CVertexSource::AttributeType::TexCoord_3, },
	{ "a_Color",		-1,		CVertexSource::AttributeIndex::Color,		CVertexSource::AttributeFormat::Color,		CVertexSource::AttributeSize::Color,		CVertexSource::AttributeStride::Color,			CVertexSource::AttributeType::Color, },
	{ "a_NormalL",		-1,		CVertexSource::AttributeIndex::Normal,		CVertexSource::AttributeFormat::Normal,		CVertexSource::AttributeSize::Normal,		CVertexSource::AttributeStride::Normal,			CVertexSource::AttributeType::Normal, },
	{ "a_TangentL",		-1,		CVertexSource::AttributeIndex::Tangent,		CVertexSource::AttributeFormat::Tangent,	CVertexSource::AttributeSize::Tangent,		CVertexSource::AttributeStride::Tangent,		CVertexSource::AttributeType::Tangent, },
	{ "a_BinormalL",	-1,		CVertexSource::AttributeIndex::Binormal,	CVertexSource::AttributeFormat::Binormal,	CVertexSource::AttributeSize::Binormal,		CVertexSource::AttributeStride::Binormal,		CVertexSource::AttributeType::Binormal, },
	{ "a_WeightArraySize",-1,	CVertexSource::AttributeIndex::WeightArraySize,	CVertexSource::AttributeFormat::WeightArraySize, CVertexSource::AttributeSize::WeightArraySize, CVertexSource::AttributeStride::WeightArraySize, CVertexSource::AttributeType::WeightArraySize, },
	{ "a_WeightArray",	-1,		CVertexSource::AttributeIndex::WeightArray,	CVertexSource::AttributeFormat::WeightArray,CVertexSource::AttributeSize::WeightArray,		CVertexSource::AttributeStride::WeightArray,CVertexSource::AttributeType::WeightArray, },
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
	{ "u_Matrix_BoneArray",			-1,		CShader::UniformIndex::Matrix_BoneArray },

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
	{ "u_Material_Reflective",		-1,		CShader::UniformIndex::Material_Reflective },
	{ "u_Material_Shininess",		-1,		CShader::UniformIndex::Material_Shininess },
	{ "u_Material_Refraction",		-1,		CShader::UniformIndex::Material_Refraction },

	{ "u_Light_Enable_0",			-1,		CShader::UniformIndex::Light_Enable_0 },
	{ "u_Light_Position_0",			-1,		CShader::UniformIndex::Light_Position_0 },
	{ "u_Light_Direction_0",		-1,		CShader::UniformIndex::Light_Direction_0 },
	{ "u_Light_Ambient_0",			-1,		CShader::UniformIndex::Light_Ambient_0 },
	{ "u_Light_Diffuse_0",			-1,		CShader::UniformIndex::Light_Diffuse_0 },
	{ "u_Light_Specular_0",			-1,		CShader::UniformIndex::Light_Specular_0 },
	{ "u_Light_Attenuation_0",		-1,		CShader::UniformIndex::Light_Attenuation_0 },
    { "u_Light_FallOffAngle_0",		-1,		CShader::UniformIndex::Light_FallOffAngle_0 },
    { "u_Light_FallOffExp_0",		-1,		CShader::UniformIndex::Light_FallOffExp_0 },

	{ "u_Light_Enable_1",			-1,		CShader::UniformIndex::Light_Enable_1 },
	{ "u_Light_Position_1",			-1,		CShader::UniformIndex::Light_Position_1 },
	{ "u_Light_Direction_1",		-1,		CShader::UniformIndex::Light_Direction_1 },
	{ "u_Light_Ambient_1",			-1,		CShader::UniformIndex::Light_Ambient_1 },
	{ "u_Light_Diffuse_1",			-1,		CShader::UniformIndex::Light_Diffuse_1 },
	{ "u_Light_Specular_1",			-1,		CShader::UniformIndex::Light_Specular_1 },
	{ "u_Light_Attenuation_1",		-1,		CShader::UniformIndex::Light_Attenuation_1 },
    { "u_Light_FallOffAngle_1",		-1,		CShader::UniformIndex::Light_FallOffAngle_1 },
    { "u_Light_FallOffExp_1",		-1,		CShader::UniformIndex::Light_FallOffExp_1 },

	{ "u_Light_Enable_2",			-1,		CShader::UniformIndex::Light_Enable_2 },
	{ "u_Light_Position_2",			-1,		CShader::UniformIndex::Light_Position_2 },
	{ "u_Light_Direction_2",		-1,		CShader::UniformIndex::Light_Direction_2 },
	{ "u_Light_Ambient_2",			-1,		CShader::UniformIndex::Light_Ambient_2 },
	{ "u_Light_Diffuse_2",			-1,		CShader::UniformIndex::Light_Diffuse_2 },
	{ "u_Light_Specular_2",			-1,		CShader::UniformIndex::Light_Specular_2 },
	{ "u_Light_Attenuation_2",		-1,		CShader::UniformIndex::Light_Attenuation_2 },
    { "u_Light_FallOffAngle_2",		-1,		CShader::UniformIndex::Light_FallOffAngle_2 },
    { "u_Light_FallOffExp_2",		-1,		CShader::UniformIndex::Light_FallOffExp_2 },

	{ "u_Light_Enable_3",			-1,		CShader::UniformIndex::Light_Enable_3 },
	{ "u_Light_Position_3",			-1,		CShader::UniformIndex::Light_Position_3 },
	{ "u_Light_Direction_3",		-1,		CShader::UniformIndex::Light_Direction_3 },
	{ "u_Light_Ambient_3",			-1,		CShader::UniformIndex::Light_Ambient_3 },
	{ "u_Light_Diffuse_3",			-1,		CShader::UniformIndex::Light_Diffuse_3 },
	{ "u_Light_Specular_3",			-1,		CShader::UniformIndex::Light_Specular_3 },
	{ "u_Light_Attenuation_3",		-1,		CShader::UniformIndex::Light_Attenuation_3 },
    { "u_Light_FallOffAngle_3",		-1,		CShader::UniformIndex::Light_FallOffAngle_3 },
    { "u_Light_FallOffExp_3",		-1,		CShader::UniformIndex::Light_FallOffExp_3 },
};

// ----------------------------------------------------------------------//

CShader::CShader( Type type )
{
	m_iD = glCreateShader( Value(type) );
}

// ----------------------------------------------------------------------//

CShader::CShader( std::string name, Type type )
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
bool CShader::Load(io::CMemStream* ms)
{
    return false;
}
// ----------------------------------------------------------------------//

bool CShader::Save(io::CMemStream* ms)
{
    return false;
}
// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
