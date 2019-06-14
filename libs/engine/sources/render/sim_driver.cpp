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

#include <core/sim_core.h>

#include <render/sim_render.h>

#include <render/sim_driver.h>
#include <render/sim_render_texture.h>
#include <render/sim_rect_2d.h>
#include <render/sim_batch_2d.h>
#include <render/sim_effect.h>
#include <render/sim_material.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//
CRect2D				ScreenRectStatic(0.0f, 0.0f, 0.0f, 0.0f);
CRect2D*			CDriver::ScreenRect = &ScreenRectStatic;
// ----------------------------------------------------------------------//

// ----------------------------------------------------------------------//

CDriver::CDriver()
{
	m_batch2D	= SIM_NEW CBatch2D( this );

	Mat4StackClear( &m_worldStack );
	Mat4StackClear( &m_viewStack );
	Mat4StackClear( &m_projectionStack );

	for( s32 i = 0; i < k_Texture_Channels_Count; i++ )
		Mat4StackClear( &m_textureStack[ i ] );

	for( s32 i = 0; i < k_Animation_Bones_Max; i++ )
		zpl_mat4_identity( &m_boneArrayMatrix[ i ] );

	zpl_mat4_identity( &m_worldInverseMatrix );
	zpl_mat4_identity( &m_worldInverseTMatrix );
	m_isWorldMatrixDirty = true;
	m_isActiveStackAlteringWorldMatrix = true;

	zpl_mat4_identity( &m_viewInverseMatrix );
	zpl_mat4_identity( &m_viewInverseTMatrix );
	m_isViewMatrixDirty = true;
	m_isActiveStackAlteringViewMatrix = true;

	zpl_mat4_identity( &m_worldViewMatrix );
	m_isWorldViewMatrixDirty = true;
	m_isActiveStackAlteringWorldViewMatrix = true;

	zpl_mat4_identity( &m_viewProjectionMatrix );
	m_isViewProjectionMatrixDirty = true;
	m_isActiveStackAlteringViewProjectionMatrix = true;

	zpl_mat4_identity( &m_worldViewProjectionMatrix );
	m_isWorldViewProjectionMatrixDirty = true;
	m_isActiveStackAlteringWorldViewProjectionMatrix = true;

    zpl_mat3_identity( &m_normalMatrix );
	m_isNormalMatrixDirty	= true;
	m_isActiveStackAlteringNormalMatrix = true;

	m_isEnabledBatch2D = false;

    SIM_MEMSET( m_vertexAttributeInfo, 0, sizeof( m_vertexAttributeInfo ) );
	SIM_MEMSET( m_lightParameters, 0, sizeof( m_lightParameters ) );

	m_matrixMode			= MatrixMode::World;
	m_textureChannel        = TextureChannel::Texture_0;
	m_lightChannel          = LightChannel::Light_0;
	m_cullingMode           = CullingMode::CW;

	m_activeStack           = &m_projectionStack;

	SIM_MEMSET( m_textureBind, 0, sizeof( m_textureBind ) );

	m_fogMode		        = FogMode::Linear;
	m_fogDensity	        = 0.0f;
	m_fogStart		        = 0.0f;
	m_fogEnd		        = 0.0f;
    m_fogColor              = col::Black;
	
	m_depthStart			= 0.0f;
	m_depthEnd				= 0.0f;


	m_color                 = col::White;
	m_materialAmbient       = col::White;
	m_materialDiffuse       = col::White;
	m_materialSpecular      = col::White;
    m_materialReflective    = col::Black;

	m_materialShininess		= 0.0f;
	m_materialRefraction	= 0.0f;

	m_timer					= 0.0f;
	m_timerSin				= 0.0f;
	m_timerCos				= 0.0f;
	m_timerRot				= 0.0f;

	m_pointSize				= 1.0f;

	m_isCullingEnabled		= true;
	m_isBlendingEnabled		= false;
	m_isDepthTestEnabled	= true;
	m_isDepthMaskEnabled	= true;

	m_blendFunc.equation	= GL_FUNC_ADD;
	m_blendFunc.src			= GL_SRC_ALPHA;
	m_blendFunc.dst			= GL_ONE_MINUS_SRC_ALPHA;

	m_depthFunc.equation	= GL_LESS;

	InitUniform();

	m_crtVertexSource		= nullptr;
	m_crtRenderTexture		= nullptr;

	m_viewportWidth			= 0;
	m_viewportHeight		= 0;
	m_screenWidth			= 0;
	m_screenHeight			= 0;

	m_drawCallCount			= 0;
	m_vertexCount			= 0;
}

// ----------------------------------------------------------------------//

void CDriver::Initialize()
{
	glDepthFunc ( GL_LESS );
	glFrontFace ( GL_CCW );
	glCullFace  ( GL_BACK );
	
	glBlendEquation( GL_FUNC_ADD );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	
	glEnable( GL_CULL_FACE );
	
	glDisable( GL_BLEND );
	glEnable( GL_DEPTH_TEST );

	glDepthMask( GL_TRUE );

	glActiveTexture( GL_TEXTURE0 );
}

// ----------------------------------------------------------------------//

CDriver::~CDriver()
{
	SIM_SAFE_DELETE( m_batch2D );
}

// ----------------------------------------------------------------------//

void CDriver::Flush2D()
{
	m_batch2D->Render( this );

	m_crtVertexSource = nullptr;
}

// ----------------------------------------------------------------------//

bool CDriver::EnableCulling( bool val )
{
	bool isEnabled = m_isCullingEnabled;

	if( val != m_isCullingEnabled )
	{
		val ?
			glEnable( GL_CULL_FACE ) :
			glDisable( GL_CULL_FACE );

		m_isCullingEnabled = val;
	}

	return isEnabled;
}

