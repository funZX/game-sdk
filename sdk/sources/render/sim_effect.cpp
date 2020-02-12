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

	m_numAttribs						= CVertexSource::k_Vertex_Attributes_Count;
	m_attributeMask						= 0;
	InitAttributes();

	m_numUniforms						= 0;
    m_uniformMask                       = 0;
    m_uniforms = nullptr;

	m_vshader							= 0;
	m_pshader							= 0;

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

	glDeleteProgram( m_iD );
}

// ----------------------------------------------------------------------//

void CEffect::InitUniforms(unsigned int numUniform)
{
	m_numUniforms = numUniform;
	m_uniforms = SIM_NEW CShader::TUniform[numUniform];

	SIM_MEMSET(m_uniforms, 0, numUniform * sizeof(CShader::TUniform));
}

// ----------------------------------------------------------------------//

void CEffect::InitAttributes()
{
	m_attributes = SIM_NEW CShader::TAttrib[m_numAttribs];
	SIM_MEMCPY(&m_attributes[0], &CShader::Attributes[0], m_numAttribs * sizeof(CShader::TAttrib));
}

// ----------------------------------------------------------------------//

void CEffect::AddAttribute(const std::string& name)
{
    const CShader::TAttrib* attrib = CShader::FindAttrib(name);

    SIM_ASSERT(attrib != nullptr);

	zpl_bit_set(&m_attributeMask, Value(attrib->m_compFormat));
}

// ----------------------------------------------------------------------//

void CEffect::SetAttributes()
{
	for( s32 k = 0; k < m_numAttribs; k++ )
    {
		CShader::TAttrib *curAttrib = &m_attributes[ k ];

		if (zpl_bit_get(m_attributeMask, Value(curAttrib->m_compFormat)))
			glBindAttribLocation(m_iD, k, curAttrib->m_name);

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
	driver->EnableBlendFunc( m_technique.blendfunc );
	driver->EnableDepthFunc( m_technique.depthfunc );
}

// ----------------------------------------------------------------------//

void CEffect::ApplyTextures( CDriver* driver )
{
	for( u32 i = 0; i < CDriver::k_Texture_Channels_Count; i++ )
	{
		if ( m_textures[ i ] != nullptr )
		{
			driver->SetTextureChannel( static_cast<CDriver::TextureChannel>(i) );
			driver->BindTexture( CDriver::TextureTarget::Texture2D, m_textures[ i ]->GetID() );
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
                zpl_bit_set(&m_uniformMask, isUsingWorldInverseMatrix);
				break;
			case CShader::UniformIndex::Matrix_World_InverseT:
                zpl_bit_set(&m_uniformMask, isUsingWorldInverseTMatrix);
				break;
			case CShader::UniformIndex::Matrix_View_Inverse:
                zpl_bit_set(&m_uniformMask, isUsingViewInverseMatrix);
				break;
			case CShader::UniformIndex::Matrix_View_InverseT:
                zpl_bit_set(&m_uniformMask, isUsingViewInverseTMatrix);
				break;
			case CShader::UniformIndex::Matrix_Normal:
                zpl_bit_set(&m_uniformMask, isUsingNormalMatrix);
				break;
			case CShader::UniformIndex::Matrix_WorldView:
                zpl_bit_set(&m_uniformMask, isUsingWorldViewMatrix);
				break;
			case CShader::UniformIndex::Matrix_ViewProjection:
                zpl_bit_set(&m_uniformMask, isUsingViewProjectionMatrix);
				break;
			case CShader::UniformIndex::Matrix_WorldViewProjection:
                zpl_bit_set(&m_uniformMask, isUsingWorldViewProjectionMatrix);
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

	CVertexSource::AttributeFormat vertexFormat   = vertexSource->m_vertexFormat;
	CVertexSource::AttributeStride vertexStride   = vertexSource->m_vertexStride;
	u32 vboOff         = 0;

	for( s32 k = 0; k < m_numAttribs; k++ )
	{
		CShader::TAttrib *crtAttrib	= &m_attributes[ k ];

		CVertexSource::AttributeStride attribStride	= crtAttrib->m_compStride;
		CVertexSource::AttributeFormat attribFormat	= crtAttrib->m_compFormat;
		CVertexSource::AttributeSize attribSize		= crtAttrib->m_compSize;
		CVertexSource::AttributeType attribType		= crtAttrib->m_compType;

#if SIM_DEBUG
        s32 loc = glGetAttribLocation(m_iD, crtAttrib->m_name);
        SIM_ASSERT(-1 == loc || loc == k);
#endif

		if( CVertexSource::AttributeFormat::None != ( vertexFormat & attribFormat )  )
		{
            crtAttrib->m_compOffset = vboOff;

			driver->SetVertexAttribute( crtAttrib, vertexSource );
			driver->EnableVertexAttribute( crtAttrib->m_location );

			vboOff += Value( attribStride );
		}
		else
		{
			if ( CVertexSource::AttributeFormat::None  != (vertexFormat & attribFormat ) )
				vboOff += Value(attribStride);

			driver->DisableVertexAttribute( crtAttrib->m_location );
		}
	}

	SIM_CHECK_OPENGL();
}
// ----------------------------------------------------------------------//
bool CEffect::Load(io::CMemStream* ms)
{
    return false;
}
// ----------------------------------------------------------------------//
bool CEffect::Save(io::CMemStream* ms)
{
    return false;
}
// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
