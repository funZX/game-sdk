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

#ifndef __SIM_RENDERER_H
#define __SIM_RENDERER_H

#include <core/sim_core.h>

#include <render/sim_vertex_source.h>
#include <render/sim_vertex_group.h>
#include <render/sim_render_texture.h>
#include <render/sim_shader.h>
#include <core/sim_singleton.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

class CEffect;

class CDriver : public stl::CSingleton<CDriver>
{
public:
	CDriver();
	virtual ~CDriver( );

	// ------------------------------------------------------------------//
	enum class MatrixMode : u32
	{
		World,
		View,
		Projection,
		Texture,
	};
	// ------------------------------------------------------------------//
	enum class TextureChannel : u32
	{
		Texture_0,
		Texture_1,
		Texture_2,
		Texture_3,
		Texture_4,
	};
	enum { k_Texture_Channels_Count = 5 };
	// ------------------------------------------------------------------//
	enum class CullingMode : u32
	{
		CW,
		CCW,
	};
	// ------------------------------------------------------------------//
	enum class LightChannel : u32
	{
		Light_0,
		Light_1,
		Light_2,
		Light_3,
	};
	enum { k_Light_Channels_Count = 4 };
	// ------------------------------------------------------------------//
	enum class FogMode : u32
	{
		Linear,
		Exp,
		Exp2,
	};
	// ------------------------------------------------------------------//
	
	enum class Primitive : u32
	{
		Lines			= GL_LINES,
		LineStrip		= GL_LINE_STRIP,
		Points			= GL_POINTS,
		Triangles		= GL_TRIANGLES,
		TriangleStrips	= GL_TRIANGLE_STRIP,
	};

	// ------------------------------------------------------------------//
	enum { k_Animation_Bones_Max = 64 };
	// ------------------------------------------------------------------//

	typedef struct
	{
		bool	m_isEnabled;
		Vec3	m_position;
		Vec3	m_direction;

		Vec4	m_ambient;
		Vec4	m_diffuse;
		Vec4	m_specular;

		f32		m_attenuation;
        f32		m_fallOffAngle;
        f32     m_fallOffExponent;
	} TLightParameters;
	
	// ------------------------------------------------------------------//
	
	typedef struct
	{
		u32 equation;
		u32 src;
		u32 dst;

	} TBlendFunc;

	// ------------------------------------------------------------------//

	typedef struct
	{
		u32 equation;

	} TDepthFunc;

	// ------------------------------------------------------------------//

	typedef struct
	{
        void*       m_vertexData;
        bool        m_isEnabled;
	} TVertexAttributeInfo;
	
	// ------------------------------------------------------------------//
	typedef void (CDriver::*SetUniformCallback)( s32 loc, void *value, u32 count );
	
	typedef struct
	{
        void*							m_value;
		u32								m_count;			
		CDriver::SetUniformCallback		m_callback;

	} TUniformInfo;

	// ------------------------------------------------------------------//
	static CRect2D*				ScreenRect;

	// ------------------------------------------------------------------//
	void						Initialize();
	// ------------------------------------------------------------------//
	void						Clear( Vec4 color );
	void						ClearColor( Vec4 color );
	void						Flush2D();
	// ------------------------------------------------------------------//

	TextureChannel				SetTextureChannel( TextureChannel textureSelect );
	LightChannel				SetLightChannel( LightChannel lightSelect );
	CullingMode					SetCullingMode( CullingMode cullingMode );
	MatrixMode					SetMatrixMode( MatrixMode matrixMode );
	
	u32							BindTexture ( u32 tex );
	CRenderTexture*				BindRenderTexture( CRenderTexture* framebuffer );

	bool						EnableCulling( bool val );
	bool						EnableBlending( bool val );
	bool						EnableDepthTest( bool val );
	bool						EnableDepthMask( bool val );
	void						EnableBlendFunc( TBlendFunc blendfunc );
	void						EnableBlendFunc( u32 equation, u32 src, u32 dst );
	void						EnableDepthFunc( TDepthFunc blendfunc);
	void						EnableDepthFunc(u32 equation);
	bool						EnableBatch2D( bool isEnabled);

