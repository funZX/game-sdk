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

CEffect::CEffect( std::string name )
{
	m_name				= name;
	m_iD				= glCreateProgram();

	m_numUniforms       = 0;

	m_isUsingWorldInverseMatrix			= false;
	m_isUsingWorldInverseTMatrix		= false;
	m_isUsingViewInverseMatrix			= false;
	m_isUsingViewInverseTMatrix			= false;
	m_isUsingNormalMatrix				= false;
	m_isUsingWorldViewMatrix			= false;
	m_isUsingViewProjectionMatrix		= false;
	m_isUsingWorldViewProjectionMatrix	= false;

	SIM_MEMSET( m_uniforms, 0, sizeof( m_uniforms ) );
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

CEffect::~CEffect()
{
	glDeleteProgram( m_iD );
}

// ----------------------------------------------------------------------//

void CEffect::BindAllAttributes()
{
    for( s32 k = 0; k < CShader::k_Attribute_Count; k++ )
    {
		CShader::TAttrib *curAttrib = &CShader::Attributes[ k ];
		
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
	for( u32 i = 0; i < CDriver::k_Select_Texture_Count; i++ )
	{
		if ( m_textures[ i ] != NULL )
		{
			driver->SelectTexture( ( CDriver::K_SELECT_TEXTURE ) i );
			driver->BindTexture( m_textures[ i ]->GetID() );
		}
	}
}

// ----------------------------------------------------------------------//

void CEffect::GetAllUniformLocations()
{
    for( s32 k = 0; k < CShader::k_Uniform_Count; k++ )
    {
		CShader::TUniform *curUni = &CShader::Uniforms[ k ];

		s32 loc = glGetUniformLocation( m_iD, curUni->m_name );

        if( loc != -1 )
		{
			curUni->m_location = loc;

			switch( curUni->m_ID )
			{
			case CShader::k_Uniform_Matrix_World_Inverse:
				m_isUsingWorldInverseMatrix = true;
				break;
			case CShader::k_Uniform_Matrix_World_InverseT:
				m_isUsingWorldInverseTMatrix = true;
				break;
			case CShader::k_Uniform_Matrix_View_Inverse:
				m_isUsingViewInverseMatrix = true;
				break;
			case CShader::k_Uniform_Matrix_View_InverseT:
				m_isUsingViewInverseTMatrix = true;
				break;
			case CShader::k_Uniform_Matrix_Normal:
				m_isUsingNormalMatrix = true;
				break;
			case CShader::k_Uniform_Matrix_WorldView:
				m_isUsingWorldViewMatrix = true;
				break;
			case CShader::k_Uniform_Matrix_ViewProjection:
				m_isUsingViewProjectionMatrix = true;
				break;
			case CShader::k_Uniform_Matrix_WorldViewProjection:
				m_isUsingWorldViewProjectionMatrix = true;
				break;
			}

			SIM_MEMCPY( &m_uniforms[ m_numUniforms++ ], curUni, sizeof( CShader::TUniform ) );
        }

		SIM_CHECK_OPENGL();
    }
}

// ----------------------------------------------------------------------//

void CEffect::Load( CShader *vsh, CShader *fsh )
{
	glAttachShader( m_iD, vsh->GetID() );
	glAttachShader( m_iD, fsh->GetID() );

	BindAllAttributes();

	glLinkProgram( m_iD );

#if SIM_DEBUG
	s32 status = GL_TRUE, infologLength = 0, charsWritten  = 0;
	char *infoLog = NULL;

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

	GetAllUniformLocations();
}

// ----------------------------------------------------------------------//

void CEffect::Use( CDriver *driver, CVertexSource *vertexSource )
{
	glUseProgram( m_iD );

	ApplyTextures( driver );
	ApplyTechnique( driver );

	// Uniforms
	driver->UpdateUniforms( this );

	for( s32 k = 0; k < m_numUniforms; k++ )
	{
		CShader::TUniform *uni = &m_uniforms[ k ];
		driver->SetUniform( uni );
	}

	// Attributes
	void *vboData	   = vertexSource->GetVboData();
	u32 vertexFormat   = vertexSource->GetVertexFormat();
	u32 vertexStride   = vertexSource->GetVertexStride();
	s32 vboOff         = 0;

	for( s32 k = 0; k < CShader::k_Attribute_Count; k++ )
	{
		CShader::TAttrib *crtAttrib	= &CShader::Attributes[ k ];

		CVertexSource::K_VERTEX_ATTRIBUTE_OFFSET attribOff		= crtAttrib->m_compOff;
		CVertexSource::K_VERTEX_ATTRIBUTE_FORMAT attribFormat	= crtAttrib->m_compFormat;
		CVertexSource::K_VERTEX_ATTRIBUTE_SIZE attribSize		= crtAttrib->m_compSize;
		CVertexSource::K_VERTEX_ATTRIBUTE_TYPE attribType		= crtAttrib->m_compType;

		s32 loc = glGetAttribLocation( m_iD, crtAttrib->m_name );

		SIM_ASSERT( loc == -1 || loc == k );

		if( loc != -1 && 0 != ( vertexFormat & attribFormat )  )
		{
			void *vertexData = (void*) ( ( size_t ) vboData + vboOff );

			driver->SetVertexAttribute( crtAttrib, vertexData, vertexStride );
			driver->EnableVertexAttribute( crtAttrib );

			vboOff += attribOff;
		}
		else
		{
			if ( 0 != (vertexFormat & attribFormat ) )
				vboOff += attribOff;

			driver->DisableVertexAttribute( crtAttrib );
		}
	}

	SIM_CHECK_OPENGL();
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
