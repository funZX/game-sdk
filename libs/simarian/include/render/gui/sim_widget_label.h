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

#ifndef __SIM_WIDGET_LABEL_H
#define __SIM_WIDGET_LABEL_H

#include <render/sim_render.h>
#include <render/sim_widget.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

class CSprite;
class CFont;

class CWidgetLabel: public CWidget
{
public:

	CWidgetLabel( const std::string& name );
	virtual ~CWidgetLabel();
	// ------------------------------------------------------------------//
	void				Update( u32 dt, void *userData );
	void				Render( CDriver *driver );

	void				SetString( std::string str ) { m_string.clear(); m_string = str; }
	void				SetFont( CFont *font ) { m_font = font; }
	void				SetSprite( CSprite *sprite, s32 frame ) { m_sprite = sprite; m_frame = frame; }

	virtual void		PointerDown( u32 x, u32 y );
	virtual void		PointerDrag( u32 x, u32 y );
	virtual void		PointerUp( u32 x, u32 y );
	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	std::string 		m_string;
	CFont*				m_font;
	CSprite*			m_sprite;
	s32 				m_frame;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_WIDGET_LABEL_H
