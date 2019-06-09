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

#include <render/gui/sim_widget.h>
#include <render/sim_driver.h>

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

	m_fillcolor = col::Blueish;
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

void CWidget::SetColor(Vec4 color)
{
	m_fillcolor = color;
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

