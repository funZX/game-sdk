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

		m_font->DrawString( driver, x, y, m_string );
	}

	CWidget::Render(driver);
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
