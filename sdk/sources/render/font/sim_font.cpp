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

#include <render/font/sim_font.h>
#include <render/font/sim_font_atlas.h>
#include <render/font/sim_font_char.h>

#include <render/sim_material.h>
#include <render/sim_driver.h>

#include <ft2build.h>
#include <freetype/freetype.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CFont::CFont( CFontAtlas* fontAtlas )
{
	m_fontAtlas 	= fontAtlas;
	m_ftFace		= nullptr;
}
// ----------------------------------------------------------------------//

CFont::CFont(const std::string& name, CFontAtlas* fontAtlas) 
	: CFont( fontAtlas )
{
	m_name = name;
}
// ----------------------------------------------------------------------//

CFont::~CFont()
{
	ReleaseFace();
}

// ----------------------------------------------------------------------//

void CFont::ReleaseFace()
{
	if( m_ftFace )
	{
		FT_Done_Face( m_ftFace );
		m_ftFace = nullptr;
	}
}

// ----------------------------------------------------------------------//

s32 CFont::GetWidth( const char* text )
{
	s32 currX 	= 0;
	s32 n 		= 0;
	u8 c;

	FT_Vector kerning;
	FT_UInt ixGlyph;
	FT_UInt ixGlyphPrev = 0;
	bool hasKerning = false;

	if( m_ftFace ) {
		hasKerning = FT_HAS_KERNING( m_ftFace ) != 0;
	}

	while( text[ n ] != 0 )
	{
		c = text[ n ];
		auto bitmap = m_mapBitmapChar.find( c );

		if(bitmap != m_mapBitmapChar.end() )
		{
			if( hasKerning )
			{
				// get kerning
				ixGlyph = FT_Get_Char_Index( m_ftFace, c );
				if( hasKerning && ixGlyphPrev && ixGlyph )
				{
					FT_Get_Kerning( m_ftFace, ixGlyphPrev, ixGlyph, FT_KERNING_DEFAULT, &kerning );
					currX += kerning.x >> 6;
				}
				ixGlyphPrev = ixGlyph;
			}
			currX += bitmap->second->GetXAdvance();
		}
		n++;
	}
	return currX;
}

// ----------------------------------------------------------------------//

CFontChar* CFont::GetChar( s32 charCode )
{
	auto c = m_mapBitmapChar.find( charCode );
	return c == m_mapBitmapChar.end() ? nullptr : c->second;
}

// ----------------------------------------------------------------------//

s32 CFont::GetCharWidth( s32 charCode )
{
	CFontChar *c = GetChar( charCode );

	return c == nullptr ? 0 : c->GetWidth();
}

// ----------------------------------------------------------------------//

s32 CFont::DrawString( CDriver* driver, s32 x, s32 y, const std::string &text, Vec4 color)
{
	u16 c;
	s32 currX 	= x;
	s32 n 		= 0;

	FT_Vector kerning;
	FT_UInt ixGlyph;
	FT_UInt ixGlyphPrev = 0;
	bool hasKerning = false;

	if( m_ftFace ) {
		hasKerning = FT_HAS_KERNING( m_ftFace ) != 0;
	}

	CMaterial* material = m_fontAtlas->GetMaterial();
	material->SetDiffuse(color);

	while( text[ n ] != 0 )
	{
		c = text[ n ];
		auto bitmap	= m_mapBitmapChar.find(c);

		if( bitmap != m_mapBitmapChar.end() )
		{
			if( hasKerning )
			{
				// get kerning
				ixGlyph = FT_Get_Char_Index( m_ftFace, c );
				if( hasKerning && ixGlyphPrev && ixGlyph )
				{
					FT_Get_Kerning( m_ftFace, ixGlyphPrev, ixGlyph, FT_KERNING_DEFAULT, &kerning );
					currX += kerning.x >> 6;
				}
				ixGlyphPrev = ixGlyph;
			}

			bitmap->second->Draw( driver, material, currX, y );
			currX += bitmap->second->GetXAdvance();
		}
		n++;
	}
	return currX;
}

// ----------------------------------------------------------------------//

void CFont::FinishCreating()
{
	bool hasKerning = false;
	if(m_ftFace) {
		hasKerning = FT_HAS_KERNING( m_ftFace ) != 0;
	}
	if(!hasKerning || !m_fontAtlas->GetUseKerning()) {
		ReleaseFace();
	}
}
// ----------------------------------------------------------------------//
bool CFont::Load(io::CMemStream* ms)
{
    return false;
}
// ----------------------------------------------------------------------//
bool CFont::Save(io::CMemStream* ms)
{
    return false;
}
// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
