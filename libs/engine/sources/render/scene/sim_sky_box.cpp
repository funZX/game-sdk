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

#include <render/scene/sim_sky_box.h>

#include <render/sim_driver.h>
#include <render/sim_shader.h>
#include <render/sim_effect.h>
#include <render/sim_cube_texture.h>
#include <render/sim_material.h>
#include <render/sim_vertex_group.h>
#include <render/sim_vertex_source.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CSkyBox::CSkyBox()
{
	InitEffect();

	m_texture			= SIM_NEW CCubeTexture();

	m_material			= SIM_NEW CMaterial();
	m_material->SetEffect( m_effect );

	m_vertexGroup		= SIM_NEW CVertexGroup();
	m_vertexGroup->m_vertexSource = SIM_NEW CVertexSource();

	m_vertexGroup->SetMaterial( m_material );
}

// ----------------------------------------------------------------------//

CSkyBox::CSkyBox( const std::string &name )
	: CSkyBox()
{
	m_name = name;
}
// ----------------------------------------------------------------------//

CSkyBox::~CSkyBox()
{
	SIM_SAFE_DELETE( m_effect );

	SIM_SAFE_DELETE_ARRAY( m_vertexGroup->m_vertexSource->m_vboData );
	SIM_SAFE_DELETE( m_vertexGroup->m_vertexSource );

	SIM_SAFE_DELETE_ARRAY( m_vertexGroup->m_vboData );
	SIM_SAFE_DELETE( m_vertexGroup );

	SIM_SAFE_DELETE( m_material );
	SIM_SAFE_DELETE( m_texture );
}

// ----------------------------------------------------------------------//

void CSkyBox::Generate( f32 size, 
						io::CMemStream* front,	io::CMemStream* back,
						io::CMemStream* left,	io::CMemStream* right,
						io::CMemStream* top,	io::CMemStream* bottom)
{
	static const f32 cubeVerts[ 72 ] =
	{ 
	  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
	  -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,
	  -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
	  -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f,
	  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,
	   1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f 
	};

	static const u16 cubeIndices[ 36 ] =
	{  
		0,  1,  2,  0,  2,  3,  4,  6,  5, 
	    4,  7,  6,  8, 10,  9,  8, 11, 10,
	   12, 14, 15, 12, 13, 14, 16, 18, 17,
	   16, 19, 18, 20, 22, 23, 20, 21, 22 
	};

	s32 numVertices			= 24;
	s32 numIndices			= 36;

	m_vertexGroup->m_vboData		= SIM_NEW u16[ numIndices ];
	m_vertexGroup->m_vboSize		= numIndices;

	m_vertexGroup->m_vertexSource->m_type			= CVertexSource::Type::Triangle;
	m_vertexGroup->m_vertexSource->m_vertexFormat	= CVertexSource::AttributeFormat::Position;
	m_vertexGroup->m_vertexSource->m_vertexStride	= CVertexSource::AttributeStride::Position;
	m_vertexGroup->m_vertexSource->m_vboSize		= numVertices;

	s32 vtxSize						= Value(m_vertexGroup->m_vertexSource->m_vertexStride) / sizeof( f32 );
	s32 newFloats 					= numVertices * vtxSize;
	s32 vboOff						= 0;

	m_vertexGroup->m_vertexSource->m_vboData = SIM_NEW f32[ newFloats ];

	for( s32 i = 0; i < numVertices; i++ )
	{
		s32		currIndex = i * vtxSize;
		f32*	curVertex = &m_vertexGroup->m_vertexSource->m_vboData[ currIndex ];
		s32		i3	=  3 * i;

		// position
		curVertex[ 0 ] = cubeVerts[ i3 + 0 ] * size;
		curVertex[ 1 ] = cubeVerts[ i3 + 1 ] * size;
		curVertex[ 2 ] = cubeVerts[ i3 + 2 ] * size;
	}

	SIM_MEMCPY( m_vertexGroup->m_vboData, cubeIndices, numIndices * sizeof( u16 ) );

	m_texture->Generate( front, back, left, right, top, bottom );
}

// ----------------------------------------------------------------------//

void CSkyBox::Render( CDriver *driver )
{
	driver->SetDepthRange( 0.99f, 1.0f );

	driver->SetMaterialReflectivity( m_material->GetReflectivity() );
	driver->SetMaterialShininess( m_material->GetShininess() );

	driver->SetMaterialAmbient( m_material->GetAmbient() );
	driver->SetMaterialDiffuse( m_material->GetDiffuse() );
	driver->SetMaterialSpecular( m_material->GetSpecular() );
	driver->SetMaterialEmissive( m_material->GetEmissive() );

	CDriver::TextureChannel tex =
	driver->SetTextureChannel( CDriver::TextureChannel::Texture_4 );
	glBindTexture( GL_TEXTURE_CUBE_MAP, m_texture->GetID() );
	driver->Render( m_vertexGroup );
	driver->SetTextureChannel( tex );
	
	SIM_CHECK_OPENGL();

	driver->SetDepthRange( 0.0f, 1.0f );
}

// ----------------------------------------------------------------------//
void CSkyBox::InitEffect()
{
	const char* vsource =
		"attribute vec4 a_PositionL;"

		"uniform mat4 u_Matrix_WorldViewProjection;"
		"uniform vec4 u_Material_Diffuse;"

		"uniform vec4 u_Color;"
		"uniform vec3 u_Eye_Position;"

		"varying vec4 v_Color;"
		"varying vec3 v_EyeDirL;"

		"void main()"
		"{"
		"	vec4 p;"

		"	p.xyz			= a_PositionL.xyz + u_Eye_Position.xyz; "
		"	p.w				= 1.0;"

		"	v_EyeDirL		= -a_PositionL.xyz;"
		"	v_Color			= u_Color * u_Material_Diffuse;"

		"	gl_Position		= u_Matrix_WorldViewProjection * p;"
		"}";

	// ----------------------------------------------------------------------//

	const char* psource =
		"precision mediump float;"

		"uniform samplerCube u_Sampler_Cube;"

		"varying vec4 v_Color;"
		"varying vec3 v_EyeDirL;"

		"void main ()"
		"{"
		"	vec4 tex = textureCube( u_Sampler_Cube, v_EyeDirL );"
		"	vec4 col = tex * v_Color;"

		"	gl_FragColor = col;"
		"}";

	static const char* attributes[] =
	{
		"a_PositionL"
	};

	m_effect = SIM_NEW CEffect( m_name );

	u32 nAttrib = 1;
	m_effect->InitAttributes(nAttrib);
	for ( u32 k = 0; k < nAttrib; k++ )
		m_effect->AddAttribute(attributes[k], k);

	static const char* uniforms[] =
	{
		"u_Matrix_WorldViewProjection",
		"u_Material_Diffuse",
		"u_Color",
		"u_Eye_Position",
		"u_Sampler_Cube"
	};

	u32 nUniform = 5;
	m_effect->InitUniforms(nUniform);
	for (u32 k = 0; k < nUniform; k++)
		m_effect->AddUniform(uniforms[k], k);

	m_effect->Load( vsource, psource );

	CEffect::TTechnique technique;

	technique.depthtest = false;
	technique.depthmask = false;
	technique.cullface	= true;
	technique.blending	= false;
	technique.alphatest = false;

	technique.blendfunc.equation = GL_FUNC_ADD;
	technique.blendfunc.src = GL_SRC_ALPHA;
	technique.blendfunc.dst = GL_ONE_MINUS_SRC_ALPHA;

	technique.depthfunc.equation = GL_ALWAYS;

	m_effect->SetTechnique(&technique);
}
// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