// ----------------------------------------------------------------------//

bool CDriver::EnableBlending( bool val )
{
	bool isEnabled = m_isBlendingEnabled;

	if( val != m_isBlendingEnabled )
	{
		val ?
			glEnable( GL_BLEND ) :
			glDisable( GL_BLEND );

		m_isBlendingEnabled = val;
	}

	return isEnabled;
}

// ----------------------------------------------------------------------//

bool CDriver::EnableDepthTest( bool val )
{
	bool isEnabled = m_isDepthTestEnabled;

	if( val != m_isDepthTestEnabled )
	{
		val ?
			glEnable( GL_DEPTH_TEST ) :
			glDisable( GL_DEPTH_TEST );

		m_isDepthTestEnabled = val;
	}

	return isEnabled;
}

// ----------------------------------------------------------------------//

bool  CDriver::EnableDepthMask( bool val )
{
	bool isEnabled = m_isDepthMaskEnabled;

	if( val != m_isDepthMaskEnabled )
	{
		val ?
			glDepthMask( GL_TRUE ) :
			glDepthMask( GL_FALSE );

		m_isDepthMaskEnabled = val;
	}

	return isEnabled;
}

// ----------------------------------------------------------------------//

void CDriver::Tick( f32 dt )
{
	m_timerRot += (dt * 1000.0f) / 360.0f;

	f32 angle = 100.0f * m_timerRot;
	while (angle > 360.0f )
		angle -= 360.0f;

	f32 toRad	= zpl_to_radians( angle );

	m_timerSin	= zpl_sin( toRad );
	m_timerCos	= zpl_cos( toRad );

	m_timer += dt; 
}

// ----------------------------------------------------------------------//

void  CDriver::SetDepthRange( f32 start, f32 end )
{
	m_depthStart	= start;
	m_depthEnd		= end;

	glDepthRangef( start, end );
}

// ----------------------------------------------------------------------//

u32  CDriver::BindTexture( u32 tex )
{
	u32 old = 0;

	auto texChannel = Value(m_textureChannel);

	if( m_textureBind[texChannel] != tex )
	{
		glBindTexture( GL_TEXTURE_2D, tex );

		old = m_textureBind[texChannel];
		m_textureBind[texChannel] = tex;

		SIM_CHECK_OPENGL();
	}

	return old;
}

// ----------------------------------------------------------------------//

CRenderTexture* CDriver::BindRenderTexture( CRenderTexture* texture )
{
	if (texture == m_crtRenderTexture)
		return m_crtRenderTexture;

	if (texture != nullptr)
	{
		glBindFramebuffer( GL_FRAMEBUFFER, texture->GetBufferID() );
		SIM_CHECK_OPENGL();

		SetViewport( texture->GetWidth(), texture->GetHeight() );
	}
	else
	{
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
		SIM_CHECK_OPENGL();

		SetViewport( m_screenWidth, m_screenHeight );
	}


	CRenderTexture* old = m_crtRenderTexture;
	m_crtRenderTexture = texture;

	return old;
}

// ----------------------------------------------------------------------//

void CDriver::ComputeNormalMatrix()
{
	Mat4 m;

    zpl_mat4_mul( GetViewMatrix(), &m, GetWorldMatrix());
    zpl_mat4_inverse( &m, &m );
    zpl_mat4_transpose( &m );

    m_normalMatrix.x = m.x.xyz;
    m_normalMatrix.y = m.y.xyz;
    m_normalMatrix.z = m.z.xyz;
}

// ----------------------------------------------------------------------//

CDriver::TextureChannel CDriver::SetTextureChannel(TextureChannel texChannel)
{
	TextureChannel old = m_textureChannel;

	if(m_textureChannel == texChannel)
		return old;

	glActiveTexture( GL_TEXTURE0 + Value(texChannel));

	m_textureChannel = texChannel;

	SIM_CHECK_OPENGL();

	return old;
}

// ----------------------------------------------------------------------//

void CDriver::EnableBlendFunc( TBlendFunc blendfunc )
{
	if ( blendfunc.equation != m_blendFunc.equation )
	{
		m_blendFunc.equation = blendfunc.equation;

		glBlendEquation( blendfunc.equation );
	}

	if ( blendfunc.src != m_blendFunc.src ||
		 blendfunc.dst != m_blendFunc.dst )
	{
		m_blendFunc.src = blendfunc.src;
		m_blendFunc.dst = blendfunc.dst;

		glBlendFunc( blendfunc.src, blendfunc.dst );
	}
}

// ----------------------------------------------------------------------//

void CDriver::EnableBlendFunc( u32 equation, u32 src, u32 dst )
{
	TBlendFunc blendfunc;

	blendfunc.equation	= equation;
	blendfunc.src		= src;
	blendfunc.dst		= dst;

	EnableBlendFunc( blendfunc );
}

// ----------------------------------------------------------------------//

void CDriver::EnableDepthFunc(TDepthFunc depthfunc)
{
	if (depthfunc.equation != m_depthFunc.equation)
	{
		m_depthFunc.equation = depthfunc.equation;

		glDepthFunc(depthfunc.equation);
	}
}

// ----------------------------------------------------------------------//

void CDriver::EnableDepthFunc(u32 equation)
{
	TDepthFunc depthfunc;
	depthfunc.equation = equation;
	EnableDepthFunc(depthfunc);
}

// ----------------------------------------------------------------------//

CDriver::LightChannel CDriver::SetLightChannel(LightChannel lightChannel )
{
	LightChannel old = m_lightChannel;

	if(m_lightChannel == lightChannel)
		return old;

	m_lightChannel = lightChannel;

	SIM_CHECK_OPENGL();

	return old;
}

