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

#include <render/sim_sprite.h>
#include <render/sim_driver.h>
#include <render/sim_material.h>
#include <render/sim_widget.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CSprite::CSprite()
{
	m_material = NULL;
}

// ----------------------------------------------------------------------//

void CSprite::AddModule( s32 modId, s32 x, s32 y, s32 w, s32 h  )
{
	TModule m;

	f32 rw = 1.0f / GetWidth();
	f32 rh = 1.0f / GetHeight();

	m.m_texCoords[ 0 ] = ( x + 0 ) * rw;		m.m_texCoords[ 1 ] = ( y + 0 ) * rh;
	m.m_texCoords[ 2 ] = ( x + w ) * rw;		m.m_texCoords[ 3 ] = ( y + 0 ) * rh;
	m.m_texCoords[ 4 ] = ( x + 0 ) * rw;		m.m_texCoords[ 5 ] = ( y + h ) * rh;
	m.m_texCoords[ 6 ] = ( x + w ) * rw;		m.m_texCoords[ 7 ] = ( y + h ) * rh;

	m_modules[ modId ] = m;
}

// ----------------------------------------------------------------------//

void CSprite::RenderModule( CDriver *driver, CWidget *widget, s32 modId )
{
	std::map<s32, TModule>::iterator m = m_modules.find( modId );

	if( m != m_modules.end() )
	{
	    CRect2D *rect2D = ( CRect2D* ) widget;

		rect2D->SetMaterial( m_material );
		rect2D->Render( driver, (f32*) &m->second, NULL );
	}
}

// ----------------------------------------------------------------------//

void CSprite::Load( const std::string &file )
{

}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