	void						Tick( f32 dt );
	inline f32				    GetTimer()   { return m_timer; }
	inline f32					GetTimerSin()  { return m_timerSin; }
	inline f32					GetTimerCos()   { return m_timerCos; }
	inline f32					GetTimerRot()  { return m_timerRot; }

	void						MatrixPush();
	void						MatrixPop();
	void						MatrixDirty();

	void						MatrixLoadIdentity();
	void						MatrixLoad( Mat4 *mat );
	void						MatrixMultiply( Mat4 *mat );

	void						MatrixTranslate( Vec3 translation );
	void						MatrixRotate( Vec3 axis, f32 angle );
	void						MatrixScale( Vec3 scale );

    inline Mat3*                GetNormalMatrix() ;

    inline Mat4*                GetWorldMatrix() ;
    inline Mat4*                GetWorldMatrixInverse() ;
    inline Mat4*                GetWorldMatrixInverseT() ;
                                
    inline Mat4*                GetViewMatrix() ;
    inline Mat4*                GetViewMatrixInverse() ;
    inline Mat4*                GetViewMatrixInverseT() ;
                                
    inline Mat4*                GetProjectionMatrix() ;
    inline Mat4*                GetTextureMatrix(TextureChannel texChannel) ;

	void						SetScreenSize(u32 width, u32 height);
	void						SetViewport( u32 width, u32 height );

    inline void					SetColor(Vec4 col);
    inline Vec4                 GetColor() ;
    inline void					SetPointSize(float size);
    inline f32					GetPointSize() ;

    inline void					SetFogColor(Vec4 col);
    inline Vec4                 GetFogColor() ;

    inline void					SetFogMode(FogMode fogMode);

    inline void					SetFogStart(f32 fogStart);
    inline f32				    GetFogStart() ;

    inline  void				SetFogEnd(f32 fogEnd);
    inline f32				    GetFogEnd() ;

    inline void					SetFogDensity(f32 fogDensity);
    inline f32				    GetFogDensity() ;

    inline void					SetLightPosition(Vec3 pos);
    inline Vec3                 GetLightPosition() ;

    inline void					SetLightDirection(Vec3 dir);
    inline Vec3                 GetLightDirection() ;

    inline void					SetLightAmbient(Vec4 ambient);
    inline Vec4                 GetLightAmbient() ;

    inline void					SetLightDiffuse(Vec4 diffuse);
    inline Vec4                 GetLightDiffuse() ;

    inline void					SetLightSpecular(Vec4 specular);
    inline Vec4                 GetLightSpecular() ;

    inline void					SetLightAttenuation(f32 attenuation);
    inline f32				    GetLightAttenuation() ;

    inline void					SetLightFallOffAngle(f32 fallOffAngle);
    inline f32				    GetLightFallOffAngle();

    inline void					SetLightFallOffExponent(f32 fallOffExp);
    inline f32				    GetLightFallOffExponent();

    inline void					SetMaterialShininess(f32 shininess);
    inline f32				    GetMaterialShininess() ;

    inline void					SetMaterialRefraction(f32 refraction);
    inline f32				    GetMaterialRefraction() ;

    inline Vec4                 GetMaterialAmbient() ;
    inline void					SetMaterialAmbient(Vec4 ambient);

    inline Vec4                 GetMaterialDiffuse() ;
    inline  void				SetMaterialDiffuse(Vec4 diffuse);

    inline Vec4                 GetMaterialSpecular() ;
    inline void					SetMaterialSpecular(Vec4 specular);

    inline Vec4                 GetMaterialEmissive() ;
    inline void					SetMaterialEmissive(Vec4 emissive);

    inline Vec4                 GetMaterialReflective() ;
    inline void					SetMaterialReflective(Vec4 col);


    inline void					SetEyePosition(Vec3 pos);
	inline Vec3			        GetEyePosition() ;

    inline void					SetEyeDirection(Vec3 pos);
    inline Vec3                 GetEyeDirection() ;