CDriver::CullingMode CDriver::SetCullingMode( CullingMode cullingMode )
{
	CullingMode old = m_cullingMode;

	if(m_cullingMode == cullingMode)
		return old;

	switch(Value(cullingMode))
	{
	case CullingMode::CW:
		{
			glFrontFace( GL_CW );
			break;
		}

	case CullingMode::CCW:
		{
			glFrontFace( GL_CCW );
			break;
		}
	}

	m_cullingMode = cullingMode;

	SIM_CHECK_OPENGL();

	return old;
}

// ----------------------------------------------------------------------//

void CDriver::MatrixLoadIdentity()
{
	Mat4StackIdentity( m_activeStack );
	MatrixDirty();
}

// ----------------------------------------------------------------------//

void CDriver::MatrixLoad( Mat4 *mat )
{
    Mat4StackLoad( m_activeStack, mat );
	MatrixDirty();
}

// ----------------------------------------------------------------------//

void CDriver::MatrixMultiply( Mat4 *mat )
{
    Mat4StackMultiply( m_activeStack, mat );
	MatrixDirty();
}

// ----------------------------------------------------------------------//

CDriver::MatrixMode CDriver::SetMatrixMode(MatrixMode matrixMode)
{
	MatrixMode old = m_matrixMode;

	if (m_matrixMode == matrixMode)
		return old;

	switch( Value(matrixMode) )
	{
	case MatrixMode::World:
		{
			m_activeStack = &m_worldStack;

			m_isActiveStackAlteringWorldMatrix = true;
			m_isActiveStackAlteringViewMatrix = false;
			m_isActiveStackAlteringWorldViewMatrix = true;
			m_isActiveStackAlteringViewProjectionMatrix = false;
			m_isActiveStackAlteringWorldViewProjectionMatrix = true;
			m_isActiveStackAlteringNormalMatrix = true;

			break;
		}

	case MatrixMode::View:
		{
			m_activeStack = &m_viewStack;

			m_isActiveStackAlteringWorldMatrix = false;
			m_isActiveStackAlteringViewMatrix = true;
			m_isActiveStackAlteringWorldViewMatrix = true;
			m_isActiveStackAlteringViewProjectionMatrix = true;
			m_isActiveStackAlteringWorldViewProjectionMatrix = true;
			m_isActiveStackAlteringNormalMatrix = true;

			break;
		}

	case MatrixMode::Projection:
		{
			m_activeStack = &m_projectionStack;

			m_isActiveStackAlteringWorldMatrix = false;
			m_isActiveStackAlteringViewMatrix = false;
			m_isActiveStackAlteringWorldViewMatrix = false;
			m_isActiveStackAlteringViewProjectionMatrix = true;
			m_isActiveStackAlteringWorldViewProjectionMatrix = true;
			m_isActiveStackAlteringNormalMatrix = false;

			break;
		}

	case MatrixMode::Texture:
		{
			m_activeStack = &m_textureStack[ Value(m_textureChannel) ];

			m_isActiveStackAlteringWorldMatrix = false;
			m_isActiveStackAlteringViewMatrix = false;
			m_isActiveStackAlteringWorldViewMatrix = false;
			m_isActiveStackAlteringViewProjectionMatrix = false;
			m_isActiveStackAlteringWorldViewProjectionMatrix = false;
			m_isActiveStackAlteringNormalMatrix = false;

			break;
		}
	}

	m_matrixMode = matrixMode;

	return old;
}

// ----------------------------------------------------------------------//
bool CDriver::EnableBatch2D( bool isEnabled )
{
	bool wasBatchEnabled = m_isEnabledBatch2D;

	if ( m_isEnabledBatch2D != isEnabled )
	{
		m_isEnabledBatch2D = isEnabled;

		if ( wasBatchEnabled )
			Flush2D();
	}

	return wasBatchEnabled;
}

// ----------------------------------------------------------------------//

void CDriver::MatrixPush()
{
	Mat4StackPush( m_activeStack );
	MatrixDirty();
}

// ----------------------------------------------------------------------//

void CDriver::MatrixPop()
{
	Mat4StackPop( m_activeStack );
	MatrixDirty();
}

// ----------------------------------------------------------------------//

void CDriver::MatrixTranslate( Vec3 translation )
{
    Mat4StackTranslate( m_activeStack, translation );
	MatrixDirty();
}

// ----------------------------------------------------------------------//

void CDriver::MatrixRotate( Vec3 axis, f32 angle )
{
	Mat4StackRotate( m_activeStack, axis, angle );
	MatrixDirty();
}

// ----------------------------------------------------------------------//

void CDriver::MatrixScale( Vec3 scale )
{
    Mat4StackScale( m_activeStack, scale );
	MatrixDirty();
}

// ----------------------------------------------------------------------//

