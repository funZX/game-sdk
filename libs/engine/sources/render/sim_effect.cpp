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

#include <render/sim_shader.h>
#include <render/sim_effect.h>
#include <render/sim_texture.h>
#include <render/sim_driver.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CEffect::CEffect()
{
	m_iD								= glCreateProgram();

	m_uniforms							= nullptr;
	m_numUniforms						= 0;

	m_attributes						= nullptr;
	m_numAttrib							= 0;

	m_vshader							= 0;
	m_pshader							= 0;

	m_isUsingWorldInverseMatrix			= false;
	m_isUsingWorldInverseTMatrix		= false;
	m_isUsingViewInverseMatrix			= false;
	m_isUsingViewInverseTMatrix			= false;
	m_isUsingNormalMatrix				= false;
	m_isUsingWorldViewMatrix			= false;
	m_isUsingViewProjectionMatrix		= false;
	m_isUsingWorldViewProjectionMatrix	= false;

	SIM_MEMSET( m_textures, 0, sizeof( m_textures ) );

	m_technique.depthtest				= true;
	m_technique.depthmask				= true;
	m_technique.cullface				= true;
	m_technique.alphatest				= false;

	m_technique.blending				= false;

	m_technique.blendfunc.equation		= GL_FUNC_ADD;
	m_technique.blendfunc.src			= GL_SRC_ALPHA;
	m_technique.blendfunc.dst			= GL_ONE_MINUS_SRC_ALPHA;

	m_technique.depthfunc.equation		= GL_LESS;
}

// ----------------------------------------------------------------------//

CEffect::CEffect( std::string name )
	: CEffect()
{
	m_name = name;
}

// ----------------------------------------------------------------------//

CEffect::~CEffect()
{
	SIM_SAFE_DELETE(m_vshader);
	SIM_SAFE_DELETE(m_pshader);

	SIM_SAFE_DELETE_ARRAY(m_uniforms);
	SIM_SAFE_DELETE_ARRAY(m_attributes);

	glDeleteProgram( m_iD );
}

// ----------------------------------------------------------------------//

void CEffect::InitAttributes(unsigned int numAttrib)
{
	m_numAttrib = numAttrib;
	m_attributes = SIM_NEW CShader::TAttrib[numAttrib];

	SIM_MEMSET(m_attributes, 0, numAttrib * sizeof(CShader::TAttrib));
}

// ----------------------------------------------------------------------//

void CEffect::InitUniforms(unsigned int numUniform)
{
	m_numUniforms = numUniform;
	m_uniforms = SIM_NEW CShader::TUniform[numUniform];

	SIM_MEMSET(m_uniforms, 0, numUniform * sizeof(CShader::TUniform));
}

// ----------------------------------------------------------------------//

void CEffect::AddAttribute(const std::string& name, int index)
{
	const CShader::TAttrib* attrib = CShader::FindAttrib( name );

	SIM_ASSERT( attrib != nullptr );

	SIM_MEMCPY( &m_attributes[index], attrib, sizeof(CShader::TAttrib) );
}

// ----------------------------------------------------------------------//

void CEffect::SetAttributes()
{
    for( s32 k = 0; k < m_numAttrib; k++ )
    {
		CShader::TAttrib *curAttrib = &m_attributes[ k ];
		
		glBindAttribLocation( m_iD, k, curAttrib->m_name );
		curAttrib->m_location = k;

		SIM_CHECK_OPENGL();
    }
}

// ----------------------------------------------------------------------//

void CEffect::ApplyTechnique( CDriver* driver )
{
	driver->EnableDepthTest( m_technique.depthtest );
	driver->EnableDepthMask( m_technique.depthmask );
	driver->EnableCulling( m_technique.cullface );
	driver->EnableBlending( m_technique.blending );
	driver->EnableBlendFunc( &m_technique.blendfunc );
	driver->EnableDepthFunc( &m_technique.depthfunc );
}

// ----------------------------------------------------------------------//

void CEffect::ApplyTextures( CDriver* driver )
{
	for( u32 i = 0; i < CDriver::k_Texture_Channels_Count; i++ )
	{
		if ( m_textures[ i ] != nullptr )
		{
			driver->SetTextureChannel( static_cast<CDriver::TextureChannel>(i) );
			driver->BindTexture( m_textures[ i ]->GetID() );
		}
	}
}

// ----------------------------------------------------------------------//

void CEffect::AddUniform(const std::string& name, int index)
{
	const CShader::TUniform* uni = CShader::FindUniform(name);

	SIM_ASSERT( uni != nullptr );

	SIM_MEMCPY( &m_uniforms[index], uni, sizeof(CShader::TUniform) );
}

// ----------------------------------------------------------------------//

