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

#ifndef __SIM_BONE_H
#define __SIM_BONE_H

#include <render/sim_render.h>

#include <math/sim_vec3.h>
#include <math/sim_quat.h>

using namespace sim::mat;

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

class CBone
{
	friend class CBoneAnimation;

public:
	// ------------------------------------------------------------------//

	typedef struct
	{
		TVec3 v;
		TVec3 s;
		TQuat q;

	} TTransform;

	// ------------------------------------------------------------------//

	CBone();
	virtual ~CBone();

	// ------------------------------------------------------------------//
	inline void						SetName( const std::string& name)	{ m_name = name; }
	inline const std::string&		GetName()							{ return m_name; }

	inline void						SetParent( s32 parent )				{ m_parent = parent; }
	inline s32						GetParent()							{ return m_parent; }
	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	std::string						m_name;
	s32								m_iD;
	s32								m_parent;
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim

#endif // __SIM_BONE_H