void CDriver::Clear( Vec4 color )
{
	m_crtVertexSource	= 0;
	m_crtRenderTexture	= 0;

	glClearColor( color.x, color.y, color.z, color.w );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

// ----------------------------------------------------------------------//

void CDriver::ClearColor( Vec4 color )
{
	glClearColor( color.x, color.y, color.z, color.w );
	glClear( GL_COLOR_BUFFER_BIT );
}
// ----------------------------------------------------------------------//

void CDriver::SetScreenSize( u32 width, u32 height )
{
	// Should be called only once.

	m_screenWidth = width;
	m_screenHeight = height;

	CDriver::ScreenRect->Resize( width, height );
}

// ----------------------------------------------------------------------//

void CDriver::SetViewport( u32 width, u32 height )
{
	if (width != m_viewportWidth || height != m_viewportHeight)
	{
		glViewport(0, 0, width, height);

		m_viewportWidth = width;
		m_viewportHeight = height;
	}
}

// ----------------------------------------------------------------------//

void CDriver::SetVertexAttribute( CShader::TAttrib* attrib, void *vertexData, 
	CVertexSource::AttributeStride vertexStride )
{
	TVertexAttributeInfo *attribInfo = &m_vertexAttributeInfo[Value(attrib->m_compIndex)];

    if( vertexData != attribInfo->m_vertexData )
    {
		glVertexAttribPointer( 
			attrib->m_location,
			Value(attrib->m_compSize),
			Value(attrib->m_compType),
			GL_FALSE,
			Value(vertexStride),
			vertexData 
			);

        attribInfo->m_vertexData = vertexData;
    }

	SIM_CHECK_OPENGL();
}

// ----------------------------------------------------------------------//

void CDriver::EnableVertexAttribute( CShader::TAttrib* attrib )
{
	TVertexAttributeInfo *attribInfo = &m_vertexAttributeInfo[ Value(attrib->m_compIndex) ];

	if( !attribInfo->m_isEnabled )
    {
		glEnableVertexAttribArray( attrib->m_location );
        attribInfo->m_isEnabled = true;
    }

	SIM_CHECK_OPENGL();
}

// ----------------------------------------------------------------------//

void CDriver::DisableVertexAttribute( CShader::TAttrib* attrib )
{
	TVertexAttributeInfo *attribInfo = &m_vertexAttributeInfo[Value(attrib->m_compIndex)];

    if( attribInfo->m_isEnabled )
	{
		glDisableVertexAttribArray( attrib->m_location );
        attribInfo->m_isEnabled = false;
    }

	SIM_CHECK_OPENGL();
}

// ----------------------------------------------------------------------//

void CDriver::UpdateUniforms( CEffect *effect )
{
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_World) ].m_value		= (void*) GetWorldMatrix();
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_View) ].m_value		= (void*) GetViewMatrix();
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_Projection) ].m_value= (void*) GetProjectionMatrix();
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_Tex_0) ].m_value		= (void*) GetTextureMatrix( TextureChannel::Texture_0 );
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_Tex_1) ].m_value		= (void*) GetTextureMatrix( TextureChannel::Texture_1 );
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_Tex_2) ].m_value		= (void*) GetTextureMatrix( TextureChannel::Texture_2 );
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_Tex_3) ].m_value		= (void*) GetTextureMatrix( TextureChannel::Texture_3 );
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_Tex_4) ].m_value		= (void*) GetTextureMatrix( TextureChannel::Texture_4 );

	if( m_isWorldMatrixDirty )
	{
		if( effect->m_isUsingWorldInverseMatrix )
			zpl_mat4_inverse( GetWorldMatrix(), &m_worldInverseMatrix );

        if (effect->m_isUsingWorldInverseTMatrix)
        {
            zpl_mat4_copy( &m_worldInverseTMatrix, &m_worldInverseMatrix );
            zpl_mat4_transpose( &m_worldInverseTMatrix );
        }

		m_isWorldMatrixDirty = false;
	}

	if( m_isViewMatrixDirty )
	{
		if ( effect->m_isUsingViewInverseMatrix )
            zpl_mat4_inverse( GetViewMatrix(), &m_viewInverseMatrix );

        if (effect->m_isUsingViewInverseTMatrix)
        {
            zpl_mat4_copy(&m_viewInverseTMatrix, &m_viewInverseMatrix);
            zpl_mat4_transpose(&m_viewInverseTMatrix);
        }	
		
		m_isViewMatrixDirty = false;
	}

	if( m_isWorldViewMatrixDirty )
	{
		if ( effect->m_isUsingWorldViewMatrix )
			zpl_mat4_mul( &m_worldViewMatrix, GetWorldMatrix(), GetViewMatrix());

		m_isWorldViewMatrixDirty = false;
	}

	if( m_isViewProjectionMatrixDirty )
	{
		if ( effect->m_isUsingViewProjectionMatrix )
            zpl_mat4_mul( &m_viewProjectionMatrix, GetViewMatrix(), GetProjectionMatrix());

		m_isViewProjectionMatrixDirty = false;
	}

	if( m_isWorldViewProjectionMatrixDirty )
	{
		if ( effect->m_isUsingWorldViewProjectionMatrix )
		{
			static Mat4 m;

            zpl_mat4_mul( &m, GetViewMatrix(), GetProjectionMatrix());
            zpl_mat4_mul( &m_worldViewProjectionMatrix,  GetWorldMatrix(), &m);
		}

		m_isWorldViewProjectionMatrixDirty = false;
	}

	if( m_isNormalMatrixDirty )
	{
		if ( effect->m_isUsingNormalMatrix )
			ComputeNormalMatrix();

		m_isNormalMatrixDirty = false;
	}
}

// ----------------------------------------------------------------------//

void CDriver::SetUniform( CShader::TUniform* uni )
{
	TUniformInfo *uniInfo		= &m_uniformInfo[ Value(uni->m_index) ];
	SetUniformCallback callback = uniInfo->m_callback;
	
	(this->*callback)( uni->m_location, uniInfo->m_value, uniInfo->m_count );	
}

// ----------------------------------------------------------------------//

void CDriver::SetUniform1f( s32 loc, void *value, u32 count )
{
	f32 val = *( ( f32 * ) value );
	glUniform1f( loc, val );

	SIM_CHECK_OPENGL();
}

// ----------------------------------------------------------------------//

void CDriver::SetUniform1i( s32 loc, void *value, u32 count )
{
	s32 val = *( ( s32 * ) value );
	glUniform1i( loc, val );

	SIM_CHECK_OPENGL();
}

