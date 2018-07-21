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

#include <render/font/sim_font_node.h>
#include <render/font/sim_font_char.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

stl::CPool<CFontNode> CFontNode::m_pool;

CFontNode::CFontNode()
{
	m_leaf1 	= NULL;
	m_leaf2 	= NULL;
}

// ----------------------------------------------------------------------//

CFontNode::CFontNode( s32 x, s32 y, s32 width, s32 height )
{
	m_x 		= x;
	m_y 		= y;
	m_width 	= width;
	m_height 	= height;
	m_leaf1 	= NULL;
	m_leaf2 	= NULL;
}

// ----------------------------------------------------------------------//

void CFontNode::Set( s32 x, s32 y, s32 width, s32 height )
{
	m_x 		= x;
	m_y 		= y;
	m_width 	= width;
	m_height	= height;
	m_leaf1 	= NULL;
	m_leaf2 	= NULL;
}

// ----------------------------------------------------------------------//

CFontNode::~CFontNode()
{
}

// ----------------------------------------------------------------------//

void CFontNode::CreateBranches( CFontChar* fontChar )
{
	s32 dx = m_width - fontChar->GetWidth();
	s32 dy = m_height - fontChar->GetHeight();

	// we split to give one very small leaf and one very big one
	// because it allows more efficent use of space
	// if you don't do this, the bottom right corner never gets used
	if( dx < dy )
	{
		//	split so the top is cut in half and the rest is one big rect below
		m_leaf1 = m_pool.New();
		m_leaf1->Set(m_x + fontChar->GetWidth(), m_y,
					 m_width - fontChar->GetWidth(),
					 fontChar->GetHeight());

		m_leaf2 = m_pool.New();
		m_leaf2->Set(m_x, m_y + fontChar->GetHeight(),
					 m_width,
		             m_height - fontChar->GetHeight());
	}
	else
	{
		//	m_leaf1 = left (cut in half)
		m_leaf1 = m_pool.New();
		m_leaf1->Set(m_x, m_y + fontChar->GetHeight(),
					 fontChar->GetWidth(),
		             m_height - fontChar->GetHeight());

		// m_leaf2 = right (not cut)
		m_leaf2 = m_pool.New();
		m_leaf2->Set(m_x + fontChar->GetWidth(), m_y,
					 m_width - fontChar->GetWidth(),
		             m_height);
	}
}

// ----------------------------------------------------------------------//

bool CFontNode::Add(CFontChar* fontChar)
{
	if( fontChar->IsEmpty() ) {
		return true;
	}

	if( IsEmpty() )
	{
		if( Fits( fontChar ) )
		{
			CreateBranches( fontChar );
			fontChar->SetXY( m_x, m_y );
			return true;
		}
		return false;
	}

	if( m_leaf1->Add( fontChar ) ) {
		return true;
	}

	if( m_leaf2->Add( fontChar ) ) {
		return true;
	}

	return false;
}

// ----------------------------------------------------------------------//

bool CFontNode::Fits( CFontChar* fontChar )
{
	return fontChar->GetWidth() <= m_width && fontChar->GetHeight() <= m_height;
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
