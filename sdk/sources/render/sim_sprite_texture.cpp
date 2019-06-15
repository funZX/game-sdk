/*
*
*    MIT License
*
*    SIM - OpenGL ES 2.0 portable game engine
*    Copyright (C) 2012  Adrian SIMINCIUC
*
*    Permission is hereby granted, free of charge, to any person obtaining a copy
*    of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in all
*    copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*    SOFTWARE.
*/

#include <render/sim_sprite_texture.h>
#include <render/sim_driver.h>
#include <render/sim_material.h>

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

bool CSpriteTexture::Load(io::CMemStream* ms)
{
    return false;
}

// ----------------------------------------------------------------------//

bool CSpriteTexture::Save(io::CMemStream* ms)
{
    return false;
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
