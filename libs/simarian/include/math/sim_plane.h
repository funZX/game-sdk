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

#ifndef __SIM_PLANE_H
#define __SIM_PLANE_H

#include <math/sim_math.h>
#include <math/sim_vec3.h>

namespace sim
{
namespace mat
{
// ----------------------------------------------------------------------//

union TPlane
{
	f32 raw[ 4 ];

	struct
	{
		f32 a;
		f32 b;
		f32 c;
		f32 d;
	};
};

/*
void PlaneFromTri( TPlane *_this, TVec3 *v0, TVec3 *v1, TVec3 *v2 )
{

}
*/

// ----------------------------------------------------------------------//
}; // namespace mat
}; // namespace sim

#endif // __SIM_PLANE_H
