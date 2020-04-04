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
#include <core/sim_list.h>
#include <core/sim_singleton.h>
#include <core/sim_interfaces.h>

#include <render/sim_render.h>
#include <render/sim_rect_2d.h>

#include <imgui.h>
#include <zpl.h>

namespace sim
{
// ----------------------------------------------------------------------//
class CEngine;
// ----------------------------------------------------------------------//
namespace ren
{
// ----------------------------------------------------------------------//

class CCamera;
class CFontAtlas;
class CVertexGroup;
class CVertexSource;

class CCanvas : public CRect2D
{
	friend class sim::CEngine;

public:
    enum class EvType : u32
    {
        Mouse,
        Key,
    };

    typedef struct _TEvent
    {
        CCanvas*        evCanvas;
        EvType		    evType;

        struct {
            int			button;
            bool        down;
            int			x;
            int			y;
        } evMouse;

        struct {
            int			code;
            bool        down;
        } evKey;

        struct {
            bool		shift;
            bool		ctrl;
            bool		alt;
        } evMode;

        _TEvent()
        {
            evCanvas = nullptr;
            evType = EvType::Key;
            SIM_MEMSET(&evMouse, 0, sizeof(evMouse));
            SIM_MEMSET(&evKey, 0, sizeof(evKey));
            SIM_MEMSET(&evMode, 0, sizeof(evMode));
        }
    } TEvent;

	CCanvas( CFontAtlas *fontAtlas );
	CCanvas( const std::string& name, CFontAtlas* fontAtlas );
	virtual ~CCanvas();
	// ------------------------------------------------------------------//
    void				        Resize( f32 width, f32 height );

	virtual void		        Update( f32 dt, void *userData );
	virtual void		        Render( CDriver* driver );

	virtual void		        Reset();

public: // Signals
    // ------------------------------------------------------------------//
    sigcxx::Signal<CCanvas*>					OnGui;
    sigcxx::Signal<CCanvas*, TEvent*>			OnEvent;

	// ------------------------------------------------------------------//
protected:
	static void*				ImGuiAlloc(size_t sz, void* userData);
	static void					ImGuiDelloc(void* ptr, void* userData);

    void						MouseDown( int button );
    void						MouseUp( int button );
    void						MouseMove( f32 x, f32 y );
    void						KeyDown( int Key, bool KeyShift, bool KeyCtrl, bool KeyAlt );
	void						KeyUp( int Key, bool KeyShift, bool KeyCtrl, bool KeyAlt );

    CFontAtlas*                 m_fontAtlas;

    CVertexSource*              m_vertexSource;
	CVertexGroup*				m_vertexGroup;

	stl::CList<TEvent>			m_eventList;
    ImVec2                      m_mousePos;
    bool                        m_shiftDown;
    bool                        m_ctrlDown;
    bool                        m_altDown;
    // ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace ren
}; // namespace sim
#endif // __SIM_CANVAS_H

