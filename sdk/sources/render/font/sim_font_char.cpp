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

#include <render/font/sim_font_char.h>
#include <render/sim_vertex_source.h>
#include <render/sim_vertex_group.h>
#include <render/sim_effect.h>
#include <render/sim_material.h>
#include <render/sim_rect_2d.h>
#include <freetype/ftglyph.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CFontChar::CFontChar()
{
	m_x 			= 0;
	m_y 			= 0;
	m_width 		= 0;
	m_height 		= 0;
	m_offsetX 		= 0;
	m_offsetY 		= 0;
	m_advanceX 		= 0;
	m_glyph 		= nullptr;
}

// ----------------------------------------------------------------------//

CFontChar::~CFontChar()
{
}

// ----------------------------------------------------------------------//

void CFontChar::InitTexCoords( s32 texWidth, s32 texHeight )
{
	m_texRect.Bound(
		(f32)m_x / (f32)texWidth,
		(f32)m_y / (f32)texHeight,
		(f32)m_width / (f32)texWidth,
		(f32)m_height / (f32)texHeight
	);
}

// ----------------------------------------------------------------------//

void CFontChar::Draw( CDriver *driver, CMaterial* material, s32 x, s32 y )
{
	if( IsEmpty() )
		return;

	CRect2D r;
	r.Bound( (f32)x, (f32)y, (f32)m_width, (f32)m_height );
	r.Move( (f32)m_offsetX, (f32)m_offsetY );
	r.SetMaterial( material );

	r.Render( driver, &m_texRect );
}

// ----------------------------------------------------------------------//

void CFontChar::DrawToBitmap( u8* pData, s32 texWidth, s32 texHeight )
{
	if( !IsEmpty() )
	{
        InitTexCoords( texWidth, texHeight );

        // Convert The Glyph To A Bitmap.
        FT_Glyph_To_Bitmap( &m_glyph, FT_RENDER_MODE_NORMAL, 0, 1 );
        FT_BitmapGlyph bitmap_glyph = ( FT_BitmapGlyph ) m_glyph;

        // This Reference Will Make Accessing The Bitmap Easier.
        FT_Bitmap& bitmap = bitmap_glyph->bitmap;

        SIM_ASSERT( bitmap.width == m_width );
        SIM_ASSERT( bitmap.rows == m_height );

        for( s32 y = 0; y < bitmap.rows; y++ )
        {
            for( s32 x = 0; x < bitmap.width; x++ )
            {
                s32 index = ( m_y + y ) * texWidth + m_x + x;
                pData[ index ] = bitmap.buffer[ y * bitmap.width + x ];
            }
        }
	}
}

// ----------------------------------------------------------------------//

void CFontChar::ReleaseGlyph()
{
	if( m_glyph ) {
	    FT_Done_Glyph( m_glyph );
	}
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
