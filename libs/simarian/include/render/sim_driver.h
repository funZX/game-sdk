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

#ifndef __SIM_RENDERER_H
#define __SIM_RENDERER_H

#include <core/sim_core.h>
#include <math/sim_matrix4_stack.h>
#include <math/sim_matrix4.h>
#include <math/sim_vec3.h>
#include <math/sim_vec4.h>

#include <render/sim_vertex_source.h>
#include <render/sim_vertex_group.h>
#include <render/sim_frame_buffer.h>
#include <render/sim_shader.h>

using namespace sim::mat;

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

class CEffect;

class CDriver
{
public:
	CDriver();
	virtual ~CDriver( );

	// ------------------------------------------------------------------//
	typedef enum
	{
		k_Select_Matrix_None = -1,

		k_Select_Matrix_World,
		k_Select_Matrix_View,
		k_Select_Matrix_Projection,
		k_Select_Matrix_Texture,

		k_Select_Matrix_Count

	} K_SELECT_MATRIX;
	// ------------------------------------------------------------------//
	typedef enum
	{
		k_Select_Texture_None = -1,

		k_Select_Texture_0,
		k_Select_Texture_1,
		k_Select_Texture_2,
		k_Select_Texture_3,
		k_Select_Texture_4,

		k_Select_Texture_Count
	} K_SELECT_TEXTURE;
	// ------------------------------------------------------------------//
	typedef enum
	{
		k_Select_Face_None = -1,

		k_Select_Face_CW,
		k_Select_Face_CCW,

		k_Select_Face_Count

	} K_SELECT_FACE;
	// ------------------------------------------------------------------//
	typedef enum
	{
		k_Select_Light_None = -1,

		k_Select_Light_0,
		k_Select_Light_1,
		k_Select_Light_2,
		k_Select_Light_3,

		k_Select_Light_Count

	} K_SELECT_LIGHT;
	// ------------------------------------------------------------------//
	typedef enum
	{
		k_Select_Fog_None = -1,

		k_Select_Fog_Linear,
		k_Select_Fog_Exp,
		k_Select_Fog_Exp2,

		k_Select_Fog_Count

	} K_SELECT_FOG;
	// ------------------------------------------------------------------//
	typedef enum
	{
		k_Select_Batch_None = -1,

		k_Select_Batch_2D,
		k_Select_Batch_3D,

		k_Select_Batch_Count

	} K_SELECT_BATCH;
	// ------------------------------------------------------------------//
	
	typedef enum
	{
		k_Render_Type_None = -1,

		k_Render_Type_Lines				= GL_LINES,
		k_Render_Type_LineStrip			= GL_LINE_STRIP,
		k_Render_Type_Points			= GL_POINTS,
		k_Render_Type_Triangles			= GL_TRIANGLES,
		k_Render_Type_TriangleStrips	= GL_TRIANGLE_STRIP,

		k_Render_Type_Count

	} K_RENDER_TYPE;

	// ------------------------------------------------------------------//

