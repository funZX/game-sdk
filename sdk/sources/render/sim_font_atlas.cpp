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

#include <core/io/sim_mem_stream.h>

#include <render/sim_font_atlas.h>
#include <render/sim_font.h>

#include <render/sim_driver.h>
#include <render/sim_material.h>
#include <render/sim_effect.h>
#include <render/sim_texture.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CFontAtlas::CFontAtlas()
{
	m_material 		= nullptr;
	m_effect		= nullptr;
	m_texture		= nullptr;

    m_imAtlas       = SIM_NEW ImFontAtlas;
}

// ----------------------------------------------------------------------//

CFontAtlas::CFontAtlas( const std::string &name ) 
	: CFontAtlas()
{
	m_name = name;
}
// ----------------------------------------------------------------------//

CFontAtlas::~CFontAtlas()
{
    m_imAtlas->ClearFonts();
    SIM_SAFE_DELETE( m_imAtlas );

	SIM_SAFE_DELETE( m_material );
	SIM_SAFE_DELETE( m_texture );
	SIM_SAFE_DELETE( m_effect );
}

// ----------------------------------------------------------------------//

CFont* CFontAtlas::AddFont( const std::string name, io::CMemStream* ms, f32 pixelSize )
{
    CFont* font = SIM_NEW CFont( this );

    ImFontConfig fontConfig;

    fontConfig.FontDataOwnedByAtlas = true;
    fontConfig.SizePixels = pixelSize;
    fontConfig.FontDataSize = ms->GetSize();
    fontConfig.FontData = (void*) SIM_NEW u8[ fontConfig.FontDataSize ];
    SIM_MEMCPY( fontConfig.FontData, ms->Read(0), fontConfig.FontDataSize );

    font->m_imFont      = m_imAtlas->AddFontFromMemoryTTF(fontConfig.FontData, fontConfig.FontDataSize, (f32)pixelSize, &fontConfig);
    font->m_pixelSize   = pixelSize;

    return font;
}

// ----------------------------------------------------------------------//

void CFontAtlas::Create()
{
    s32 texWidth = 0;
    s32 texHeight = 0;
    u8* texBuf = nullptr;

    m_imAtlas->GetTexDataAsAlpha8( &texBuf, &texWidth, &texHeight );

	m_texture = SIM_NEW CTexture( m_name );
	m_texture->Generate( texBuf
		, texWidth
		, texHeight
		, CTexture::Type::TGA
		, CTexture::Wrap::Clamp
		, CTexture::Filter::Nearest
		, CTexture::Format::Alpha
	);

	InitEffect();
	InitMaterial();

    m_imAtlas->ClearTexData();
    m_imAtlas->ClearInputData();

    m_imAtlas->TexID = m_material;
}

// ----------------------------------------------------------------------//
void CFontAtlas::InitEffect()
{
	const s8* vsource =
		"attribute vec4 a_PositionL;"
		"attribute vec2 a_TexCoord_0;"

		"uniform mat4 u_Matrix_WorldViewProjection;"
		"uniform vec4 u_Material_Diffuse;"
		"uniform vec4 u_Color;"

		"varying vec2 v_Tex0;"
		"varying vec4 v_Color;"

		"void main()"
		"{"
		"	v_Tex0			= a_TexCoord_0;"
		"	v_Color			= u_Color * u_Material_Diffuse;"

		"	gl_Position		= u_Matrix_WorldViewProjection * a_PositionL;"
		"}";

	// ----------------------------------------------------------------------//

	const s8 *psource =
		"precision mediump float;"

		"uniform sampler2D	u_Sampler_Tex_0;"

		"varying vec2 v_Tex0;"
		"varying vec4 v_Color;"

		"void main()"
		"{"
		"	vec4 tex = texture2D( u_Sampler_Tex_0, v_Tex0 );"
		"	vec4 col = v_Color;"
		"	col.a	 = tex.a;"

		"	gl_FragColor = col;"
		"}";

	static const s8* attributes[] =
	{
		"a_PositionL",
		"a_TexCoord_0"
	};
	
	m_effect = SIM_NEW CEffect( m_name );

	u32 nAttrib = 2;
	m_effect->InitAttributes(nAttrib);
	for (u32 k = 0; k < nAttrib; k++)
		m_effect->AddAttribute(attributes[k], k);

	static const s8* uniforms[] =
	{
		"u_Matrix_WorldViewProjection",
		"u_Sampler_Tex_0",
		"u_Material_Diffuse",
		"u_Color"
	};

	u32 nUniform = 4;
	m_effect->InitUniforms(nUniform);
	for (u32 k = 0; k < nUniform; k++)
		m_effect->AddUniform(uniforms[k], k);

	m_effect->Load( vsource, psource );

	m_effect->m_technique.depthtest = true;
	m_effect->m_technique.depthmask = true;
	m_effect->m_technique.cullface = true;
	m_effect->m_technique.alphatest = false;

	m_effect->m_technique.blending = true;
	m_effect->m_technique.blendfunc.equation = GL_FUNC_ADD;
	m_effect->m_technique.blendfunc.src = GL_SRC_ALPHA;
	m_effect->m_technique.blendfunc.dst = GL_ONE_MINUS_SRC_ALPHA;

	m_effect->m_technique.depthfunc.equation = GL_LESS;
}

void CFontAtlas::InitMaterial()
{
// ----------------------------------------------------------------------//
	m_material = SIM_NEW CMaterial( m_name );
	m_material->SetTexture( m_texture, 0 );
	m_material->SetDiffuse( col::Green );
	m_material->SetEffect( m_effect );
}
// ----------------------------------------------------------------------//
bool CFontAtlas::Load(io::CMemStream* ms)
{
    return false;
}
// ----------------------------------------------------------------------//
bool CFontAtlas::Save(io::CMemStream* ms)
{
    return false;
}
// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim