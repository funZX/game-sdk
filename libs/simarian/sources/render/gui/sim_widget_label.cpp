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

#include <render/sim_batch_2d.h>
#include <render/sim_driver.h>
#include <render/sim_material.h>
#include <render/sim_sprite.h>

#include <render/font/sim_font.h>

#include <render/gui/sim_widget_label.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CWidgetLabel::CWidgetLabel( const std::string& name )
	:CWidget( name )
{
	m_font			= NULL;
	m_sprite		= NULL;
	m_frame			= 0;

	SetEnabled( false );
}

// ----------------------------------------------------------------------//

CWidgetLabel::~CWidgetLabel()
{
}

// ----------------------------------------------------------------------//

void CWidgetLabel::Update( u32 dt, void *userData )
{
}

// ----------------------------------------------------------------------//

void CWidgetLabel::Render( CDriver *driver )
{
	if( m_sprite != NULL ) {
		m_sprite->RenderModule( driver, this, m_frame );
	}

	if( !m_string.empty() && m_font != NULL )
	{
		s32 x = ( s32 ) Left();
		s32 y = ( s32 ) Top();

		m_font->DrawString( driver, x, y, m_string );
	}
}

// ----------------------------------------------------------------------//

void CWidgetLabel::PointerDown( u32 x, u32 y )
{
	f32 px = (f32) x;
	f32 py = (f32) y;

	if( IsInside( px, py ) ) {
		CWidget::PointerDown( x, y );
	}
}

// ----------------------------------------------------------------------//

void CWidgetLabel::PointerUp( u32 x, u32 y )
{
	f32 px = (f32) x;
	f32 py = (f32) y;

	if( IsInside( px, py ) ) {
		CWidget::PointerUp( x, y );
	}
}

// ----------------------------------------------------------------------//

void CWidgetLabel::PointerDrag( u32 x, u32 y )
{
	f32 px = (f32) x;
	f32 py = (f32) y;

	if( IsInside( px, py ) ) {
		CWidget::PointerDrag( x, y );
	}
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