	typedef struct
	{
		bool	m_isEnabled;
		TVec3	m_position;
		TVec3	m_direction;

		TVec4	m_ambient;
		TVec4	m_diffuse;
		TVec4	m_specular;

		f32	m_intensity;
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

	static const u32			k_Skeleton_Bones_Max = 16;

	// ------------------------------------------------------------------//
	void						Initialize();
	// ------------------------------------------------------------------//
	void						Clear();
	void						Flush2D();
	// ------------------------------------------------------------------//

	K_SELECT_TEXTURE			SelectTexture( K_SELECT_TEXTURE textureSelect );
	K_SELECT_LIGHT				SelectLight( K_SELECT_LIGHT lightSelect );
	K_SELECT_FACE				SelectFace( K_SELECT_FACE faceSelect );
	K_SELECT_MATRIX				SelectMatrix( K_SELECT_MATRIX matrixSelect );
	K_SELECT_BATCH				SelectBatch( K_SELECT_BATCH batchSelect );

	u32							BindTexture ( u32 tex );
	CFrameBuffer*				BindFrameBuffer( CFrameBuffer* framebuffer );

	bool						EnableCulling( bool val );
	bool						EnableBlending( bool val );
	bool						EnableDepthTest( bool val );
	bool						EnableDepthMask( bool val );
	void						EnableBlendFunc( const TBlendFunc* blendfunc );
	void						EnableBlendFunc( u32 equation, u32 src, u32 dst );
	void						EnableDepthFunc(const TDepthFunc* blendfunc);
	void						EnableDepthFunc(u32 equation);

	void						Tick( const f32 dt );
	inline f32				    GetTimer()		{ return m_timer; }
	inline f32					GetTimerSin()	{ return m_timerSin; }
	inline f32					GetTimerCos()	{ return m_timerCos; }
	inline f32					GetTimerRot()	{ return m_timerRot; }

	void						MatrixPush();
	void						MatrixPop();
	void						MatrixDirty();

	void						MatrixLoadIdentity();
	void						MatrixLoad( const TMatrix4 *mat );
	void						MatrixMultiply( const TMatrix4 *mat );

	void						MatrixTranslate( const TVec3 *pos );
	void						MatrixTranslate( const f32 x, const f32 y, const f32 z );
	void						MatrixTranslateX( const f32 x );
	void						MatrixTranslateY( const f32 y );
	void						MatrixTranslateZ( const f32 z );

	void						MatrixRotate( const f32 angle, const TVec3 *rot );
	void						MatrixRotate( const f32 angle, const f32 x, const f32 y, const f32 z );
	void						MatrixRotateX( const f32 angle );
	void						MatrixRotateY( const f32 angle );
	void						MatrixRotateZ( const f32 angle );

	void						MatrixScale( const TVec3 *scl );
	void						MatrixScale( const f32 x, const f32 y, const f32 z );
	void						MatrixScaleX( const f32 scale );
	void						MatrixScaleY( const f32 scale );
	void						MatrixScaleZ( const f32 scale );

		   const TMatrix3*		GetNormalMatrix(){ return &m_normalMatrix; }

	inline const TMatrix4*		GetWorldMatrix()						{ return m_worldStack.topmatrix; }
	inline const TMatrix4*		GetWorldMatrixInverse()					{ return &m_worldInverseMatrix; }
	inline const TMatrix4*		GetWorldMatrixInverseT()				{ return &m_worldInverseTMatrix; }

	inline const TMatrix4*		GetViewMatrix()							{ return m_viewStack.topmatrix; }
	inline const TMatrix4*		GetViewMatrixInverse()					{ return &m_viewInverseMatrix; }
	inline const TMatrix4*		GetViewMatrixInverseT()					{ return &m_viewInverseTMatrix; }

	inline const TMatrix4*		GetProjectionMatrix()					{ return m_projectionStack.topmatrix; }
	inline const TMatrix4*		GetTextureMatrix( K_SELECT_TEXTURE e )	{ return m_textureStack[ e ].topmatrix; }

	void						SetScreenSize(u32 width, u32 height);
	void						SetViewport( u32 width, u32 height );

	void						SetColor( const TVec4 *col );
	inline TVec4*				GetColor() { return &m_color; }

	void						SetPointSize( const float size ) { m_pointSize = size; }
	inline f32					GetPointSize() { return m_pointSize; }

	void						SetFogColor( const TVec4 *col );
	inline TVec4*				GetFogColor() { return &m_fogColor; }

	void						SetFogMode( K_SELECT_FOG fogMode);

	void						SetFogStart( const f32 fogStart );
	inline f32				    GetFogStart() { return m_fogStart; }

	void						SetFogEnd( const f32 fogEnd );
	inline f32				    GetFogEnd() { return m_fogEnd; }

	void						SetFogDensity( const f32 d );
	inline f32				    GetFogDensity() { return m_fogDensity; }

	void						SetLightPosition( const TVec3 *pos );
	inline TVec3*				GetLightPosition() { return &m_lightParameters[ m_lightSelect ].m_position; }

	void						SetLightDirection( const TVec3 *dir );
	inline TVec3*				GetLightDirection() { return &m_lightParameters[ m_lightSelect ].m_direction; }

	void						SetLightAmbient( const TVec4 *col );
	inline TVec4*				GetLightAmbient() { return &m_lightParameters[ m_lightSelect ].m_ambient; }

	void						SetLightDiffuse( const TVec4 *col );
	inline TVec4*				GetLightDiffuse() { return &m_lightParameters[ m_lightSelect ].m_diffuse; }

	void						SetLightSpecular( const TVec4 *col );
	inline TVec4*				GetLightSpecular() { return &m_lightParameters[ m_lightSelect ].m_specular; }

	void						SetLightIntensity( f32 intens );
	inline f32				    GetLightIntensity() { return m_lightParameters[ m_lightSelect ].m_intensity; }

	inline f32				    GetMaterialReflectivity() { return m_materialReflectivity; }
	inline void					SetMaterialReflectivity( f32 refl ) { m_materialReflectivity = refl; }

	inline void					SetMaterialShininess( f32 shine ) { m_materialShininess = shine; }
	inline f32				    GetMaterialShininess() { return m_materialShininess; }

	inline TVec4*				GetMaterialAmbient() { return &m_materialAmbient; }
	inline void					SetMaterialAmbient( const TVec4 *col ) { Vec4Copy( &m_materialAmbient, col ); }

	inline TVec4*				GetMaterialDiffuse() { return &m_materialDiffuse; }
	inline  void				SetMaterialDiffuse( const TVec4 *col ) { Vec4Copy( &m_materialDiffuse, col ); }

	inline TVec4*				GetMaterialSpecular() { return &m_materialSpecular; }
	inline void					SetMaterialSpecular( const TVec4 *col ) { Vec4Copy( &m_materialSpecular, col ); }

	inline TVec4*				GetMaterialEmissive() { return &m_materialEmissive; }
	inline void					SetMaterialEmissive( const TVec4 *col ) { Vec4Copy( &m_materialEmissive, col ); }

	inline void					SetEyePosition( const TVec3 *pos ) { Vec3Copy( &m_eyePosition, pos ); }
	inline TVec3*				GetEyePosition() { return &m_eyePosition; }

	inline void					SetEyeDirection( const TVec3 *pos ) { Vec3Copy( &m_eyeDirection, pos ); }
	inline TVec3*				GetEyeDirection() { return &m_eyeDirection; }

	void						SetDepthRange( f32 start, f32 end );

	// ------------------------------------------------------------------//
	void                        SetVertexAttribute( CShader::TAttrib* attrib, void *vertexData, u32 vertexStride );
    void                        EnableVertexAttribute( CShader::TAttrib* attrib );
    void                        DisableVertexAttribute( CShader::TAttrib* attrib );
	// ------------------------------------------------------------------//
	void						UpdateUniforms( CEffect *effect );
	void						SetUniform( CShader::TUniform* uni );
	// ------------------------------------------------------------------//
	void						Render( CVertexGroup* vertexGroup );
	// ------------------------------------------------------------------//
	void						Log( const char *fmt, ... );
	// ------------------------------------------------------------------//
	inline u32					GetDrawCallCount()	{ return m_drawCallCount; }
	inline u32					GetVertexCount()	{ return m_vertexCount; }

	const std::string&			GetName()		{ return m_name; }
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
	std::string					m_name;
	// ------------------------------------------------------------------//
	TUniformInfo				m_uniformInfo[ CShader::k_Uniform_Count ];
	// ------------------------------------------------------------------//

	CBatch2D*					m_batch2D;
	K_SELECT_BATCH				m_batchSelect;

	TMatrix4Stack				m_worldStack;
	TMatrix4Stack				m_viewStack;
	TMatrix4Stack				m_projectionStack;

	TMatrix4Stack*				m_activeStack;

	TMatrix4Stack				m_textureStack[ k_Select_Texture_Count ];
	K_SELECT_TEXTURE			m_textureSelect;

	TMatrix4					m_worldInverseMatrix;
	TMatrix4					m_worldInverseTMatrix;
	bool						m_isWorldMatrixDirty;
	bool						m_isActiveStackAlteringWorldMatrix;

	TMatrix4					m_viewInverseMatrix;
	TMatrix4					m_viewInverseTMatrix;
	bool						m_isViewMatrixDirty;
	bool						m_isActiveStackAlteringViewMatrix;

	TMatrix4					m_worldViewMatrix;
	bool						m_isWorldViewMatrixDirty;
	bool						m_isActiveStackAlteringWorldViewMatrix;

	TMatrix4					m_viewProjectionMatrix;
	bool						m_isViewProjectionMatrixDirty;
	bool						m_isActiveStackAlteringViewProjectionMatrix;

	TMatrix4					m_worldViewProjectionMatrix;
	bool						m_isWorldViewProjectionMatrixDirty;
	bool						m_isActiveStackAlteringWorldViewProjectionMatrix;

	TMatrix3					m_normalMatrix;
	bool						m_isNormalMatrixDirty;
	bool						m_isActiveStackAlteringNormalMatrix;
	K_SELECT_MATRIX				m_matrixSelect;

	TMatrix4					m_skeletonMatrix[ k_Skeleton_Bones_Max ];

	u32				            m_textureBind[ k_Select_Texture_Count ];

    TVertexAttributeInfo        m_vertexAttributeInfo[ CShader::k_Attribute_Count ];
	TLightParameters			m_lightParameters[ k_Select_Light_Count ];
	K_SELECT_LIGHT				m_lightSelect;

	K_SELECT_FACE				m_faceSelect;

	TBlendFunc					m_blendFunc;
	TDepthFunc					m_depthFunc;

	TVec4						m_fogColor;
	K_SELECT_FOG				m_fogMode;
	f32						    m_fogDensity;
	f32						    m_fogStart;
	f32						    m_fogEnd;

	f32							m_depthStart;
	f32							m_depthEnd;

	f32						    m_materialReflectivity;
	f32						    m_materialShininess;
	TVec4						m_materialAmbient;
	TVec4						m_materialDiffuse;
	TVec4						m_materialSpecular;
	TVec4						m_materialEmissive;

	TVec3						m_eyePosition;
	TVec3						m_eyeDirection;

	bool						m_isCullingEnabled;
	bool						m_isBlendingEnabled;
	bool						m_isDepthTestEnabled;
	bool						m_isDepthMaskEnabled;

	f32						    m_timer;
	f32						    m_timerSin;
	f32							m_timerCos;
	f32							m_timerRot;

	TVec4						m_color;
	f32							m_pointSize;

	u32							m_screenWidth;
	u32							m_screenHeight;
	u32							m_viewportWidth;
	u32							m_viewportHeight;
	// ------------------------------------------------------------------//
	CEffect*					m_crtEffect;
	CMaterial*					m_crtMaterial;
	CVertexSource*				m_crtVertexSource;
	CFrameBuffer*				m_crtFrameBuffer;
	// ------------------------------------------------------------------//
	u32							m_drawCallCount;
	u32							m_vertexCount;
	// ------------------------------------------------------------------//

};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_RENDERER_H
