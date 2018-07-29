/* 
*    SIM - OpenGL ES 2.0 portable game engine
*    Copyright (C) 2012  Adrian SIMINCIUC
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

	void							SetColor(const TVec4* color);
	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	CWidget*						m_parent;
	std::map< CWidget*, CWidget* > 	m_childs;

	bool							m_isEnabled;
	bool							m_isVisible;
	bool							m_isFlipped;

	TVec4							m_fillcolor;
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __WIDGET_ABSTRACT_H
