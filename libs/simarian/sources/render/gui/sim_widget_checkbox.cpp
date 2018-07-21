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

#include <render/gui/sim_widget_checkbox.h>

#include <render/sim_batch_2d.h>
#include <render/sim_driver.h>
#include <render/sim_material.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//
/*
CCheckbox::CCheckbox()
{
	m_id = 0;
	m_listener = NULL;

	m_text[0] = 0;
	m_sprite = -1;
	m_frameOff = 0;
	m_frameOn = 0;

	m_font = -1;

	m_checked = false;
}

CCheckbox::~CCheckbox()
{
}

void CCheckbox::Update()
{
}

void CCheckbox::Render()
{
	s32 w = 0,h = 0;

	if (m_sprite >= 0)
	{
		CSprite* spr = SPRMGR->GetSprite(m_sprite);

		if (m_checked)
			spr->PaintFrame(m_frameOn, GetX(), GetY());
		else
			spr->PaintFrame(m_frameOff, GetX(), GetY());

		spr->GetFrameSize(m_frameOn, w, h);
	}

	if (m_text[0] && (m_font >= 0))
	{
		CSprite* fnt = SPRMGR->GetSprite(m_font);

		fnt->DrawText(m_text, GetX()+w+5, GetYc(), SPR_ALIGN_LEFT | SPR_ALIGN_VCENTER);
	}
}

void CCheckbox::Uncheck(CCheckbox* sender)
{
	if (sender != this)
	{
		m_checked = false;
		m_radioLink->Uncheck(sender);
	}
}

void CCheckbox::OnMouseDown(s32 x, s32 y)
{
	if (m_radioLink)
	{
		if (m_checked)
			return;

		m_checked = true;
		m_radioLink->Uncheck(this);
	}
	else
	{
		m_checked = !m_checked;
	}

	if (m_listener)
		m_listener->BoxChecked(m_id, m_checked);

	PlayRelativeSource(menuClickSndRel, false);
}

void CCheckbox::Autosize()
{
	s32 tw, th;

	tw = th = 0;

	if (m_sprite >= 0)
	{
		s32 w, h;

		CSprite* spr = SPRMGR->GetSprite(m_sprite);

		spr->GetFrameSize(m_frameOff, w, h);
		tw = w;
		th = h;
	}

	if (m_text[0] && (m_font >= 0))
	{
		CSprite* fnt = SPRMGR->GetSprite(m_font);

		s32 w, h;

		fnt->GetTextSize(m_text, w, h);

		tw += w + 5;
	}

	SetSize(tw, th);
}
*/
// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
