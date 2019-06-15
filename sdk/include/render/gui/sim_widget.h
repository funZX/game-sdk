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

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

class CWidget : public CRect2D
{
public:
	CWidget();
	CWidget( const std::string& name );
	virtual	~CWidget();

	// ------------------------------------------------------------------//
	typedef union
	{
		s32		  m_param;
		struct
		{
			u16 m_loParam;
			u16 m_hiParam;
		};

	} TPointerParam;
	// ------------------------------------------------------------------//

	void							SetParent( CWidget* parent );
	CWidget*						GetParent() { return m_parent; }

	void							SetEnabled(bool enabled) {m_isEnabled = enabled;}
	bool							IsEnabled() {return m_isEnabled;}

	void							SetVisible(bool visible) {m_isVisible = visible;}
	bool							IsVisible() {return m_isVisible;}

	void							AddChild( CWidget *child );
	void							RemChild( CWidget *child );
	void							DelChild( CWidget *child );

	void							RemAllChilds( void );
	void							DelAllChilds( void );

    virtual void					Render( CDriver *driver );

	virtual void					PointerDown( u32 x, u32 y );
	virtual void					PointerDrag( u32 x, u32 y );
	virtual void					PointerUp( u32 x, u32 y );

	void							SetColor( Vec4 color);
	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	CWidget*						m_parent;
	std::map< CWidget*, CWidget* > 	m_childs;

	bool							m_isEnabled;
	bool							m_isVisible;

	Vec4							m_fillcolor;
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __WIDGET_ABSTRACT_H