	void						SetDepthRange( f32 start, f32 end );

	// ------------------------------------------------------------------//
	void                        SetVertexAttribute( CShader::TAttrib* attrib, void *vertexData, CVertexSource::AttributeStride vertexStride );
    void                        EnableVertexAttribute( CShader::TAttrib* attrib );
    void                        DisableVertexAttribute( CShader::TAttrib* attrib );
	// ------------------------------------------------------------------//
	void						UpdateUniforms( CEffect *effect );
	void						SetUniform( CShader::TUniform* uni );
	// ------------------------------------------------------------------//
	void						Render( CVertexGroup* vertexGroup );
	// ------------------------------------------------------------------//
	void						Log( char *fmt, ... );
	// ------------------------------------------------------------------//
	inline u32					GetDrawCallCount()	{ return m_drawCallCount; }
	inline u32					GetVertexCount()	{ return m_vertexCount; }

	// ------------------------------------------------------------------//

protected:
	void						ComputeNormalMatrix();

	void						SetUniform1f( s32 loc, void *value, u32 count );
	void						SetUniform1i( s32 loc, void *value, u32 count );

	void						SetUniform3fv( s32 loc, void *value, u32 count );
	void						SetUniform4fv( s32 loc, void *value, u32 count );

	void						SetUniformMatrix3fv( s32 loc, void *value, u32 count );
	void						SetUniformMatrix4fv( s32 loc, void *value, u32 count );

	void						InitUniform();
	// ------------------------------------------------------------------//
protected:
	// ------------------------------------------------------------------//
	TUniformInfo				m_uniformInfo[ CShader::k_Uniform_Count ];
	// ------------------------------------------------------------------//

	CBatch2D*					m_batch2D;
	bool						m_isEnabledBatch2D;

    Mat4Stack				    m_worldStack;
    Mat4Stack				    m_viewStack;
    Mat4Stack				    m_projectionStack;

	Mat4Stack*				    m_activeStack;

	Mat4Stack				    m_textureStack[k_Texture_Channels_Count];
	TextureChannel				m_textureChannel;

	Mat4					    m_worldInverseMatrix;
	Mat4					    m_worldInverseTMatrix;
	bool						m_isWorldMatrixDirty;
	bool						m_isActiveStackAlteringWorldMatrix;

	Mat4					    m_viewInverseMatrix;
	Mat4					    m_viewInverseTMatrix;
	bool						m_isViewMatrixDirty;
	bool						m_isActiveStackAlteringViewMatrix;

	Mat4					    m_worldViewMatrix;
	bool						m_isWorldViewMatrixDirty;
	bool						m_isActiveStackAlteringWorldViewMatrix;

	Mat4					    m_viewProjectionMatrix;
	bool						m_isViewProjectionMatrixDirty;
	bool						m_isActiveStackAlteringViewProjectionMatrix;

	Mat4					    m_worldViewProjectionMatrix;
	bool						m_isWorldViewProjectionMatrixDirty;
	bool						m_isActiveStackAlteringWorldViewProjectionMatrix;

	Mat3					    m_normalMatrix;
	bool						m_isNormalMatrixDirty;
	bool						m_isActiveStackAlteringNormalMatrix;
	MatrixMode					m_matrixMode;

	Mat4					    m_boneArrayMatrix[ k_Animation_Bones_Max ];

	u32				            m_textureBind[ k_Texture_Channels_Count ];

    TVertexAttributeInfo        m_vertexAttributeInfo[ CVertexSource::k_Vertex_Attributes_Count ];
	TLightParameters			m_lightParameters[ k_Light_Channels_Count ];
	LightChannel				m_lightChannel;

	CullingMode					m_cullingMode;

	TBlendFunc					m_blendFunc;
	TDepthFunc					m_depthFunc;

	Vec4						m_fogColor;
	FogMode						m_fogMode;
	f32						    m_fogDensity;
	f32						    m_fogStart;
	f32						    m_fogEnd;

	f32							m_depthStart;
	f32							m_depthEnd;

