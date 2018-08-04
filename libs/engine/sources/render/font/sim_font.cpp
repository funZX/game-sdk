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

#include <render/font/sim_font.h>
#include <render/font/sim_font_atlas.h>
#include <render/font/sim_font_char.h>

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

s32 CFont::DrawString( CDriver* driver, s32 x, s32 y, const std::string &text )
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

			bitmap->second->Draw( driver, m_fontAtlas->GetMaterial(), currX, y );
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
}; // namespace rnr
}; // namespace sim
