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

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

#include <algorithm>

#include <core/sim_pool.h>
#include <render/font/sim_font_atlas.h>
#include <render/font/sim_font_char.h>
#include <render/font/sim_font_node.h>
#include <render/font/sim_font.h>

#include <render/sim_driver.h>
#include <render/sim_material.h>
#include <render/sim_effect.h>
#include <render/sim_batch_2d.h>
#include <render/sim_texture.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CFontAtlas::CFontAtlas()
{
	m_useKerning 	= false;
	m_library 		= nullptr;

	m_material 		= nullptr;
	m_effect		= nullptr;
	m_texture		= nullptr;
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
	for(s32 n = 0; n < (s32)m_listBitmapChar.size(); n++) {
        SIM_SAFE_DELETE( m_listBitmapChar[ n ] );
    }

	SIM_SAFE_DELETE( m_material );
	SIM_SAFE_DELETE( m_texture );
	SIM_SAFE_DELETE( m_effect );

	ReleaseLibrary();
}

// ----------------------------------------------------------------------//

void CFontAtlas::ReleaseLibrary()
{
	if( m_library )
	{
		FT_Done_FreeType( m_library );
		m_library = nullptr;
	}
}

// ----------------------------------------------------------------------//

CFont* CFontAtlas::AddFont( const std::string &name, u8* memfile, s32 memsize, s32 size, const char* szLetters )
{
	if( m_library == nullptr )
	{
		if( FT_Init_FreeType( &m_library ) ) {
			SIM_ASSERT( 0 && SIM_PRINT("FT_Init_Frek_Type_ failed") );
		}
	}

	// The Object In Which Frek_Type_ Holds Information On A Given
	// Font Is Called A "face".
	FT_Face face;

	// This Is Where We Load In The Font Information From The File.
	if( FT_New_Memory_Face( m_library, memfile, memsize, 0, &face ) ) {
		SIM_ASSERT( 0 && SIM_PRINT( "font not loaded: %s", name.c_str() ) );
	}

	// Frek_Type_ Measures Font Size In Terms Of 1/64ths Of Pixels.
	FT_Set_Char_Size( face, size * 64, size * 64, 72, 72 );

	s32 len = strlen( szLetters );
	s32 n;
	CFontChar* fontChar;
	FT_Glyph glyph;

	CFont* ftBitmapFont = SIM_NEW CFont( name, this );
	ftBitmapFont->SetHeight( face->size->metrics.height >> 6 );
	ftBitmapFont->SetFTFace( face );
	m_listBitmapFont.push_back( ftBitmapFont );

	u8 c;
	s32 height;
	s32 yOffset;
	s32 ixGlyph;

	for( n = 0; n < len; n++ )
	{
		c = szLetters[ n ];
		// chaeck that the character hasn't already been processed
		if( ftBitmapFont->GetChar( c ) == nullptr )
		{
			// Load The Glyph For Our Character.
			ixGlyph = FT_Get_Char_Index( face, c );
			if( ixGlyph == 0 ) {
				SIM_PRINT( "character doesn't exist in font: %c", c );
			}
			else
			{
				if( FT_Load_Glyph( face, ixGlyph, FT_LOAD_DEFAULT ) ) {
					SIM_ASSERT( 0 && SIM_PRINT( "FT_Load_Glyph failed" ) );
				}

				// Move The Face's Glyph Into A Glyph Object.
				if( FT_Get_Glyph( face->glyph, &glyph ) ) {
					SIM_ASSERT( 0 && SIM_PRINT( "FT_Get_Glyph failed" ) );
				}

				s8 n[2]; n[ 0 ] = c; n[ 1 ] = '\0';
				std::string name( ( char* ) &n[0] );
				fontChar = SIM_NEW CFontChar();

				// all metrics dimensions are multiplied by 64, so we have to divide by 64
				height = face->glyph->metrics.height >> 6;
				yOffset = ftBitmapFont->GetHeight() - ( face->glyph->metrics.horiBearingY >> 6 ) ;
				fontChar->SetOffsets( face->glyph->metrics.horiBearingX >> 6, yOffset );
				fontChar->SetSize( face->glyph->metrics.width >> 6, height );
				fontChar->SetXAdvance( face->glyph->metrics.horiAdvance >> 6 );
				fontChar->SetGlyph( glyph );
				m_listBitmapChar.push_back( fontChar );
				ftBitmapFont->AddChar( c, fontChar );
			}
		}
	}

	return ftBitmapFont;
}

