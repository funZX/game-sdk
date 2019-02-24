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

#include <render/font/sim_font_node.h>
#include <render/font/sim_font_char.h>

namespace sim
{
namespace rnr
{

// ----------------------------------------------------------------------//
stl::CPool<CFontNode>*			CFontNode::m_pool = nullptr;
// ----------------------------------------------------------------------//

CFontNode::CFontNode()
{
	m_leaf1 	= nullptr;
	m_leaf2 	= nullptr;
}

// ----------------------------------------------------------------------//

CFontNode::CFontNode( s32 x, s32 y, s32 width, s32 height )
{
	m_x 		= x;
	m_y 		= y;
	m_width 	= width;
	m_height 	= height;
	m_leaf1 	= nullptr;
	m_leaf2 	= nullptr;
}

// ----------------------------------------------------------------------//

void CFontNode::Set( s32 x, s32 y, s32 width, s32 height )
{
	m_x 		= x;
	m_y 		= y;
	m_width 	= width;
	m_height	= height;
	m_leaf1 	= nullptr;
	m_leaf2 	= nullptr;
}

// ----------------------------------------------------------------------//

CFontNode::~CFontNode()
{

}

// ----------------------------------------------------------------------//

stl::CPool<CFontNode>* CFontNode::NewPool()
{
	m_pool = SIM_NEW stl::CPool<CFontNode>();
	return m_pool;
}

// ----------------------------------------------------------------------//

void CFontNode::DelPool()
{
	SIM_SAFE_DELETE(m_pool);
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
		m_leaf1 = m_pool->New();
		m_leaf1->Set(m_x + fontChar->GetWidth(), m_y,
					 m_width - fontChar->GetWidth(),
					 fontChar->GetHeight());

		m_leaf2 = m_pool->New();
		m_leaf2->Set(m_x, m_y + fontChar->GetHeight(),
					 m_width,
		             m_height - fontChar->GetHeight());
	}
	else
	{
		//	m_leaf1 = left (cut in half)
		m_leaf1 = m_pool->New();
		m_leaf1->Set(m_x, m_y + fontChar->GetHeight(),
					 fontChar->GetWidth(),
		             m_height - fontChar->GetHeight());

		// m_leaf2 = right (not cut)
		m_leaf2 = m_pool->New();
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
