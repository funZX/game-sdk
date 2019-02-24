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

#include <render/sim_batch_2d.h>
#include <render/sim_driver.h>
#include <render/sim_material.h>
#include <render/sim_sprite_texture.h>

#include <render/font/sim_font.h>

#include <render/gui/sim_widget_label.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CWidgetLabel::CWidgetLabel()
{
	m_font			= nullptr;
	m_sprite		= nullptr;
	m_frame			= 0;

	SetEnabled( false );
}
// ----------------------------------------------------------------------//

CWidgetLabel::CWidgetLabel(const std::string& name)
	:CWidgetLabel()
{
	m_name = name;
}
// ----------------------------------------------------------------------//

CWidgetLabel::~CWidgetLabel()
{
}

// ----------------------------------------------------------------------//

void CWidgetLabel::Render( CDriver *driver )
{
	if( m_sprite != nullptr ) {
		m_sprite->Render( driver, this, m_frame );
	}

	if( !m_string.empty() && m_font != nullptr )
	{
		s32 x = ( s32 ) Left();
		s32 y = ( s32 ) Top();

		m_font->DrawString( driver, x, y, m_string, &col::White);
	}

	CWidget::Render(driver);
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