void CEffect::SetUniforms()
{
    for( s32 k = 0; k < m_numUniforms; k++ )
    {
		CShader::TUniform *curUni = &m_uniforms[ k ];

		s32 loc = glGetUniformLocation( m_iD, curUni->m_name );

        if( loc != -1 )
		{
			curUni->m_location = loc;

			switch( curUni->m_index )
			{
			case CShader::UniformIndex::Matrix_World_Inverse:
				m_isUsingWorldInverseMatrix = true;
				break;
			case CShader::UniformIndex::Matrix_World_InverseT:
				m_isUsingWorldInverseTMatrix = true;
				break;
			case CShader::UniformIndex::Matrix_View_Inverse:
				m_isUsingViewInverseMatrix = true;
				break;
			case CShader::UniformIndex::Matrix_View_InverseT:
				m_isUsingViewInverseTMatrix = true;
				break;
			case CShader::UniformIndex::Matrix_Normal:
				m_isUsingNormalMatrix = true;
				break;
			case CShader::UniformIndex::Matrix_WorldView:
				m_isUsingWorldViewMatrix = true;
				break;
			case CShader::UniformIndex::Matrix_ViewProjection:
				m_isUsingViewProjectionMatrix = true;
				break;
			case CShader::UniformIndex::Matrix_WorldViewProjection:
				m_isUsingWorldViewProjectionMatrix = true;
				break;
			}
        }

		SIM_CHECK_OPENGL();
    }
}

// ----------------------------------------------------------------------//

void CEffect::Load( const s8* vsource, const s8* psource )
{
	m_vshader = SIM_NEW CShader( CShader::Type::Vertex );
	m_vshader->Load( vsource );

	m_pshader = SIM_NEW CShader( CShader::Type::Pixel);
	m_pshader->Load(psource);

	glAttachShader( m_iD, m_vshader->GetID() );
	glAttachShader( m_iD, m_pshader->GetID() );

	SetAttributes();

	glLinkProgram( m_iD );

#if SIM_DEBUG
	s32 status = GL_TRUE, infologLength = 0, charsWritten  = 0;
	char *infoLog = nullptr;

	glGetProgramiv( m_iD, GL_LINK_STATUS, &status );
	glGetProgramiv( m_iD, GL_INFO_LOG_LENGTH, &infologLength );

	if( infologLength > 0 )
	{
		infoLog = SIM_NEW char[ infologLength ];

		glGetProgramInfoLog( m_iD, infologLength, &charsWritten, infoLog );
		SIM_PRINT("%s\n",infoLog);

		SIM_SAFE_DELETE_ARRAY( infoLog );
	}

	SIM_ASSERT( status == GL_TRUE );
#endif // SIM_DEBUG

	SetUniforms();
}

// ----------------------------------------------------------------------//

void CEffect::Render(CDriver *driver)
{
	glUseProgram(m_iD);

	driver->UpdateUniforms(this);

	for (s32 k = 0; k < m_numUniforms; k++)
	{
		CShader::TUniform *crtUniform = &m_uniforms[k];

		if ( crtUniform->m_location != -1 )
			driver->SetUniform(crtUniform);
	}

	ApplyTextures(driver);
	ApplyTechnique(driver);
}

// ----------------------------------------------------------------------//

void CEffect::Bind( CDriver *driver, CVertexSource *vertexSource )
{
	// Attributes
	void *vboData = vertexSource->GetVboData();

	CVertexSource::AttributeFormat vertexFormat   = vertexSource->GetVertexFormat();
	CVertexSource::AttributeStride vertexStride   = vertexSource->GetVertexStride();
	u32 vboOff         = 0;

	for( s32 k = 0; k < m_numAttrib; k++ )
	{
		CShader::TAttrib *crtAttrib	= &m_attributes[ k ];

		CVertexSource::AttributeStride attribStride	= crtAttrib->m_compStride;
		CVertexSource::AttributeFormat attribFormat	= crtAttrib->m_compFormat;
		CVertexSource::AttributeSize attribSize		= crtAttrib->m_compSize;
		CVertexSource::AttributeType attribType		= crtAttrib->m_compType;

#if SIM_DEBUG
		s32 loc = glGetAttribLocation( m_iD, crtAttrib->m_name );
		SIM_ASSERT( loc == -1 || loc == crtAttrib->m_location);
#endif

		if( crtAttrib->m_location != -1 && 
			CVertexSource::AttributeFormat::None != ( vertexFormat & attribFormat )  )
		{
			void *vertexData = (void*) ( ( size_t ) vboData + vboOff );

			driver->SetVertexAttribute( crtAttrib, vertexData, vertexStride );
			driver->EnableVertexAttribute( crtAttrib );

			vboOff += EnumValue(attribStride);
		}
		else
		{
			if ( CVertexSource::AttributeFormat::None  != (vertexFormat & attribFormat ) )
				vboOff += EnumValue(attribStride);

			driver->DisableVertexAttribute( crtAttrib );
		}
	}

	SIM_CHECK_OPENGL();
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
