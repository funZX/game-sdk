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

#ifndef __SIM_BONE_HIERARCHY_H
#define __SIM_BONE_HIERARCHY_H

#include <render/sim_render.h>
#include <render/animation/sim_bone.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//
class CBone;
// ----------------------------------------------------------------------//

class CBoneHierarchy
{
	friend class CBoneAnimation;

public:
	CBoneHierarchy( u32 nBones );
	virtual ~CBoneHierarchy();

	// ------------------------------------------------------------------//
	u32			GetBonesCount()			{ return m_nBones; }
	CBone*		GetBone( u32 index )	{ return &m_bones[index]; }
	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	CBone*		m_bones;
	u32			m_nBones;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim

#endif // __SIM_BONE_HIERARCHY_H