	f32						    m_materialShininess;
	f32						    m_materialRefraction;
	Vec4						m_materialAmbient;
	Vec4						m_materialDiffuse;
	Vec4						m_materialSpecular;
	Vec4						m_materialEmissive;
	Vec4					    m_materialReflective;

	Vec3						m_eyePosition;
	Vec3						m_eyeDirection;

	bool						m_isCullingEnabled;
	bool						m_isBlendingEnabled;
	bool						m_isDepthTestEnabled;
	bool						m_isDepthMaskEnabled;

	f32						    m_timer;
	f32						    m_timerSin;
	f32							m_timerCos;
	f32							m_timerRot;

	Vec4						m_color;
	f32							m_pointSize;

	u32							m_screenWidth;
	u32							m_screenHeight;
	u32							m_viewportWidth;
	u32							m_viewportHeight;
	// ------------------------------------------------------------------//
	CVertexSource*				m_crtVertexSource;
	CRenderTexture*				m_crtRenderTexture;
	// ------------------------------------------------------------------//
	u32							m_drawCallCount;
	u32							m_vertexCount;
	// ------------------------------------------------------------------//
};


 inline Mat3* CDriver::GetNormalMatrix() 
{
    return &m_normalMatrix; 
}

inline Mat4* CDriver::GetWorldMatrix()  
{
    return m_worldStack.topmatrix; 
}

inline Mat4* CDriver::GetWorldMatrixInverse()  
{
    return &m_worldInverseMatrix; 
}

inline Mat4* CDriver::GetWorldMatrixInverseT()  
{
    return &m_worldInverseTMatrix; 
}

inline Mat4* CDriver::GetViewMatrix()  
{
    return m_viewStack.topmatrix; 
}

inline Mat4* CDriver::GetViewMatrixInverse() 
{
    return &m_viewInverseMatrix; 
}

inline Mat4* CDriver::GetViewMatrixInverseT() 
{
    return &m_viewInverseTMatrix; 
}

inline Mat4* CDriver::GetProjectionMatrix() 
{
    return m_projectionStack.topmatrix; 
}

inline Mat4* CDriver::GetTextureMatrix(TextureChannel texChannel)  
{
    return m_textureStack[Value(texChannel)].topmatrix; 
}

void CDriver::SetColor(Vec4 col) 
{ 
    m_color = col;
}

inline Vec4 CDriver::GetColor()  
{ 
    return m_color; 
}

void CDriver::SetPointSize(float size) 
{ 
    m_pointSize = size; 
}

inline f32 CDriver::GetPointSize() 
{ 
    return m_pointSize;
}

void CDriver::SetFogColor(Vec4 col) 
{ 
    m_fogColor = col; 
}

inline Vec4 CDriver::GetFogColor() 
{ 
    return m_fogColor;
}

void CDriver::SetFogMode(FogMode fogMode) 
{ 
    m_fogMode = fogMode; 
}

void CDriver::SetFogStart(f32 fogStart)
{ 
    m_fogStart = fogStart;
}

inline f32 CDriver::GetFogStart() 
{ 
    return m_fogStart; 
}

void CDriver::SetFogEnd(f32 fogEnd)
{ 
    m_fogEnd = fogEnd; 
}

inline f32 CDriver::GetFogEnd() 
{ 
    return m_fogEnd;
}

void CDriver::SetFogDensity(f32 fogDensity)
{ 
    m_fogDensity = fogDensity;
}

inline f32 CDriver::GetFogDensity() 
{ 
    return m_fogDensity; 
}

inline void CDriver::SetLightPosition(Vec3 pos) 
{ 
    m_lightParameters[Value(m_lightChannel)].m_position = pos;
}

inline Vec3 CDriver::GetLightPosition() 
{ 
    return m_lightParameters[Value(m_lightChannel)].m_position;
}

inline void CDriver::SetLightDirection(Vec3 dir)
{ 
    m_lightParameters[Value(m_lightChannel)].m_direction = dir; 
}
inline Vec3 CDriver::GetLightDirection()  
{
    return m_lightParameters[Value(m_lightChannel)].m_direction;
}

