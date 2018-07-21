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

#ifndef __SIM_FONT_NODE_H
#define __SIM_FONT_NODE_H

#include <core/sim_pool.h>
#include <render/sim_render.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

class CFontChar;

class CFontNode
{
public:
	CFontNode();
	CFontNode( s32 x, s32 y, s32 width, s32 height );
	virtual ~CFontNode();
	// ------------------------------------------------------------------//
	void									Set( s32 x, s32 y, s32 width, s32 height );
	bool									Add( CFontChar* fontChar );
	bool									IsEmpty() const { return m_leaf1 == NULL && m_leaf2 == NULL; }

	static stl::CPool<CFontNode>&			GetPool()  { return m_pool; }
	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	void									CreateBranches( CFontChar* fontChar );
	bool									Fits( CFontChar* fontChar );
	// ------------------------------------------------------------------//
	s32										m_x;
	s32										m_y;
	s32										m_width;
	s32										m_height;
	CFontNode*								m_leaf1;
	CFontNode*								m_leaf2;

	static stl::CPool<CFontNode> 			m_pool;
	// ---------------------------------	---------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_FONT_NODE_H
