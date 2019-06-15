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

#ifndef __SIM_WIDGET_H
#define __SIM_WIDGET_H

#include <core/sim_interfaces.h>
#include <render/sim_render.h>
#include <render/sim_rect_2d.h>

#include <imgui.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//
class CFontAtlas;
// ----------------------------------------------------------------------//

class CWidget : public CRect2D
{
public:
	CWidget( CFontAtlas* atlas );
	CWidget( const std::string& name, CFontAtlas* atlas);
	virtual	~CWidget();

	// ------------------------------------------------------------------//
    void                        DrawString(CDriver* driver, s32 x, s32 y, const std::string& text, Vec4 color);

	// ------------------------------------------------------------------//
    virtual void				Update( f32 dt, void* userData );
    virtual void				Render( CDriver* driver );
	// ------------------------------------------------------------------//

protected:
    ImGuiContext*               m_imContext;
    ImGuiStyle*                 m_imStyle;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __WIDGET_ABSTRACT_H
