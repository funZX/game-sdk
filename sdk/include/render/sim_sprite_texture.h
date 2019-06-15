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

#ifndef __SIM_SPRITE_H
#define __SIM_SPRITE_H

#include <core/sim_interfaces.h>

#include <render/sim_render.h>
#include <render/sim_material.h>
#include <render/sim_texture.h>
#include <render/sim_rect_2d.h>

namespace sim
{
namespace io { class CMemoryStream;  }
namespace rnr
{
// ----------------------------------------------------------------------//

class CRect2D;
class CDriver;

class CSpriteTexture : public CTexture
{
public:
	// ------------------------------------------------------------------//
	CSpriteTexture();
	CSpriteTexture( const std::string& name );
	// ------------------------------------------------------------------//
	void						AddFrame( s32 frame, s32 x, s32 y, s32 w, s32 h );
	void						Render( CDriver *driver, CRect2D *rect, s32 frame );

    virtual bool				Load(io::CMemStream* ms);
    virtual bool				Save(io::CMemStream* ms);
	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	std::map<s32, CRect2D>		m_frames;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_SPRITE_H
