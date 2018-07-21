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

#include <render/gui/sim_widget_slider.h>

#include <render/sim_sprite.h>
#include <render/sim_batch_2d.h>
#include <render/sim_driver.h>
#include <render/sim_material.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//
/*
CSlider::CSlider()
{
	m_id = 0;
	m_listener = NULL;
	m_min = 0.0f;
	m_max = 100.0f;
	m_value = 50.0f;
}

CSlider::~CSlider()
{
}

void CSlider::SetRange(f32 min, f32 max)
{
	m_min = min;
	m_max = max;

	SetValue(m_value);	//force validation against bounds
}

void CSlider::SetValue(f32 value)
{
	f32 oldVal = m_value;

	m_value = value;

	if (m_value < m_min)
		m_value = m_min;

	if (m_value > m_max)
		m_value = m_max;

	if (oldVal != m_value)
	{
		if (m_listener)
			m_listener->SliderMoved(m_id, m_value);
	}
}

void CSlider::Update()
{
}

void CSlider::Render()
{
	if (m_sprite)
	{
		CSprite* spr = SPRMGR->GetSprite(m_sprite);

		spr->PaintFrame(m_frameBg, GetX(), GetY());

		s32 p = ValToPos(m_value);

		spr->PaintFrame(m_frameTick, GetX()+p, GetY());
	}
}

void CSlider::Autosize()
{
	if (m_sprite < 0)
		return;

	s32 w, h;

	CSprite* spr = SPRMGR->GetSprite(m_sprite);

	spr->GetFrameSize(m_frameBg, w, h);
	SetSize(w, h);

	m_limit1 = 0;
	m_limit2 = w;
}

s32 CSlider::ValToPos(f32 v)
{
	f32 p = (v - m_min) / (m_max - m_min);

	return p * ActiveWidth() + m_limit1;
}

f32 CSlider::PosToVal(s32 p)
{
	p-=m_limit1;
	f32 v = (f32)p / (f32)ActiveWidth();

	return v * (m_max - m_min) + m_min;
}

s32 CSlider::ActiveWidth()
{
	s32 w = 0, h;

	if (m_sprite >= 0)
	{
		CSprite* spr = SPRMGR->GetSprite(m_sprite);
		spr->GetFrameSize(m_frameTick, w, h);
	}

	return (m_limit2 - m_limit1) - w;
}

void CSlider::OnMouseDown(s32 x, s32 y)
{
	if (m_sprite < 0)
		return;

	s32 p = ValToPos(m_value) + GetX();

	s32 w, h;

	CSprite* spr = SPRMGR->GetSprite(m_sprite);
	spr->GetFrameSize(m_frameBg, w, h);

	if ((x < p) || (x>p+w) || (y<GetY()) || (y>GetY()+h))
		return;

	m_offsetX = x-p;

	CaptureMouse(true);
}

void CSlider::OnMouseDrag(s32 x, s32 y)
{
	f32 v = PosToVal(x - GetX() - m_offsetX);

	SetValue(v);
}

void CSlider::OnMouseUp(s32 x, s32 y)
{
	CaptureMouse(false);
}
*/
// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