// ----------------------------------------------------------------------//

void CDriver::SetUniform3fv( s32 loc, void *value, u32 count )
{
	f32 *ptr = ( f32 * ) value;
	glUniform3fv( loc, count, ptr );

	SIM_CHECK_OPENGL();
}

// ----------------------------------------------------------------------//

void CDriver::SetUniform4fv( s32 loc, void *value, u32 count )
{
	f32 *ptr = ( f32 * ) value;
	glUniform4fv( loc, count, ptr );

	SIM_CHECK_OPENGL();
}

// ----------------------------------------------------------------------//

void CDriver::SetUniformMatrix3fv( s32 loc, void *value, u32 count )
{
	f32 *ptr = ( f32 * ) value;
	glUniformMatrix3fv( loc, count, GL_FALSE, ptr );

	SIM_CHECK_OPENGL();
}

// ----------------------------------------------------------------------//

void CDriver::SetUniformMatrix4fv( s32 loc, void *value, u32 count )
{
	f32 *ptr = ( f32 * ) value;
	glUniformMatrix4fv( loc, count, GL_FALSE, ptr );

	SIM_CHECK_OPENGL();
}

// ------------------------------------------------------------------//

void CDriver::Render( CVertexGroup* vertexGroup )
{
	SIM_ASSERT( vertexGroup != nullptr );
	SIM_ASSERT( vertexGroup->GetMaterial() != nullptr );
	SIM_ASSERT( vertexGroup->GetMaterial()->GetEffect() != nullptr );

	CEffect* effect				= vertexGroup->GetMaterial()->GetEffect();
	CMaterial* material			= vertexGroup->GetMaterial();
	CVertexSource* vertexSource = vertexGroup->GetVertexSource();

	if ( m_isEnabledBatch2D )
	{
		s32 vboSize = vertexSource->GetVboSize();
		f32* vboData = (f32*)vertexSource->GetVboData();
		u32 vboStride = Value(vertexSource->GetVertexStride());
		u32 vtxSize = vboStride / sizeof(f32);

		for (s32 q = 0; q < vboSize >> 2; q++)
		{
			s32 qOff = (q << 2) * vtxSize;

			m_batch2D->AddQuad(
				material,
				&vboData[qOff + vtxSize * 0],
				&vboData[qOff + vtxSize * 1],
				&vboData[qOff + vtxSize * 2],
				&vboData[qOff + vtxSize * 3]);
		}
	}
	else
	{
		static Primitive primitives[] = {
			Primitive::Lines,
			Primitive::LineStrip,
			Primitive::Points,
			Primitive::Triangles,
			Primitive::TriangleStrips
		};

		SIM_ASSERT(vertexSource != nullptr);

		material->Render(this);
		effect->Render(this);

		if (vertexSource != m_crtVertexSource)
		{
			effect->Bind(this, vertexSource);

			m_crtVertexSource = vertexSource;
		}

		m_drawCallCount += 1;
		m_vertexCount += vertexGroup->GetVboSize();

		u32 rt = Value(primitives[Value(vertexSource->GetType())]);
		glDrawElements(rt, vertexGroup->GetVboSize(), GL_UNSIGNED_SHORT, vertexGroup->GetVboData());
	}
}

// ----------------------------------------------------------------------//

void CDriver::MatrixDirty()
{
	m_isWorldMatrixDirty				= m_isActiveStackAlteringWorldMatrix;
	m_isViewMatrixDirty					= m_isActiveStackAlteringViewMatrix;
	m_isWorldViewMatrixDirty			= m_isActiveStackAlteringWorldViewMatrix;
	m_isViewProjectionMatrixDirty		= m_isActiveStackAlteringViewProjectionMatrix;
	m_isWorldViewProjectionMatrixDirty	= m_isActiveStackAlteringWorldViewProjectionMatrix;
	m_isNormalMatrixDirty				= m_isActiveStackAlteringNormalMatrix;
}

// ----------------------------------------------------------------------//

