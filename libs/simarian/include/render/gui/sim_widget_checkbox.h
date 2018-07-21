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

#ifndef __SIM_WIDGET_CHECKBOX_H
#define __SIM_WIDGET_CHECKBOX_H

#include <render/sim_render.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//
/*
class CCheckbox: public CWidget
{
public:
	CCheckbox();
	virtual ~CCheckbox();
	// ------------------------------------------------------------------//
	virtual void 							Update();
	virtual void 							Render();

	void 									SetListener(ICheckListener* listener) {m_listener = listener;}

	void 									SetID(s32 id) {m_id = id;}
	void 									SetText(const char* text) {strcpy(m_text, text);}
	void 									SetSprite(s32 spr, s32 frameOn, s32 frameOff) {m_sprite = spr; m_frameOn = frameOn; m_frameOff = frameOff;}
	void 									SetFont(s32 font) {m_font = font;}
	void 									SetChecked(bool checked) {m_checked = checked;}
	bool 									IsChecked() {return m_checked;}

	void 									SetRadioLink( CCheckbox* nextLink ) { m_radioLink = nextLink; }

	virtual void							PointerDown( u16 x, u16 y );
	virtual void							PointerDrag( u16 x, u16 y );
	virtual void							PointerUp( u16 x, u16 y );
	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	void 									Uncheck(CCheckbox* sender);

	s32 									m_id;
	ICheckListener* 						m_listener;

	std::string 							m_string;
	s32 									m_sprite;
	s32 									m_frameOff, m_frameOn;

	s32 									m_font;
	bool 									m_isChecked;
	CCheckbox* 								m_radioLink;
	// ------------------------------------------------------------------//
};
*/

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_WIDGET_CHECKBOX_H