// ----------------------------------------------------------------------//

void GetNextTextureSize( s32 &texWidth, s32 &texHeight, s32 ixSize )
{
	if( ixSize % 2 ) {
		texHeight *= 2;
	}
	else {
		texWidth *= 2;
	}

	if( texWidth > 1024 || texHeight > 1024 ) {
		SIM_ASSERT( 0 && SIM_PRINT( "to many images to fit in one texture") );
	}
}

// ----------------------------------------------------------------------//

bool GreaterSize( CFontChar* ftBitmapChar1, CFontChar* ftBitmapChar2 )
{
	return ftBitmapChar1->GetNumPixels() > ftBitmapChar2->GetNumPixels();
}

// ----------------------------------------------------------------------//

void CFontAtlas::Create()
{
	s32 totalPixels = 0;

    for( s32 n = 0; n < (s32)m_listBitmapChar.size(); n++ ) {
        totalPixels += m_listBitmapChar[n]->GetNumPixels();
    }

    s32 ixSize      = 0;
	s32 texWidth    = 32;
	s32 texHeight   = 32;

    while( true )
    {
    	if( totalPixels <= texWidth * texHeight ) {
    		break;
    	}

		GetNextTextureSize( texWidth, texHeight, ixSize );
		ixSize++;
    }

	sort( m_listBitmapChar.begin(), m_listBitmapChar.end(), GreaterSize );
	
	stl::CPool<CFontNode>* pool = CFontNode::NewPool();
	while( !BinPack( pool, texWidth, texHeight ) )
    {
		GetNextTextureSize(texWidth, texHeight, ixSize);
		ixSize++;
	}
	CFontNode::DelPool();

	u32 dataSize	= texWidth * texHeight;
	u8* data		= SIM_NEW u8[ dataSize ];

	for( s32 n = 0; n < (s32) m_listBitmapChar.size(); n++ )
    {
         m_listBitmapChar[n]->DrawToBitmap( data, texWidth, texHeight );
		 m_listBitmapChar[n]->ReleaseGlyph();
    }

    for( s32 n = 0; n < (s32) m_listBitmapFont.size(); n++ ) {
        m_listBitmapFont[n]->FinishCreating();
    }

	if( !m_useKerning ) {
		ReleaseLibrary();
	}

	m_texture = SIM_NEW CTexture( m_name );
	m_texture->Generate( data
		, texWidth
		, texHeight
		, CTexture::Type::TGA
		, CTexture::Wrap::Clamp
		, CTexture::Filter::Nearest
		, CTexture::Format::Alpha
	);
	
	InitEffect();
	InitMaterial();

	// clean up memory
	SIM_SAFE_DELETE_ARRAY( data );
}

// ----------------------------------------------------------------------//

bool CFontAtlas::BinPack(stl::CPool<CFontNode>* pool, s32 texWidth, s32 texHeight )
{
	CFontNode* treeNode = pool->New(0, 0, texWidth, texHeight );

    for( s32 n = 0; n < (s32)m_listBitmapChar.size(); n++ )
	{
		if( !treeNode->Add( m_listBitmapChar[ n ] ) )  {
			return false;
		}
	}
	return true;
}


// ----------------------------------------------------------------------//
void CFontAtlas::InitEffect()
{
	const char* vsource =
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

		"	gl_Position		= a_PositionL * u_Matrix_WorldViewProjection;"
		"}";

	// ----------------------------------------------------------------------//

	const char *psource =
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

	static const char* attributes[] =
	{
		"a_PositionL",
		"a_TexCoord_0"
	};
	
	m_effect = SIM_NEW CEffect( m_name );

	u32 nAttrib = 2;
	m_effect->InitAttributes(nAttrib);
	for (u32 k = 0; k < nAttrib; k++)
		m_effect->AddAttribute(attributes[k], k);

	static const char* uniforms[] =
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
	m_material->SetDiffuse( &col::Green );
	m_material->SetEffect( m_effect );
}

}; // namespace rnr
}; // namespace sim