void CDriver::InitUniform()
{
	// -----------------------------------------
	m_uniformInfo[ Value(CShader::UniformIndex::Time) ].m_value = &m_timer;
	m_uniformInfo[ Value(CShader::UniformIndex::Time) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Time) ].m_callback = &CDriver::SetUniform1f;

	m_uniformInfo[ Value(CShader::UniformIndex::Color) ].m_value = &m_color;
	m_uniformInfo[ Value(CShader::UniformIndex::Color) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Color) ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Point_Size) ].m_value = &m_pointSize;
	m_uniformInfo[ Value(CShader::UniformIndex::Point_Size) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Point_Size) ].m_callback = &CDriver::SetUniform1f;

	// -----------------------------------------
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_World) ].m_value = nullptr;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_World) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_World) ].m_callback = &CDriver::SetUniformMatrix4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_World_Inverse) ].m_value = &m_worldInverseMatrix;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_World_Inverse) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_World_Inverse) ].m_callback = &CDriver::SetUniformMatrix4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_World_InverseT) ].m_value = &m_worldInverseTMatrix;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_World_InverseT) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_World_InverseT) ].m_callback = &CDriver::SetUniformMatrix4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_View) ].m_value = nullptr;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_View) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_View) ].m_callback = &CDriver::SetUniformMatrix4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_View_Inverse) ].m_value = &m_viewInverseMatrix;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_View_Inverse) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_View_Inverse) ].m_callback = &CDriver::SetUniformMatrix4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_View_InverseT) ].m_value = &m_viewInverseTMatrix;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_View_InverseT) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_View_InverseT) ].m_callback = &CDriver::SetUniformMatrix4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_Projection) ].m_value = nullptr;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_Projection) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_Projection) ].m_callback = &CDriver::SetUniformMatrix4fv;

	// -------------------------------Matrix_----------
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_WorldView) ].m_value = &m_worldViewMatrix;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_WorldView) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_WorldView) ].m_callback = &CDriver::SetUniformMatrix4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_ViewProjection) ].m_value = &m_viewProjectionMatrix;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_ViewProjection) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_ViewProjection) ].m_callback = &CDriver::SetUniformMatrix4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_WorldViewProjection) ].m_value = &m_worldViewProjectionMatrix;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_WorldViewProjection) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_WorldViewProjection) ].m_callback = &CDriver::SetUniformMatrix4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_Normal) ].m_value = &m_normalMatrix;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_Normal) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_Normal) ].m_callback = &CDriver::SetUniformMatrix3fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_BoneArray) ].m_value = &m_boneArrayMatrix;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_BoneArray) ].m_count = k_Animation_Bones_Max;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_BoneArray) ].m_callback = &CDriver::SetUniformMatrix4fv;

	// -----------------------------------------
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_Tex_0) ].m_value = nullptr;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_Tex_0) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_Tex_0) ].m_callback = &CDriver::SetUniformMatrix4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_Tex_1) ].m_value = nullptr;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_Tex_1) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_Tex_1) ].m_callback = &CDriver::SetUniformMatrix4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_Tex_2) ].m_value = nullptr;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_Tex_2) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_Tex_2) ].m_callback = &CDriver::SetUniformMatrix4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_Tex_3) ].m_value = nullptr;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_Tex_3) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_Tex_3) ].m_callback = &CDriver::SetUniformMatrix4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_Tex_4) ].m_value = nullptr;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_Tex_4) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Matrix_Tex_4) ].m_callback = &CDriver::SetUniformMatrix4fv;
	
	// -----------------------------------------

	m_uniformInfo[ Value(CShader::UniformIndex::Eye_Position) ].m_value = &m_eyePosition;
	m_uniformInfo[ Value(CShader::UniformIndex::Eye_Position) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Eye_Position) ].m_callback = &CDriver::SetUniform3fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Eye_Direction) ].m_value = &m_eyeDirection;
	m_uniformInfo[ Value(CShader::UniformIndex::Eye_Direction) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Eye_Direction) ].m_callback = &CDriver::SetUniform3fv;

	// -----------------------------------------
	static u32 texSelect[ k_Texture_Channels_Count ] = 
	{
		Value(TextureChannel::Texture_0), 
		Value(TextureChannel::Texture_1),
		Value(TextureChannel::Texture_2),
		Value(TextureChannel::Texture_3),
		Value(TextureChannel::Texture_4),
	};

	m_uniformInfo[ Value(CShader::UniformIndex::Sampler_Tex_0) ].m_value = &texSelect[ 0 ];
	m_uniformInfo[ Value(CShader::UniformIndex::Sampler_Tex_0) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Sampler_Tex_0) ].m_callback = &CDriver::SetUniform1i;

	m_uniformInfo[ Value(CShader::UniformIndex::Sampler_Tex_1) ].m_value = &texSelect[ 1 ];
	m_uniformInfo[ Value(CShader::UniformIndex::Sampler_Tex_1) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Sampler_Tex_1) ].m_callback = &CDriver::SetUniform1i;

	m_uniformInfo[ Value(CShader::UniformIndex::Sampler_Tex_2) ].m_value = &texSelect[ 2 ];
	m_uniformInfo[ Value(CShader::UniformIndex::Sampler_Tex_2) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Sampler_Tex_2) ].m_callback = &CDriver::SetUniform1i;

	m_uniformInfo[ Value(CShader::UniformIndex::Sampler_Tex_3) ].m_value = &texSelect[ 3 ];
	m_uniformInfo[ Value(CShader::UniformIndex::Sampler_Tex_3) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Sampler_Tex_3) ].m_callback = &CDriver::SetUniform1i;

	m_uniformInfo[ Value(CShader::UniformIndex::Sampler_Tex_4) ].m_value = &texSelect[ 4 ];
	m_uniformInfo[ Value(CShader::UniformIndex::Sampler_Tex_4) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Sampler_Tex_4) ].m_callback = &CDriver::SetUniform1i;

	m_uniformInfo[ Value(CShader::UniformIndex::Sampler_Cube) ].m_value = &texSelect[ 4 ];
	m_uniformInfo[ Value(CShader::UniformIndex::Sampler_Cube) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Sampler_Cube) ].m_callback = &CDriver::SetUniform1i;

	m_uniformInfo[ Value(CShader::UniformIndex::Fog_Color) ].m_value = &m_fogColor;
	m_uniformInfo[ Value(CShader::UniformIndex::Fog_Color) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Fog_Color) ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Fog_Density) ].m_value = &m_fogDensity;
	m_uniformInfo[ Value(CShader::UniformIndex::Fog_Density) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Fog_Density) ].m_callback = &CDriver::SetUniform1f;

	m_uniformInfo[ Value(CShader::UniformIndex::Fog_Select) ].m_value = &m_fogMode;
	m_uniformInfo[ Value(CShader::UniformIndex::Fog_Select) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Fog_Select) ].m_callback = &CDriver::SetUniform1i;

	m_uniformInfo[ Value(CShader::UniformIndex::Fog_Start) ].m_value = &m_fogStart;
	m_uniformInfo[ Value(CShader::UniformIndex::Fog_Start) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Fog_Start) ].m_callback = &CDriver::SetUniform1f;

	m_uniformInfo[ Value(CShader::UniformIndex::Fog_End) ].m_value = &m_fogEnd;
	m_uniformInfo[ Value(CShader::UniformIndex::Fog_End) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Fog_End) ].m_callback = &CDriver::SetUniform1f;

	// -----------------------------------------

	m_uniformInfo[ Value(CShader::UniformIndex::Material_Ambient) ].m_value = &m_materialAmbient;
	m_uniformInfo[ Value(CShader::UniformIndex::Material_Ambient) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Material_Ambient) ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Material_Diffuse) ].m_value = &m_materialDiffuse;
	m_uniformInfo[ Value(CShader::UniformIndex::Material_Diffuse) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Material_Diffuse) ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Material_Specular)  ].m_value = &m_materialSpecular;
	m_uniformInfo[ Value(CShader::UniformIndex::Material_Specular)  ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Material_Specular)  ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Material_Emissive)  ].m_value = &m_materialEmissive;
	m_uniformInfo[ Value(CShader::UniformIndex::Material_Emissive)  ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Material_Emissive)  ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[Value(CShader::UniformIndex::Material_Reflective)].m_value = &m_materialReflective;
	m_uniformInfo[Value(CShader::UniformIndex::Material_Reflective)].m_count = 1;
	m_uniformInfo[Value(CShader::UniformIndex::Material_Reflective)].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Material_Shininess) ].m_value = &m_materialShininess;
	m_uniformInfo[ Value(CShader::UniformIndex::Material_Shininess) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Material_Shininess) ].m_callback = &CDriver::SetUniform1f;

	m_uniformInfo[Value(CShader::UniformIndex::Material_Refraction)].m_value = &m_materialRefraction;
	m_uniformInfo[Value(CShader::UniformIndex::Material_Refraction)].m_count = 1;
	m_uniformInfo[Value(CShader::UniformIndex::Material_Refraction)].m_callback = &CDriver::SetUniform1f;

	// -----------------------------------------

	m_uniformInfo[ Value(CShader::UniformIndex::Light_Enable_0) ].m_value = &m_lightParameters[Value(LightChannel::Light_0)].m_isEnabled;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Enable_0) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Enable_0) ].m_callback = &CDriver::SetUniform1i;

	m_uniformInfo[ Value(CShader::UniformIndex::Light_Position_0) ].m_value = &m_lightParameters[Value(LightChannel::Light_0)].m_position;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Position_0) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Position_0) ].m_callback = &CDriver::SetUniform3fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Light_Direction_0) ].m_value = &m_lightParameters[Value(LightChannel::Light_0)].m_direction;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Direction_0) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Direction_0) ].m_callback = &CDriver::SetUniform3fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Light_Ambient_0) ].m_value = &m_lightParameters[Value(LightChannel::Light_0)].m_ambient;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Ambient_0) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Ambient_0) ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Light_Diffuse_0) ].m_value = &m_lightParameters[Value(LightChannel::Light_0)].m_diffuse;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Diffuse_0) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Diffuse_0) ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Light_Specular_0) ].m_value = &m_lightParameters[Value(LightChannel::Light_0)].m_specular;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Specular_0) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Specular_0) ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Light_Attenuation_0) ].m_value = &m_lightParameters[Value(LightChannel::Light_0)].m_attenuation;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Attenuation_0) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Attenuation_0) ].m_callback = &CDriver::SetUniform1f;

    m_uniformInfo[Value(CShader::UniformIndex::Light_FallOffAngle_0)].m_value = &m_lightParameters[Value(LightChannel::Light_0)].m_fallOffAngle;
    m_uniformInfo[Value(CShader::UniformIndex::Light_FallOffAngle_0)].m_count = 1;
    m_uniformInfo[Value(CShader::UniformIndex::Light_FallOffAngle_0)].m_callback = &CDriver::SetUniform1f;

    m_uniformInfo[Value(CShader::UniformIndex::Light_FallOffExp_0)].m_value = &m_lightParameters[Value(LightChannel::Light_0)].m_fallOffExponent;
    m_uniformInfo[Value(CShader::UniformIndex::Light_FallOffExp_0)].m_count = 1;
    m_uniformInfo[Value(CShader::UniformIndex::Light_FallOffExp_0)].m_callback = &CDriver::SetUniform1f;

	m_uniformInfo[ Value(CShader::UniformIndex::Light_Enable_1) ].m_value = &m_lightParameters[Value(LightChannel::Light_1)].m_isEnabled;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Enable_1) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Enable_1) ].m_callback = &CDriver::SetUniform1i;

	m_uniformInfo[ Value(CShader::UniformIndex::Light_Position_1) ].m_value = &m_lightParameters[Value(LightChannel::Light_1)].m_position;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Position_1) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Position_1) ].m_callback = &CDriver::SetUniform3fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Light_Direction_1) ].m_value = &m_lightParameters[Value(LightChannel::Light_1)].m_direction;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Direction_1) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Direction_1) ].m_callback = &CDriver::SetUniform3fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Light_Ambient_1) ].m_value = &m_lightParameters[Value(LightChannel::Light_1)].m_ambient;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Ambient_1) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Ambient_1) ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Light_Diffuse_1) ].m_value = &m_lightParameters[Value(LightChannel::Light_1)].m_diffuse;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Diffuse_1) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Diffuse_1) ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Light_Specular_1) ].m_value = &m_lightParameters[Value(LightChannel::Light_1)].m_specular;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Specular_1) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Specular_1) ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Light_Attenuation_1) ].m_value = &m_lightParameters[Value(LightChannel::Light_1)].m_attenuation;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Attenuation_1) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Attenuation_1) ].m_callback = &CDriver::SetUniform1f;

    m_uniformInfo[Value(CShader::UniformIndex::Light_FallOffAngle_1)].m_value = &m_lightParameters[Value(LightChannel::Light_1)].m_fallOffAngle;
    m_uniformInfo[Value(CShader::UniformIndex::Light_FallOffAngle_1)].m_count = 1;
    m_uniformInfo[Value(CShader::UniformIndex::Light_FallOffAngle_1)].m_callback = &CDriver::SetUniform1f;

    m_uniformInfo[Value(CShader::UniformIndex::Light_FallOffExp_1)].m_value = &m_lightParameters[Value(LightChannel::Light_1)].m_fallOffExponent;
    m_uniformInfo[Value(CShader::UniformIndex::Light_FallOffExp_1)].m_count = 1;
    m_uniformInfo[Value(CShader::UniformIndex::Light_FallOffExp_1)].m_callback = &CDriver::SetUniform1f;

    m_uniformInfo[ Value(CShader::UniformIndex::Light_Enable_2) ].m_value = &m_lightParameters[Value(LightChannel::Light_2)].m_isEnabled;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Enable_2) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Enable_2) ].m_callback = &CDriver::SetUniform1i;

	m_uniformInfo[ Value(CShader::UniformIndex::Light_Position_2) ].m_value = &m_lightParameters[Value(LightChannel::Light_2)].m_position;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Position_2) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Position_2) ].m_callback = &CDriver::SetUniform3fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Light_Direction_2) ].m_value = &m_lightParameters[Value(LightChannel::Light_2)].m_direction;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Direction_2) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Direction_2) ].m_callback = &CDriver::SetUniform3fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Light_Ambient_2) ].m_value = &m_lightParameters[Value(LightChannel::Light_2)].m_ambient;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Ambient_2) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Ambient_2) ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Light_Diffuse_2) ].m_value = &m_lightParameters[Value(LightChannel::Light_2)].m_diffuse;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Diffuse_2) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Diffuse_2) ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Light_Specular_2) ].m_value = &m_lightParameters[Value(LightChannel::Light_2)].m_specular;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Specular_2) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Specular_2) ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Light_Attenuation_2) ].m_value = &m_lightParameters[Value(LightChannel::Light_2)].m_attenuation;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Attenuation_2) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Attenuation_2) ].m_callback = &CDriver::SetUniform1f;

    m_uniformInfo[Value(CShader::UniformIndex::Light_FallOffAngle_2)].m_value = &m_lightParameters[Value(LightChannel::Light_2)].m_fallOffAngle;
    m_uniformInfo[Value(CShader::UniformIndex::Light_FallOffAngle_2)].m_count = 1;
    m_uniformInfo[Value(CShader::UniformIndex::Light_FallOffAngle_2)].m_callback = &CDriver::SetUniform1f;

    m_uniformInfo[Value(CShader::UniformIndex::Light_FallOffExp_2)].m_value = &m_lightParameters[Value(LightChannel::Light_2)].m_fallOffExponent;
    m_uniformInfo[Value(CShader::UniformIndex::Light_FallOffExp_2)].m_count = 1;
    m_uniformInfo[Value(CShader::UniformIndex::Light_FallOffExp_2)].m_callback = &CDriver::SetUniform1f;

    m_uniformInfo[Value(CShader::UniformIndex::Light_Enable_3)].m_value = &m_lightParameters[Value(LightChannel::Light_3)].m_isEnabled;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Enable_3) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Enable_3) ].m_callback = &CDriver::SetUniform1i;

	m_uniformInfo[ Value(CShader::UniformIndex::Light_Position_3) ].m_value = &m_lightParameters[Value(LightChannel::Light_3)].m_position;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Position_3) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Position_3) ].m_callback = &CDriver::SetUniform3fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Light_Direction_3) ].m_value = &m_lightParameters[Value(LightChannel::Light_3)].m_direction;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Direction_3) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Direction_3) ].m_callback = &CDriver::SetUniform3fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Light_Ambient_3) ].m_value = &m_lightParameters[Value(LightChannel::Light_3)].m_ambient;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Ambient_3) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Ambient_3) ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Light_Diffuse_3) ].m_value = &m_lightParameters[Value(LightChannel::Light_3)].m_diffuse;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Diffuse_3) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Diffuse_3) ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Light_Specular_3) ].m_value = &m_lightParameters[Value(LightChannel::Light_3)].m_specular;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Specular_3) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Specular_3) ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ Value(CShader::UniformIndex::Light_Attenuation_3) ].m_value = &m_lightParameters[Value(LightChannel::Light_3)].m_attenuation;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Attenuation_3) ].m_count = 1;
	m_uniformInfo[ Value(CShader::UniformIndex::Light_Attenuation_3) ].m_callback = &CDriver::SetUniform1f;

    m_uniformInfo[Value(CShader::UniformIndex::Light_FallOffAngle_3)].m_value = &m_lightParameters[Value(LightChannel::Light_3)].m_fallOffAngle;
    m_uniformInfo[Value(CShader::UniformIndex::Light_FallOffAngle_3)].m_count = 1;
    m_uniformInfo[Value(CShader::UniformIndex::Light_FallOffAngle_3)].m_callback = &CDriver::SetUniform1f;

    m_uniformInfo[Value(CShader::UniformIndex::Light_FallOffExp_3)].m_value = &m_lightParameters[Value(LightChannel::Light_3)].m_fallOffExponent;
    m_uniformInfo[Value(CShader::UniformIndex::Light_FallOffExp_3)].m_count = 1;
    m_uniformInfo[Value(CShader::UniformIndex::Light_FallOffExp_3)].m_callback = &CDriver::SetUniform1f;

}

// ----------------------------------------------------------------------//

void CDriver::Log( char *fmt, ... )
{
#if 1
    
	va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

#endif
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
