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

#include <render/gui/sim_widget_button.h>

#include <render/sim_sprite_texture.h>
#include <render/sim_batch_2d.h>
#include <render/sim_driver.h>
#include <render/sim_material.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CWidgetButton::CWidgetButton()
	: CWidget()
{
	m_sprite	= nullptr;
	m_frameUp	= -1;
	m_frameDn	= -1;

	m_font		= nullptr;

	m_isPressed = false;
}

// ----------------------------------------------------------------------//

CWidgetButton::CWidgetButton( const std::string& name )
	: CWidgetButton()
{
	m_name = name;
}

// ----------------------------------------------------------------------//

CWidgetButton::~CWidgetButton()
{
}

// ----------------------------------------------------------------------//

void CWidgetButton::Update( u32 dt, void *userData )
{
}

// ----------------------------------------------------------------------//

void CWidgetButton::Render( CDriver *driver )
{
	if( m_sprite != nullptr )
	{
		if (m_isPressed) {
			m_sprite->Render( driver, this, m_frameDn );
		}
		else {
			m_sprite->Render( driver, this, m_frameUp );
		}
	}

	if( !m_string.empty() && ( m_font != nullptr ) )
	{
		s32 x = ( s32 ) Left();
		s32 y = ( s32 ) Top();
		s32 off = IsPressed() ? 0 : 1;

		m_font->DrawString( driver, x + off, y + off, m_string, &col::White );
	}

	CWidget::Render(driver);
}

// ----------------------------------------------------------------------//

void CWidgetButton::PointerDown( u32 x, u32 y)
{
	m_isPressed = true;

	f32 px = (f32) x;
	f32 py = (f32) y;

	if( IsInside( px, py ) )
		CWidget::PointerDown( x, y );
}

// ----------------------------------------------------------------------//

void CWidgetButton::PointerUp( u32 x, u32 y )
{
	m_isPressed = false;

	f32 px = (f32) x;
	f32 py = (f32) y;

	if( IsInside( px, py ) )
		CWidget::PointerUp( x, y );
}

// ----------------------------------------------------------------------//

void CWidgetButton::PointerDrag( u32 x, u32 y )
{
	f32 px = (f32) x;
	f32 py = (f32) y;

	if( IsInside( px, py ) )
		CWidget::PointerDrag( x, y );
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
