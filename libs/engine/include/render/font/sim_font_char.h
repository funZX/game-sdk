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

#ifndef __SIM_FONT_CHAR_H
#define __SIM_FONT_CHAR_H

#include <render/sim_render.h>
#include <render/sim_rect_2d.h>

struct FT_GlyphRec_;

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

class CEffect;
class CMaterial;
class CDriver;

class CFontChar
{
public:
	CFontChar();
	virtual ~CFontChar();
	// ------------------------------------------------------------------//
	inline void 			SetXY(s32 x, s32 y)
							{ m_x = x; m_y = y; }

	inline void 			SetSize(s32 width, s32 height)
							{ m_width = width; m_height = height; }

	inline void 			SetOffsets(s32 xOffset, s32 yOffset)
							{ m_offsetX = xOffset; m_offsetY = yOffset; }

	s32						GetX2() const { return m_x + m_width; }
	s32						GetY2() const { return m_x + m_width; }
	s32						GetXAdvance() const { return m_advanceX; }
	void					SetXAdvance(s32 xAdvance) { m_advanceX = xAdvance; }
	s32						GetHeight() const { return m_height; }
	s32						GetTotalHeight() const { return m_offsetY + m_height; }
	s32						GetWidth() const { return m_width; }
	s32						GetYOffset() const { return m_offsetY; }
	void					ReduceYOffset(s32 amount) { m_offsetY -= amount; }
	void					AddKerning(CFontChar* bitmapChar, s32 amount);
	bool					IsLoaded() const { return m_x >= 0; }
	s32						GetNumPixels() const { return m_width * m_height; }
	void					SetGlyph(struct FT_GlyphRec_* glyph) { m_glyph = glyph; }
	void					DrawToBitmap(u8* pData, s32 texWidth, s32 texHeight);
	bool					IsEmpty() const { return m_width == 0 || m_height == 0; }
	void					ReleaseGlyph();
	void					InitTexCoords(s32 texWidth, s32 texHeight);

	void					Draw( CDriver* driver, CMaterial* material, s32 x, s32 y );
	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	s32						m_x;
	s32						m_y;
	s32						m_width;
	s32						m_height;
	s32						m_offsetX;
	s32						m_offsetY;
	s32						m_advanceX;

	CRect2D					m_texRect;

	struct FT_GlyphRec_*	m_glyph;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_FONT_CHAR_H
