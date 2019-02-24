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
