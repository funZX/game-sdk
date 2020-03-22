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

#include <core/sim_core.h>
#include <core/sim_singleton.h>
#include <core/sim_interfaces.h>

#include <render/sim_render.h>
#include <render/sim_rect_2d.h>

#include <imgui.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

class CCamera;
class CFontAtlas;
class CVertexGroup;
class CVertexSource;

class CCanvas : public CRect2D
{
public:
	CCanvas( CFontAtlas *fontAtlas );
	CCanvas( const std::string& name, CFontAtlas* fontAtlas );
	virtual ~CCanvas();
	// ------------------------------------------------------------------//
    void				        Resize( f32 width, f32 height );

	virtual void		        Update( f32 dt, void *userData );
	virtual void		        Render( CDriver* driver );

	virtual void		        PointerDown( u32 x, u32 y );
	virtual void		        PointerDrag( u32 x, u32 y );
	virtual void		        PointerUp( u32 x, u32 y );

	virtual void		        ClearEvents();

public: // Signals
    // ------------------------------------------------------------------//
    sigcxx::Signal<CCanvas*>	OnGui;

	// ------------------------------------------------------------------//
protected:
	static void*				ImGuiAlloc(size_t sz, void* userData);
	static void					ImGuiDelloc(void* ptr, void* userData);

    CFontAtlas*                 m_fontAtlas;

    CVertexSource*              m_vertexSource;
	CVertexGroup*				m_vertexGroup;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_CANVAS_H

