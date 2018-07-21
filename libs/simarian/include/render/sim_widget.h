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

	void						SetID( s32 wID ) { m_iD = wID; }
	s32							GetID() { return m_iD; }

	void						SetParent( CWidget* parent );
	CWidget*					GetParent() { return m_parent; }

	void						SetFocus( bool focus ) { m_isFocused = focus; }
	bool						IsFocused() { return m_isFocused; }

	void						SetEnabled(bool enabled) {m_isEnabled = enabled;}
	bool						IsEnabled() {return m_isEnabled;}

	void						SetVisible(bool visible) {m_isVisible = visible;}
	bool						IsVisible() {return m_isVisible;}

	inline TMatrix4*			GetTransform() { return &m_transform; }
	inline void					SetTransform( TMatrix4* m) { Matrix4Copy(&m_transform, m ); }

	void						AddChild( CWidget *child );
	void						RemoveChild( s32 wID );
	void						DeleteChild( s32 wiD );

	void						RemoveAllChilds( void );
	void						DeleteAllChilds( void );

    virtual void				Render( CDriver *driver, TMatrix4* tranform );

	virtual void				PointerDown( u32 x, u32 y ) = 0;
	virtual void				PointerDrag( u32 x, u32 y ) = 0;
	virtual void				PointerUp( u32 x, u32 y )   = 0;

	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	CWidget*					m_parent;
	std::map< s32, CWidget* > 	m_childs;

	s32							m_iD;

	bool						m_isFocused;
	bool						m_isEnabled;
	bool						m_isVisible;

	TMatrix4					m_transform;

	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __WIDGET_ABSTRACT_H
