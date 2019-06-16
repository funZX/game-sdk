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

#ifndef __SIM_DRAWABLE_H
#define __SIM_DRAWABLE_H

#include <core/sim_core.h>
#include <render/sim_render.h>
#include <render/sim_rect_2d.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//
class CCamera;
class CRenderTexture;
class CDriver;
// ----------------------------------------------------------------------//

class CDrawable: public CRect2D
{
public:
	// ------------------------------------------------------------------//
	CDrawable();
	CDrawable( const std::string& name );
	virtual ~CDrawable();
	// ------------------------------------------------------------------//
	void							Draw( CDriver *driver );
	virtual void					Render( CDriver *driver );

    inline Vec4				        GetColor();
    inline void						SetColor( Vec4 ambient );

    inline void                     SetEffect( CEffect* effect );
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

    Vec4                            m_color;
    CEffect*                        m_effect;
	// ------------------------------------------------------------------//
};

inline Vec4 CDrawable::GetColor()
{
    return m_color;
}

inline void CDrawable::SetColor( Vec4 color )
{
    m_color = color;
}

inline void CDrawable::SetEffect( CEffect* effect )
{
    m_effect = effect;
}
// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_DRAWABLE_H
