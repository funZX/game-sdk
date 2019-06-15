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

#ifndef __SIM_WIDGET_RENDERTEXTURE_H
#define __SIM_WIDGET_RENDERTEXTURE_H

#include <core/sim_core.h>

#include <render/sim_render.h>
#include <render/gui/sim_widget.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//
class CCamera;
class CRenderTexture;
class CDriver;
// ----------------------------------------------------------------------//

class CWidgetDrawable: public CWidget
{
public:
	// ------------------------------------------------------------------//
	CWidgetDrawable();
	CWidgetDrawable( const std::string& name );
	virtual ~CWidgetDrawable();
	// ------------------------------------------------------------------//
	void							Draw( CDriver *driver );	
	CRenderTexture*					GetTexture() { return m_rendertexture; }
	virtual void					Render(CDriver *driver);
	// ------------------------------------------------------------------//
public: // Signals
	// ------------------------------------------------------------------//
	sigcxx::Signal<CDriver*>		OnDraw;
	// ------------------------------------------------------------------//

protected:
	// ------------------------------------------------------------------//
	virtual void					OnResize();
	// ------------------------------------------------------------------//
	CRenderTexture*					m_rendertexture;
	CCamera*						m_camera;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_WIDGET_RENDERTEXTURE_H
