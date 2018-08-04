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

#include <sim_engine.h>
#include <render/sim_driver.h>
#include <render/sim_sprite_texture.h>
#include <render/gui/sim_widget_sprite.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CWidgetSprite::CWidgetSprite()
	:CWidget()
{
	m_sprite = nullptr;
}

// ----------------------------------------------------------------------//

CWidgetSprite::CWidgetSprite( const std::string& name )
	:CWidgetSprite()
{
	m_name = name;
}
// ----------------------------------------------------------------------//

CWidgetSprite::~CWidgetSprite()
{
	SIM_SAFE_DELETE( m_sprite );
}

// ----------------------------------------------------------------------//
void CWidgetSprite::Render( CDriver *driver )
{
	CWidget::Render( driver );
}
// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
