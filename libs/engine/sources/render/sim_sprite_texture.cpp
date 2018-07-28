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

#include <render/sim_sprite_texture.h>
#include <render/sim_driver.h>
#include <render/sim_material.h>
#include <render/gui/sim_widget.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CSpriteTexture::CSpriteTexture()
	: CTexture()
{
}

// ----------------------------------------------------------------------//

CSpriteTexture::CSpriteTexture( const std::string& name )
	: CSpriteTexture()
{
	m_name = name;
}

// ----------------------------------------------------------------------//

void CSpriteTexture::AddFrame( s32 frame, s32 x, s32 y, s32 w, s32 h  )
{
	CRect2D m;

	f32 rw = 1.0f / GetWidth();
	f32 rh = 1.0f / GetHeight();

	m.Bound(
		x * rw,
		y * rw,
		w * rw,
		h * rh
	);

	m_frames[frame] = m;
}

// ----------------------------------------------------------------------//

void CSpriteTexture::Render( CDriver *driver, CRect2D *rect, s32 frame )
{
	std::map<s32, CRect2D>::iterator m = m_frames.find(frame);

	SIM_ASSERT( this == rect->GetMaterial()->GetTexture( 0 ) );

	if (m != m_frames.end())
		rect->Render(driver, &m->second);
}

// ----------------------------------------------------------------------//

void CSpriteTexture::Load(io::CMemStream* ms)
{

}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
