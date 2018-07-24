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

#ifndef __SIM_WIDGET_BUTTON_H
#define __SIM_WIDGET_BUTTON_H

#include <render/sim_render.h>
#include <render/sim_widget.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

class CSprite;
class CFont;
class CDriver;

class CWidgetButton: public CWidget
{
public:
	CWidgetButton();
	CWidgetButton( const std::string& name );
	~CWidgetButton();
	// ------------------------------------------------------------------//
	bool				IsPressed() { return m_isPressed; }

	void				Update( u32 dt, void *userData );
	void				Render( CDriver *driver );

	void				SetString( std::string str ) { m_string.clear(); m_string = str; }
	void				SetSprite( CSprite *spr, s32 frameUp, s32 frameDn ) { m_sprite = spr; m_frameUp = frameUp; m_frameDn = frameDn; }
	void				SetFont( CFont *font ) { m_font = font ;}

	virtual void		PointerDown( u32 x, u32 y );
	virtual void		PointerDrag( u32 x, u32 y );
	virtual void		PointerUp( u32 x, u32 y );
	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	std::string		m_string;
	CSprite*			m_sprite;
	s32					m_frameUp, m_frameDn;

	CFont*				m_font;
	bool				m_isPressed;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_WIDGET_BUTTON_H
