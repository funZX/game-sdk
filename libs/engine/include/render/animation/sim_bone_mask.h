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

#ifndef __SIM_BONE_MASK_H
#define __SIM_BONE_MASK_H

#include <render/sim_render.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//
class CBoneHierarchy;
// ----------------------------------------------------------------------//
class CBoneMask
{
public:
	// ------------------------------------------------------------------//
	CBoneMask( CBoneHierarchy* hierarchy, u32 mask );
	virtual ~CBoneMask();
	// ------------------------------------------------------------------//
	bool		IsSet( u32 value ) { return ( 0 != ( m_mask & value ) ); }
	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	u32			m_mask;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim

#endif // __SIM_BONE_MASK_H