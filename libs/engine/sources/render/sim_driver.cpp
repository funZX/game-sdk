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

#include <math/sim_math.h>

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
	m_batch2D	= SIM_NEW CBatch2D( "Driver", this );

	Matrix4StackClear( &m_worldStack );
	Matrix4StackClear( &m_viewStack );
	Matrix4StackClear( &m_projectionStack );

	for( s32 i = 0; i < k_Texture_Channels_Count; i++ ) {
		Matrix4StackClear( &m_textureStack[ i ] );
	}

	for( s32 i = 0; i < k_Skeleton_Bones_Max; i++ ) {
		Matrix4ToIdentity( &m_skeletonMatrix[ i ] );
	}

	Matrix4ToIdentity( &m_worldInverseMatrix );
	Matrix4ToIdentity( &m_worldInverseTMatrix );
	m_isWorldMatrixDirty = true;
	m_isActiveStackAlteringWorldMatrix = true;

	Matrix4ToIdentity( &m_viewInverseMatrix );
	Matrix4ToIdentity( &m_viewInverseTMatrix );
	m_isViewMatrixDirty = true;
	m_isActiveStackAlteringViewMatrix = true;

	Matrix4ToIdentity( &m_worldViewMatrix );
	m_isWorldViewMatrixDirty = true;
	m_isActiveStackAlteringWorldViewMatrix = true;

	Matrix4ToIdentity( &m_viewProjectionMatrix );
	m_isViewProjectionMatrixDirty = true;
	m_isActiveStackAlteringViewProjectionMatrix = true;

	Matrix4ToIdentity( &m_worldViewProjectionMatrix );
	m_isWorldViewProjectionMatrixDirty = true;
	m_isActiveStackAlteringWorldViewProjectionMatrix = true;

	Matrix3ToIdentity( &m_normalMatrix );
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
	Vec4ToZero( &m_fogColor );
	
	m_depthStart			= 0.0f;
	m_depthEnd				= 0.0f;

	Vec4ToZero( &m_materialAmbient );
	Vec4ToZero( &m_materialDiffuse );
	Vec4ToZero( &m_materialSpecular );

	m_timer					= 0.0f;
	m_timerSin				= 0.0f;
	m_timerCos				= 0.0f;
	m_timerRot				= 0.0f;

	m_pointSize				= 1.0f;
	Vec4ToOne( &m_color );

	m_materialReflectivity	= 0.0f;
	m_materialShininess		= 0.0f;

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

void CDriver::Tick( const f32 dt )
{
	m_timerRot += dt * 3.6f;

	while ( m_timerRot > 360.0f )
		m_timerRot -= 360.0f;

	f32 toRad	= SIM_DEG2RAD( m_timerRot );

	m_timerSin	= sinf( toRad );
	m_timerCos	= cosf( toRad );

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

	auto texChannel = EnumValue(m_textureChannel);

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
	TMatrix4 m;

	Matrix4Multiply( GetViewMatrix(), GetWorldMatrix(), &m );
	Matrix4Invert( &m, &m );
	Matrix4Transpose( &m, &m );

	Matrix4ToMatrix3( &m, &m_normalMatrix );
}

// ----------------------------------------------------------------------//

CDriver::TextureChannel CDriver::SetTextureChannel(TextureChannel texChannel)
{
	TextureChannel old = m_textureChannel;

	if(m_textureChannel == texChannel)
		return old;

	glActiveTexture( GL_TEXTURE0 + EnumValue(texChannel));

	m_textureChannel = texChannel;

	SIM_CHECK_OPENGL();

	return old;
}

// ----------------------------------------------------------------------//

void CDriver::EnableBlendFunc( const TBlendFunc* blendfunc )
{
	if ( blendfunc->equation != m_blendFunc.equation )
	{
		m_blendFunc.equation = blendfunc->equation;

		glBlendEquation( blendfunc->equation );
	}

	if ( blendfunc->src != m_blendFunc.src ||
		 blendfunc->dst != m_blendFunc.dst )
	{
		m_blendFunc.src = blendfunc->src;
		m_blendFunc.dst = blendfunc->dst;

		glBlendFunc( blendfunc->src, blendfunc->dst );
	}
}

// ----------------------------------------------------------------------//

void CDriver::EnableBlendFunc( u32 equation, u32 src, u32 dst )
{
	TBlendFunc blendfunc;

	blendfunc.equation	= equation;
	blendfunc.src		= src;
	blendfunc.dst		= dst;

	EnableBlendFunc( &blendfunc );
}

// ----------------------------------------------------------------------//

