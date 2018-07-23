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

CFontChar::CFontChar( const std::string& name )
{
	m_name			= name;

	m_x 			= 0;
	m_y 			= 0;
	m_width 		= 0;
	m_height 		= 0;
	m_offsetX 		= 0;
	m_offsetY 		= 0;
	m_advanceX 		= 0;
	m_glyph 		= NULL;
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

	CRect2D r( m_name );
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
