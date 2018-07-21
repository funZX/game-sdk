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

#ifndef __SIM_WIDGET_SLIDER_H
#define __SIM_WIDGET_SLIDER_H

#include <render/sim_render.h>
#include <render/sim_widget.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//
/*
class CSlider: public CWidget
{
public:
	CSlider();
	virtual ~CSlider();
	// ------------------------------------------------------------------//
	void 				SetRange(f32 min, f32 max);
	void 				SetValue(f32 value);
	void 				SetLimits(s32 l1, s32 l2) {m_limit1 = l1; m_limit2 = l2;}

	f32 				GetValue() { return m_value; }
	virtual void 		Update();
	virtual void 		Render();

	void 				SetSprite( s32 sprite, s32 frameBg, s32 frameTick ) {m_sprite = sprite; m_frameBg = frameBg; m_frameTick = frameTick;}

	void 				SetListener(ISliderListener* listener) {m_listener = listener;}

	void 				SetID( s32 id ) { m_id = id; }

	virtual void 		Autosize();

	virtual void		PointerDown( u16 x, u16 y );
	virtual void		PointerDrag( u16 x, u16 y );
	virtual void		PointerUp( u16 x, u16 y );
	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	s32 				ValToPos(f32 v);
	f32 				PosToVal(s32 p);
	s32 				ActiveWidth();
	// ------------------------------------------------------------------//
	s32 				m_id;
	ISliderListener* 	m_listener;

	f32 				m_min, m_max;
	f32 				m_value;

	s32 				m_sprite;
	s32 				m_frameBg, m_frameTick;

	s32 				m_limit1, m_limit2;

	s32 				m_offsetX;
	// ------------------------------------------------------------------//
};
*/
// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_WIDGET_SLIDER_H