void CDriver::EnableDepthFunc(const TDepthFunc* depthfunc)
{
	if (depthfunc->equation != m_depthFunc.equation)
	{
		m_depthFunc.equation = depthfunc->equation;

		glDepthFunc(depthfunc->equation);
	}
}

// ----------------------------------------------------------------------//

void CDriver::EnableDepthFunc(u32 equation)
{
	TDepthFunc depthfunc;
	depthfunc.equation = equation;
	EnableDepthFunc(&depthfunc);
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

	switch(EnumValue(cullingMode))
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
	Matrix4ToIdentity( m_activeStack->topmatrix );
	MatrixDirty();
}

// ----------------------------------------------------------------------//

void CDriver::MatrixLoad( const TMatrix4 *mat )
{
	Matrix4Copy( m_activeStack->topmatrix, mat );
	MatrixDirty();
}

// ----------------------------------------------------------------------//

void CDriver::MatrixMultiply( const TMatrix4 *mat )
{
	Matrix4Multiply( m_activeStack->topmatrix, mat );
	MatrixDirty();
}

// ----------------------------------------------------------------------//

CDriver::MatrixMode CDriver::SetMatrixMode(MatrixMode matrixMode)
{
	MatrixMode old = m_matrixMode;

	if (m_matrixMode == matrixMode)
		return old;

	switch( EnumValue(matrixMode) )
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
			m_activeStack = &m_textureStack[ EnumValue(m_textureChannel) ];

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
	Matrix4StackPush( m_activeStack );
	MatrixDirty();
}

// ----------------------------------------------------------------------//

void CDriver::MatrixPop()
{
	Matrix4StackPop( m_activeStack );
	MatrixDirty();
}

// ----------------------------------------------------------------------//

void CDriver::MatrixTranslate( const TVec3 *pos )
{
	Matrix4Translate( m_activeStack->topmatrix, pos );
	MatrixDirty();
}

// ----------------------------------------------------------------------//

void CDriver::MatrixTranslate( const f32 x, const f32 y, const f32 z )
{
	Matrix4Translate( m_activeStack->topmatrix, x, y, z );
	MatrixDirty();
}

void CDriver::MatrixTranslateX( const f32 x )
{
	Matrix4TranslateX( m_activeStack->topmatrix, x );
	MatrixDirty();
}

// ----------------------------------------------------------------------//

void CDriver::MatrixTranslateY( const f32 y )
{
	Matrix4TranslateY( m_activeStack->topmatrix,y );
	MatrixDirty();
}

// ----------------------------------------------------------------------//

void CDriver::MatrixTranslateZ( const f32 z )
{
	Matrix4TranslateZ( m_activeStack->topmatrix, z );
	MatrixDirty();
}

// ----------------------------------------------------------------------//

void CDriver::MatrixRotate( const f32 angle, const TVec3 *rot )
{
	Matrix4Rotate( m_activeStack->topmatrix, angle, rot );
	MatrixDirty();
}

// ----------------------------------------------------------------------//

void CDriver::MatrixRotate( const f32 angle, const f32 x, const f32 y, const f32 z )
{
	Matrix4Rotate( m_activeStack->topmatrix, angle, x, y, z );
	MatrixDirty();
}

// ----------------------------------------------------------------------//

void CDriver::MatrixRotateX( const f32 angle )
{
	Matrix4RotateX( m_activeStack->topmatrix, angle );
	MatrixDirty();
}

// ----------------------------------------------------------------------//

void CDriver::MatrixRotateY( const f32 angle )
{
	Matrix4RotateY( m_activeStack->topmatrix, angle );
	MatrixDirty();
}

void CDriver::MatrixRotateZ( const f32 angle )
{
	Matrix4RotateZ( m_activeStack->topmatrix, angle );
	MatrixDirty();
}

// ----------------------------------------------------------------------//

void CDriver::MatrixScale( const TVec3 *scl )
{
	Matrix4Scale( m_activeStack->topmatrix, scl );
	MatrixDirty();
}

// ----------------------------------------------------------------------//

void CDriver::MatrixScale( const f32 x, const f32 y, const f32 z )
{
	Matrix4Scale( m_activeStack->topmatrix, x, y, z );
	MatrixDirty();
}

// ----------------------------------------------------------------------//

void CDriver::MatrixScaleX( const f32 scale )
{
	Matrix4ScaleX( m_activeStack->topmatrix, scale );
	MatrixDirty();
}

// ----------------------------------------------------------------------//

