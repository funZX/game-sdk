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

#ifndef __SIM_WIDGET_BUTTON_H
#define __SIM_WIDGET_BUTTON_H

#include <render/sim_render.h>
#include <render/gui/sim_widget.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

class CSpriteTexture;
class CFont;
class CDriver;

class CWidgetButton: public CWidget
{
public:
	CWidgetButton();
	CWidgetButton( const std::string& name );
	~CWidgetButton();
	// ------------------------------------------------------------------//
	bool				IsPressed() { return m_isPressed; }

	void				Update( u32 dt, void *userData );
	void				Render( CDriver *driver );

	void				SetString( std::string str ) { m_string.clear(); m_string = str; }
	void				SetSprite( CSpriteTexture *spr, s32 frameUp, s32 frameDn ) { m_sprite = spr; m_frameUp = frameUp; m_frameDn = frameDn; }
	void				SetFont( CFont *font ) { m_font = font ;}

	virtual void		PointerDown( u32 x, u32 y );
	virtual void		PointerDrag( u32 x, u32 y );
	virtual void		PointerUp( u32 x, u32 y );
	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	std::string			m_string;
	CSpriteTexture*		m_sprite;
	s32					m_frameUp, m_frameDn;

	CFont*				m_font;
	bool				m_isPressed;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_WIDGET_BUTTON_H
