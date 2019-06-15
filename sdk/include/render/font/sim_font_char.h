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