void CDriver::MatrixScaleY( const f32 scale )
{
	Matrix4ScaleY( m_activeStack->topmatrix, scale );
	MatrixDirty();
}

// ----------------------------------------------------------------------//

void CDriver::MatrixScaleZ( const f32 scale )
{
	Matrix4ScaleZ( m_activeStack->topmatrix, scale );
	MatrixDirty();
}

// ----------------------------------------------------------------------//

void CDriver::Clear( const TVec4* color )
{
	m_crtVertexSource	= 0;
	m_crtRenderTexture	= 0;

	glClearColor( color->x, color->y, color->z, color->w );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

// ----------------------------------------------------------------------//

void CDriver::ClearColor( const TVec4* color )
{
	glClearColor( color->x, color->y, color->z, color->w );
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

void CDriver::SetColor( const TVec4 *col )
{
	Vec4Copy( &m_color, col );
}

// ----------------------------------------------------------------------//

void CDriver::SetFogColor( const TVec4 *col )
{
	Vec4Copy( &m_fogColor, col );
}

// ----------------------------------------------------------------------//

void CDriver::SetFogMode( FogMode fogMode )
{
	m_fogMode	= fogMode;
}

// ----------------------------------------------------------------------//

void CDriver::SetFogStart(  f32 fogStart )
{
	m_fogStart	= fogStart;
}

// ----------------------------------------------------------------------//

void CDriver::SetFogEnd( f32 fogEnd )
{
	m_fogEnd	= fogEnd;
}

// ----------------------------------------------------------------------//

void CDriver::SetFogDensity( f32 d )
{
	m_fogDensity = d;
}

// ----------------------------------------------------------------------//

void CDriver::SetLightPosition( const TVec3 *pos )
{
	Vec3Copy( &m_lightParameters[ EnumValue(m_lightChannel) ].m_position, pos );
}

// ----------------------------------------------------------------------//

void CDriver::SetLightDirection( const TVec3 *dir )
{
	Vec3Copy( &m_lightParameters[EnumValue(m_lightChannel)].m_direction, dir );
}

// ----------------------------------------------------------------------//

void CDriver::SetLightAmbient( const TVec4 *col )
{
	Vec4Copy( &m_lightParameters[EnumValue(m_lightChannel)].m_ambient, col );
}

// ----------------------------------------------------------------------//

void CDriver::SetLightDiffuse( const TVec4 *col )
{
	Vec4Copy( &m_lightParameters[EnumValue(m_lightChannel)].m_diffuse, col );
}

// ----------------------------------------------------------------------//

void CDriver::SetLightSpecular( const TVec4 *col )
{
	Vec4Copy( &m_lightParameters[EnumValue(m_lightChannel)].m_specular, col );
}

// ----------------------------------------------------------------------//

void CDriver::SetLightIntensity( f32 intens )
{
	m_lightParameters[EnumValue(m_lightChannel)].m_intensity = intens;
}

// ----------------------------------------------------------------------//

void CDriver::SetVertexAttribute( CShader::TAttrib* attrib, void *vertexData, 
	CVertexSource::AttributeStride vertexStride )
{
	TVertexAttributeInfo *attribInfo = &m_vertexAttributeInfo[ attrib->m_ID ];

    if( vertexData != attribInfo->m_vertexData )
    {
		glVertexAttribPointer( 
			attrib->m_location,
			EnumValue(attrib->m_compSize),
			EnumValue(attrib->m_compType),
			GL_FALSE,
			EnumValue(vertexStride),
			vertexData 
			);

        attribInfo->m_vertexData = vertexData;
    }

	SIM_CHECK_OPENGL();
}

// ----------------------------------------------------------------------//

void CDriver::EnableVertexAttribute( CShader::TAttrib* attrib )
{
	TVertexAttributeInfo *attribInfo = &m_vertexAttributeInfo[ attrib->m_ID ];

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
	TVertexAttributeInfo *attribInfo = &m_vertexAttributeInfo[ attrib->m_ID ];

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
	m_uniformInfo[ CShader::k_Uniform_Matrix_World ].m_value		= (void*) GetWorldMatrix();
	m_uniformInfo[ CShader::k_Uniform_Matrix_View ].m_value			= (void*) GetViewMatrix();
	m_uniformInfo[ CShader::k_Uniform_Matrix_Projection ].m_value	= (void*) GetProjectionMatrix();

	m_uniformInfo[ CShader::k_Uniform_Matrix_Tex_0 ].m_value		= (void*) GetTextureMatrix( TextureChannel::Texture_0 );
	m_uniformInfo[ CShader::k_Uniform_Matrix_Tex_1 ].m_value		= (void*) GetTextureMatrix( TextureChannel::Texture_1 );
	m_uniformInfo[ CShader::k_Uniform_Matrix_Tex_2 ].m_value		= (void*) GetTextureMatrix( TextureChannel::Texture_2 );
	m_uniformInfo[ CShader::k_Uniform_Matrix_Tex_3 ].m_value		= (void*) GetTextureMatrix( TextureChannel::Texture_3 );
	m_uniformInfo[ CShader::k_Uniform_Matrix_Tex_4 ].m_value		= (void*) GetTextureMatrix( TextureChannel::Texture_4 );

	if( m_isWorldMatrixDirty )
	{
		if( effect->m_isUsingWorldInverseMatrix )
			Matrix4Invert( GetWorldMatrix(), &m_worldInverseMatrix );

		if( effect->m_isUsingWorldInverseTMatrix )
			Matrix4Transpose( &m_worldInverseMatrix, &m_worldInverseTMatrix );

		m_isWorldMatrixDirty = false;
	}

	if( m_isViewMatrixDirty )
	{
		if ( effect->m_isUsingViewInverseMatrix )
			Matrix4Invert( GetViewMatrix(), &m_viewInverseMatrix );

		if( effect->m_isUsingViewInverseTMatrix )
			Matrix4Transpose( &m_viewInverseMatrix, &m_viewInverseTMatrix );		
		
		m_isViewMatrixDirty = false;
	}

	if( m_isWorldViewMatrixDirty )
	{
		if ( effect->m_isUsingWorldViewMatrix )
			Matrix4Multiply( GetViewMatrix(), GetWorldMatrix(), &m_worldViewMatrix );

		m_isWorldViewMatrixDirty = false;
	}

	if( m_isViewProjectionMatrixDirty )
	{
		if ( effect->m_isUsingViewProjectionMatrix )
			Matrix4Multiply( GetProjectionMatrix(), GetViewMatrix(), &m_viewProjectionMatrix );

		m_isViewProjectionMatrixDirty = false;
	}

	if( m_isWorldViewProjectionMatrixDirty )
	{
		if ( effect->m_isUsingWorldViewProjectionMatrix )
		{
			static TMatrix4 m;

			Matrix4Multiply( GetProjectionMatrix(), GetViewMatrix(), &m );
			Matrix4Multiply( &m, GetWorldMatrix(), &m_worldViewProjectionMatrix );
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
	TUniformInfo *uniInfo		= &m_uniformInfo[ uni->m_ID ];
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
		u32 vboStride = EnumValue(vertexSource->GetVertexStride());
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
		static const Primitive primitives[] = {
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

		u32 rt = EnumValue(primitives[EnumValue(vertexSource->GetType())]);
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
	m_uniformInfo[ CShader::k_Uniform_Time ].m_value = &m_timer;
	m_uniformInfo[ CShader::k_Uniform_Time ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Time ].m_callback = &CDriver::SetUniform1f;

	m_uniformInfo[ CShader::k_Uniform_Color ].m_value = &m_color;
	m_uniformInfo[ CShader::k_Uniform_Color ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Color ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ CShader::k_Uniform_Point_Size ].m_value = &m_pointSize;
	m_uniformInfo[ CShader::k_Uniform_Point_Size ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Point_Size ].m_callback = &CDriver::SetUniform1f;

	// -----------------------------------------
	m_uniformInfo[ CShader::k_Uniform_Matrix_World ].m_value = nullptr;
	m_uniformInfo[ CShader::k_Uniform_Matrix_World ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Matrix_World ].m_callback = &CDriver::SetUniformMatrix4fv;

	m_uniformInfo[ CShader::k_Uniform_Matrix_World_Inverse ].m_value = &m_worldInverseMatrix;
	m_uniformInfo[ CShader::k_Uniform_Matrix_World_Inverse ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Matrix_World_Inverse ].m_callback = &CDriver::SetUniformMatrix4fv;

	m_uniformInfo[ CShader::k_Uniform_Matrix_World_InverseT ].m_value = &m_worldInverseTMatrix;
	m_uniformInfo[ CShader::k_Uniform_Matrix_World_InverseT ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Matrix_World_InverseT ].m_callback = &CDriver::SetUniformMatrix4fv;

	m_uniformInfo[ CShader::k_Uniform_Matrix_View ].m_value = nullptr;
	m_uniformInfo[ CShader::k_Uniform_Matrix_View ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Matrix_View ].m_callback = &CDriver::SetUniformMatrix4fv;

	m_uniformInfo[ CShader::k_Uniform_Matrix_View_Inverse ].m_value = &m_viewInverseMatrix;
	m_uniformInfo[ CShader::k_Uniform_Matrix_View_Inverse ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Matrix_View_Inverse ].m_callback = &CDriver::SetUniformMatrix4fv;

	m_uniformInfo[ CShader::k_Uniform_Matrix_View_InverseT ].m_value = &m_viewInverseTMatrix;
	m_uniformInfo[ CShader::k_Uniform_Matrix_View_InverseT ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Matrix_View_InverseT ].m_callback = &CDriver::SetUniformMatrix4fv;

	m_uniformInfo[ CShader::k_Uniform_Matrix_Projection ].m_value = nullptr;
	m_uniformInfo[ CShader::k_Uniform_Matrix_Projection ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Matrix_Projection ].m_callback = &CDriver::SetUniformMatrix4fv;

	// -------------------------------Matrix_----------
	m_uniformInfo[ CShader::k_Uniform_Matrix_WorldView ].m_value = &m_worldViewMatrix;
	m_uniformInfo[ CShader::k_Uniform_Matrix_WorldView ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Matrix_WorldView ].m_callback = &CDriver::SetUniformMatrix4fv;

	m_uniformInfo[ CShader::k_Uniform_Matrix_ViewProjection ].m_value = &m_viewProjectionMatrix;
	m_uniformInfo[ CShader::k_Uniform_Matrix_ViewProjection ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Matrix_ViewProjection ].m_callback = &CDriver::SetUniformMatrix4fv;

	m_uniformInfo[ CShader::k_Uniform_Matrix_WorldViewProjection ].m_value = &m_worldViewProjectionMatrix;
	m_uniformInfo[ CShader::k_Uniform_Matrix_WorldViewProjection ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Matrix_WorldViewProjection ].m_callback = &CDriver::SetUniformMatrix4fv;

	m_uniformInfo[ CShader::k_Uniform_Matrix_Normal ].m_value = &m_normalMatrix;
	m_uniformInfo[ CShader::k_Uniform_Matrix_Normal ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Matrix_Normal ].m_callback = &CDriver::SetUniformMatrix3fv;

	m_uniformInfo[ CShader::k_Uniform_Matrix_Skeleton ].m_value = &m_skeletonMatrix;
	m_uniformInfo[ CShader::k_Uniform_Matrix_Skeleton ].m_count = k_Skeleton_Bones_Max;
	m_uniformInfo[ CShader::k_Uniform_Matrix_Skeleton ].m_callback = &CDriver::SetUniformMatrix4fv;

	// -----------------------------------------
	m_uniformInfo[ CShader::k_Uniform_Matrix_Tex_0 ].m_value = nullptr;
	m_uniformInfo[ CShader::k_Uniform_Matrix_Tex_0 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Matrix_Tex_0 ].m_callback = &CDriver::SetUniformMatrix4fv;

	m_uniformInfo[ CShader::k_Uniform_Matrix_Tex_1 ].m_value = nullptr;
	m_uniformInfo[ CShader::k_Uniform_Matrix_Tex_1 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Matrix_Tex_1 ].m_callback = &CDriver::SetUniformMatrix4fv;

	m_uniformInfo[ CShader::k_Uniform_Matrix_Tex_2 ].m_value = nullptr;
	m_uniformInfo[ CShader::k_Uniform_Matrix_Tex_2 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Matrix_Tex_2 ].m_callback = &CDriver::SetUniformMatrix4fv;

	m_uniformInfo[ CShader::k_Uniform_Matrix_Tex_3 ].m_value = nullptr;
	m_uniformInfo[ CShader::k_Uniform_Matrix_Tex_3 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Matrix_Tex_3 ].m_callback = &CDriver::SetUniformMatrix4fv;

	m_uniformInfo[ CShader::k_Uniform_Matrix_Tex_4 ].m_value = nullptr;
	m_uniformInfo[ CShader::k_Uniform_Matrix_Tex_4 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Matrix_Tex_4 ].m_callback = &CDriver::SetUniformMatrix4fv;
	
	// -----------------------------------------

	m_uniformInfo[ CShader::k_Uniform_Eye_Position ].m_value = &m_eyePosition;
	m_uniformInfo[ CShader::k_Uniform_Eye_Position ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Eye_Position ].m_callback = &CDriver::SetUniform3fv;

	m_uniformInfo[ CShader::k_Uniform_Eye_Direction ].m_value = &m_eyeDirection;
	m_uniformInfo[ CShader::k_Uniform_Eye_Direction ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Eye_Direction ].m_callback = &CDriver::SetUniform3fv;

	// -----------------------------------------
	static u32 texSelect[ k_Texture_Channels_Count ] = 
	{
		EnumValue(TextureChannel::Texture_0), 
		EnumValue(TextureChannel::Texture_1),
		EnumValue(TextureChannel::Texture_2),
		EnumValue(TextureChannel::Texture_3),
		EnumValue(TextureChannel::Texture_4),
	};

	m_uniformInfo[ CShader::k_Uniform_Sampler_Tex_0 ].m_value = &texSelect[ 0 ];
	m_uniformInfo[ CShader::k_Uniform_Sampler_Tex_0 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Sampler_Tex_0 ].m_callback = &CDriver::SetUniform1i;

	m_uniformInfo[ CShader::k_Uniform_Sampler_Tex_1 ].m_value = &texSelect[ 1 ];
	m_uniformInfo[ CShader::k_Uniform_Sampler_Tex_1 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Sampler_Tex_1 ].m_callback = &CDriver::SetUniform1i;

	m_uniformInfo[ CShader::k_Uniform_Sampler_Tex_2 ].m_value = &texSelect[ 2 ];
	m_uniformInfo[ CShader::k_Uniform_Sampler_Tex_2 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Sampler_Tex_2 ].m_callback = &CDriver::SetUniform1i;

	m_uniformInfo[ CShader::k_Uniform_Sampler_Tex_3 ].m_value = &texSelect[ 3 ];
	m_uniformInfo[ CShader::k_Uniform_Sampler_Tex_3 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Sampler_Tex_3 ].m_callback = &CDriver::SetUniform1i;

	m_uniformInfo[ CShader::k_Uniform_Sampler_Tex_4 ].m_value = &texSelect[ 4 ];
	m_uniformInfo[ CShader::k_Uniform_Sampler_Tex_4 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Sampler_Tex_4 ].m_callback = &CDriver::SetUniform1i;

	m_uniformInfo[ CShader::k_Uniform_Sampler_Cube ].m_value = &texSelect[ 4 ];
	m_uniformInfo[ CShader::k_Uniform_Sampler_Cube ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Sampler_Cube ].m_callback = &CDriver::SetUniform1i;

	m_uniformInfo[ CShader::k_Uniform_Fog_Color ].m_value = &m_fogColor;
	m_uniformInfo[ CShader::k_Uniform_Fog_Color ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Fog_Color ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ CShader::k_Uniform_Fog_Density ].m_value = &m_fogDensity;
	m_uniformInfo[ CShader::k_Uniform_Fog_Density ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Fog_Density ].m_callback = &CDriver::SetUniform1f;

	m_uniformInfo[ CShader::k_Uniform_Fog_Select ].m_value = &m_fogMode;
	m_uniformInfo[ CShader::k_Uniform_Fog_Select ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Fog_Select ].m_callback = &CDriver::SetUniform1i;

	m_uniformInfo[ CShader::k_Uniform_Fog_Start ].m_value = &m_fogStart;
	m_uniformInfo[ CShader::k_Uniform_Fog_Start ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Fog_Start ].m_callback = &CDriver::SetUniform1f;

	m_uniformInfo[ CShader::k_Uniform_Fog_End ].m_value = &m_fogEnd;
	m_uniformInfo[ CShader::k_Uniform_Fog_End ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Fog_End ].m_callback = &CDriver::SetUniform1f;

	// -----------------------------------------

	m_uniformInfo[ CShader::k_Uniform_Material_Ambient ].m_value = &m_materialAmbient;
	m_uniformInfo[ CShader::k_Uniform_Material_Ambient ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Material_Ambient ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ CShader::k_Uniform_Material_Diffuse ].m_value = &m_materialDiffuse;
	m_uniformInfo[ CShader::k_Uniform_Material_Diffuse ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Material_Diffuse ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ CShader::k_Uniform_Material_Specular  ].m_value = &m_materialSpecular;
	m_uniformInfo[ CShader::k_Uniform_Material_Specular  ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Material_Specular  ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ CShader::k_Uniform_Material_Emissive  ].m_value = &m_materialEmissive;
	m_uniformInfo[ CShader::k_Uniform_Material_Emissive  ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Material_Emissive  ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ CShader::k_Uniform_Material_Shininess ].m_value = &m_materialShininess;
	m_uniformInfo[ CShader::k_Uniform_Material_Shininess ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Material_Shininess ].m_callback = &CDriver::SetUniform1f;

	m_uniformInfo[ CShader::k_Uniform_Material_Reflectivity ].m_value = &m_materialReflectivity;
	m_uniformInfo[ CShader::k_Uniform_Material_Reflectivity ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Material_Reflectivity ].m_callback = &CDriver::SetUniform1f;

	// -----------------------------------------

	m_uniformInfo[ CShader::k_Uniform_Light_Enable_0 ].m_value = &m_lightParameters[EnumValue(LightChannel::Light_0)].m_isEnabled;
	m_uniformInfo[ CShader::k_Uniform_Light_Enable_0 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Light_Enable_0 ].m_callback = &CDriver::SetUniform1i;

	m_uniformInfo[ CShader::k_Uniform_Light_Position_0 ].m_value = &m_lightParameters[EnumValue(LightChannel::Light_0)].m_position;
	m_uniformInfo[ CShader::k_Uniform_Light_Position_0 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Light_Position_0 ].m_callback = &CDriver::SetUniform3fv;

	m_uniformInfo[ CShader::k_Uniform_Light_Direction_0 ].m_value = &m_lightParameters[EnumValue(LightChannel::Light_0)].m_direction;
	m_uniformInfo[ CShader::k_Uniform_Light_Direction_0 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Light_Direction_0 ].m_callback = &CDriver::SetUniform3fv;

	m_uniformInfo[ CShader::k_Uniform_Light_Ambient_0 ].m_value = &m_lightParameters[EnumValue(LightChannel::Light_0)].m_ambient;
	m_uniformInfo[ CShader::k_Uniform_Light_Ambient_0 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Light_Ambient_0 ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ CShader::k_Uniform_Light_Diffuse_0 ].m_value = &m_lightParameters[EnumValue(LightChannel::Light_0)].m_diffuse;
	m_uniformInfo[ CShader::k_Uniform_Light_Diffuse_0 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Light_Diffuse_0 ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ CShader::k_Uniform_Light_Specular_0 ].m_value = &m_lightParameters[EnumValue(LightChannel::Light_0)].m_specular;
	m_uniformInfo[ CShader::k_Uniform_Light_Specular_0 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Light_Specular_0 ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ CShader::k_Uniform_Light_Intensity_0 ].m_value = &m_lightParameters[EnumValue(LightChannel::Light_0)].m_intensity;
	m_uniformInfo[ CShader::k_Uniform_Light_Intensity_0 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Light_Intensity_0 ].m_callback = &CDriver::SetUniform1f;

	m_uniformInfo[ CShader::k_Uniform_Light_Enable_1 ].m_value = &m_lightParameters[EnumValue(LightChannel::Light_1)].m_isEnabled;
	m_uniformInfo[ CShader::k_Uniform_Light_Enable_1 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Light_Enable_1 ].m_callback = &CDriver::SetUniform1i;

	m_uniformInfo[ CShader::k_Uniform_Light_Position_1 ].m_value = &m_lightParameters[EnumValue(LightChannel::Light_1)].m_position;
	m_uniformInfo[ CShader::k_Uniform_Light_Position_1 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Light_Position_1 ].m_callback = &CDriver::SetUniform3fv;

	m_uniformInfo[ CShader::k_Uniform_Light_Direction_1 ].m_value = &m_lightParameters[EnumValue(LightChannel::Light_1)].m_direction;
	m_uniformInfo[ CShader::k_Uniform_Light_Direction_1 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Light_Direction_1 ].m_callback = &CDriver::SetUniform3fv;

	m_uniformInfo[ CShader::k_Uniform_Light_Ambient_1 ].m_value = &m_lightParameters[EnumValue(LightChannel::Light_1)].m_ambient;
	m_uniformInfo[ CShader::k_Uniform_Light_Ambient_1 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Light_Ambient_1 ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ CShader::k_Uniform_Light_Diffuse_1 ].m_value = &m_lightParameters[EnumValue(LightChannel::Light_1)].m_diffuse;
	m_uniformInfo[ CShader::k_Uniform_Light_Diffuse_1 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Light_Diffuse_1 ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ CShader::k_Uniform_Light_Specular_1 ].m_value = &m_lightParameters[EnumValue(LightChannel::Light_1)].m_specular;
	m_uniformInfo[ CShader::k_Uniform_Light_Specular_1 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Light_Specular_1 ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ CShader::k_Uniform_Light_Intensity_1 ].m_value = &m_lightParameters[EnumValue(LightChannel::Light_1)].m_intensity;
	m_uniformInfo[ CShader::k_Uniform_Light_Intensity_1 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Light_Intensity_1 ].m_callback = &CDriver::SetUniform1f;

	m_uniformInfo[ CShader::k_Uniform_Light_Enable_2 ].m_value = &m_lightParameters[EnumValue(LightChannel::Light_2)].m_isEnabled;
	m_uniformInfo[ CShader::k_Uniform_Light_Enable_2 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Light_Enable_2 ].m_callback = &CDriver::SetUniform1i;

	m_uniformInfo[ CShader::k_Uniform_Light_Position_2 ].m_value = &m_lightParameters[EnumValue(LightChannel::Light_2)].m_position;
	m_uniformInfo[ CShader::k_Uniform_Light_Position_2 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Light_Position_2 ].m_callback = &CDriver::SetUniform3fv;

	m_uniformInfo[ CShader::k_Uniform_Light_Direction_2 ].m_value = &m_lightParameters[EnumValue(LightChannel::Light_2)].m_direction;
	m_uniformInfo[ CShader::k_Uniform_Light_Direction_2 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Light_Direction_2 ].m_callback = &CDriver::SetUniform3fv;

	m_uniformInfo[ CShader::k_Uniform_Light_Ambient_2 ].m_value = &m_lightParameters[EnumValue(LightChannel::Light_2)].m_ambient;
	m_uniformInfo[ CShader::k_Uniform_Light_Ambient_2 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Light_Ambient_2 ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ CShader::k_Uniform_Light_Diffuse_2 ].m_value = &m_lightParameters[EnumValue(LightChannel::Light_2)].m_diffuse;
	m_uniformInfo[ CShader::k_Uniform_Light_Diffuse_2 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Light_Diffuse_2 ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ CShader::k_Uniform_Light_Specular_2 ].m_value = &m_lightParameters[EnumValue(LightChannel::Light_2)].m_specular;
	m_uniformInfo[ CShader::k_Uniform_Light_Specular_2 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Light_Specular_2 ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ CShader::k_Uniform_Light_Intensity_2 ].m_value = &m_lightParameters[EnumValue(LightChannel::Light_2)].m_intensity;
	m_uniformInfo[ CShader::k_Uniform_Light_Intensity_2 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Light_Intensity_2 ].m_callback = &CDriver::SetUniform1f;

	m_uniformInfo[ CShader::k_Uniform_Light_Enable_3 ].m_value = &m_lightParameters[EnumValue(LightChannel::Light_3)].m_isEnabled;
	m_uniformInfo[ CShader::k_Uniform_Light_Enable_3 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Light_Enable_3 ].m_callback = &CDriver::SetUniform1i;

	m_uniformInfo[ CShader::k_Uniform_Light_Position_3 ].m_value = &m_lightParameters[EnumValue(LightChannel::Light_3)].m_position;
	m_uniformInfo[ CShader::k_Uniform_Light_Position_3 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Light_Position_3 ].m_callback = &CDriver::SetUniform3fv;

	m_uniformInfo[ CShader::k_Uniform_Light_Direction_3 ].m_value = &m_lightParameters[EnumValue(LightChannel::Light_3)].m_direction;
	m_uniformInfo[ CShader::k_Uniform_Light_Direction_3 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Light_Direction_3 ].m_callback = &CDriver::SetUniform3fv;

	m_uniformInfo[ CShader::k_Uniform_Light_Ambient_3 ].m_value = &m_lightParameters[EnumValue(LightChannel::Light_3)].m_ambient;
	m_uniformInfo[ CShader::k_Uniform_Light_Ambient_3 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Light_Ambient_3 ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ CShader::k_Uniform_Light_Diffuse_3 ].m_value = &m_lightParameters[EnumValue(LightChannel::Light_3)].m_diffuse;
	m_uniformInfo[ CShader::k_Uniform_Light_Diffuse_3 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Light_Diffuse_3 ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ CShader::k_Uniform_Light_Specular_3 ].m_value = &m_lightParameters[EnumValue(LightChannel::Light_3)].m_specular;
	m_uniformInfo[ CShader::k_Uniform_Light_Specular_3 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Light_Specular_3 ].m_callback = &CDriver::SetUniform4fv;

	m_uniformInfo[ CShader::k_Uniform_Light_Intensity_3 ].m_value = &m_lightParameters[EnumValue(LightChannel::Light_3)].m_intensity;
	m_uniformInfo[ CShader::k_Uniform_Light_Intensity_3 ].m_count = 1;
	m_uniformInfo[ CShader::k_Uniform_Light_Intensity_3 ].m_callback = &CDriver::SetUniform1f;
}

// ----------------------------------------------------------------------//

void CDriver::Log( const char *fmt, ... )
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