inline void CDriver::SetLightAmbient(Vec4 ambient)
{ 
    m_lightParameters[Value(m_lightChannel)].m_ambient = ambient; 
}

inline Vec4 CDriver::GetLightAmbient()  
{ 
    return m_lightParameters[Value(m_lightChannel)].m_ambient;
}

inline void CDriver::SetLightDiffuse(Vec4 diffuse)
{ 
    m_lightParameters[Value(m_lightChannel)].m_diffuse = diffuse;
}

inline Vec4 CDriver::GetLightDiffuse()  
{ 
    return m_lightParameters[Value(m_lightChannel)].m_diffuse;
}

inline void CDriver::SetLightSpecular(Vec4 specular)
{ 
    m_lightParameters[Value(m_lightChannel)].m_specular = specular;
}
inline Vec4 CDriver::GetLightSpecular()  
{ 
    return m_lightParameters[Value(m_lightChannel)].m_specular; 
}

inline void CDriver::SetLightAttenuation(f32 attenuation)
{ 
    m_lightParameters[Value(m_lightChannel)].m_attenuation = attenuation;
}

inline f32 CDriver::GetLightAttenuation()
{ 
    return m_lightParameters[Value(m_lightChannel)].m_attenuation;
}

inline void CDriver::SetLightFallOffAngle(f32 fallOffAngle)
{
    m_lightParameters[Value(m_lightChannel)].m_fallOffAngle = fallOffAngle;
}

inline f32 CDriver::GetLightFallOffAngle()
{
    return m_lightParameters[Value(m_lightChannel)].m_fallOffAngle;
}

inline void CDriver::SetLightFallOffExponent(f32 fallOffExp)
{
    m_lightParameters[Value(m_lightChannel)].m_fallOffExponent = fallOffExp;
}

inline f32 CDriver::GetLightFallOffExponent()
{
    return m_lightParameters[Value(m_lightChannel)].m_fallOffExponent;
}

inline void CDriver::SetMaterialShininess(f32 shininess)
{ 
    m_materialShininess = shininess; 
}

inline f32 CDriver::GetMaterialShininess()  
{ 
    return m_materialShininess; 
}

inline void CDriver::SetMaterialRefraction(f32 refraction) 
{ 
    m_materialRefraction = refraction; 
}

inline f32 CDriver::GetMaterialRefraction() 
{ 
    return m_materialRefraction;
}

inline Vec4 CDriver::GetMaterialAmbient()  
{ 
    return m_materialAmbient; 
}

inline void CDriver::SetMaterialAmbient(Vec4 ambient) 
{ 
    m_materialAmbient = ambient;
}

inline Vec4 CDriver::GetMaterialDiffuse()  
{
    return m_materialDiffuse;
}

inline  void CDriver::SetMaterialDiffuse(Vec4 diffuse)
{ 
    m_materialDiffuse = diffuse;
}

inline Vec4 CDriver::GetMaterialSpecular()  
{ 
    return m_materialSpecular;
}

inline void CDriver::SetMaterialSpecular(Vec4 specular) 
{ 
    m_materialSpecular = specular;
}

inline Vec4 CDriver::GetMaterialEmissive()  
{ 
    return m_materialEmissive; 
}

inline void CDriver::SetMaterialEmissive(Vec4 emissive)
{ 
    m_materialEmissive = emissive;
}

inline Vec4 CDriver::GetMaterialReflective()  
{ 
    return m_materialReflective; 
}

inline void CDriver::SetMaterialReflective(Vec4 reflective)
{
    m_materialReflective = reflective;
}


inline void CDriver::SetEyePosition(Vec3 position) 
{ 
    m_eyePosition = position;
}

inline Vec3 CDriver::GetEyePosition() 
{ 
    return m_eyePosition;
}

inline void CDriver::SetEyeDirection(Vec3 direction) 
{ 
    m_eyeDirection = direction;
}

inline Vec3 CDriver::GetEyeDirection()  
{ 
    return m_eyeDirection; 
}
// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_RENDERER_H
