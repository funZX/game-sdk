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

#include <core/io/sim_mem_stream.h>
#include <render/sim_batch_2d.h>
#include <render/sim_material.h>

#include <render/animation/sim_bone.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CBone::CBone()
{
	m_iD		= -1;
	m_parent	= -1;
}

// ----------------------------------------------------------------------//

CBone::~CBone()
{
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
