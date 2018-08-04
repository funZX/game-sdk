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

#include <render/gui/sim_widget.h>
#include <render/sim_driver.h>
#include <render/sim_color.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CWidget::CWidget()
	: CRect2D()
{
	m_parent 	= nullptr;

	m_isEnabled = true;
	m_isVisible = true;

	Vec4Copy(&m_fillcolor, &col::Blueish);
}

// ----------------------------------------------------------------------//

CWidget::CWidget(const std::string& name)
	: CWidget()
{
	m_name = name;
}

// ----------------------------------------------------------------------//

CWidget::~CWidget()
{
	DelAllChilds();
}


// ----------------------------------------------------------------------//

void CWidget::SetColor(const TVec4* color)
{
	Vec4Copy( &m_fillcolor, color );
}

// ----------------------------------------------------------------------//
void CWidget::SetParent( CWidget* parent )
{
	if( m_parent != parent )
	{
		m_parent->RemChild( this );
		m_parent = parent;
	}
}

// ----------------------------------------------------------------------//

void CWidget::AddChild( CWidget *child )
{
	if( child != nullptr )
	{
		auto c = m_childs.find(child);

		if( c != m_childs.end() )
		{
			m_childs[ child ] = child;

			child->SetParent( this );
		}
	}
}

// ----------------------------------------------------------------------//

void CWidget::RemChild( CWidget *child )
{
	auto c = m_childs.find(child);

	if( c != m_childs.end() )
	{
		c->second->SetParent( nullptr );

		m_childs.erase( c );
	}
}

// ----------------------------------------------------------------------//

void CWidget::DelChild( CWidget *child )
{
	auto c = m_childs.find( child );

	if( c != m_childs.end() )
	{
		SIM_SAFE_DELETE( c->second );

		m_childs.erase( c );
	}
}

// ----------------------------------------------------------------------//

void CWidget::DelAllChilds( void )
{
	auto c = m_childs.begin();

	while( c != m_childs.end() )
	{
		SIM_SAFE_DELETE( c->second );
		++c;
	}

	m_childs.clear();
}

// ----------------------------------------------------------------------//

void CWidget::RemAllChilds( void )
{
	m_childs.clear();
}

// ----------------------------------------------------------------------//

void CWidget::PointerDown( u32 x, u32 y )
{
	TPointerParam p;

	p.m_loParam = x;
	p.m_hiParam = y;
}

// ----------------------------------------------------------------------//

void CWidget::PointerDrag( u32 x, u32 y )
{
	TPointerParam p;

	p.m_loParam = x;
	p.m_hiParam = y;
}

// ----------------------------------------------------------------------//

void CWidget::PointerUp( u32 x, u32 y )
{
	TPointerParam p;

	p.m_loParam = x;
	p.m_hiParam = y;
}
// ----------------------------------------------------------------------//

void CWidget::Render( CDriver *driver )
{
	auto c = m_childs.begin();

	while( c != m_childs.end() )
		c->second->Render( driver );

	CRect2D::Render( driver, m_texRect );
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim

