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

#ifndef __SIM_CANVAS_H
#define __SIM_CANVAS_H

#include <core/sim_singleton.h>
#include <core/sim_interfaces.h>

#include <render/sim_render.h>
#include <render/gui/sim_widget.h>

#include <math/sim_vec2.h>
#include <math/sim_vec3.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

class CCamera;

class CCanvas : public CWidget
{
public:
	CCanvas();
	CCanvas( const std::string& name );
	virtual ~CCanvas();
	// ------------------------------------------------------------------//
	void				Resize( f32 width, f32 height );
	virtual void		Update( f32 dt, void *userData );
	virtual void		Render( CDriver* driver );

	virtual void		PointerDown( u32 x, u32 y );
	virtual void		PointerDrag( u32 x, u32 y );
	virtual void		PointerUp( u32 x, u32 y );

	virtual void		ClearEvents();
	// ------------------------------------------------------------------//
protected:

	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_CANVAS_H

