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

#ifndef __SIM_COLORS_H
#define __SIM_COLORS_H

#include <math/sim_vec4.h>
#include <render/sim_render.h>

using namespace sim::mat;

namespace sim
{
namespace col
{
// ----------------------------------------------------------------------//

extern TVec4 White;
extern TVec4 Black;
extern TVec4 Red;
extern TVec4 Green;
extern TVec4 Blue;
extern TVec4 Orange;
extern TVec4 Cyan;
extern TVec4 Magenta;
extern TVec4 Yellow;

struct TColor
{
	u8 r;
	u8 g;
	u8 b;
	u8 a;
};

static inline void ColorToVec4( const TColor *col, TVec4 *vcol )
{
	vcol->x = SIM_CLAMP( col->r / 255.0f, 0.0f, 1.0f );
	vcol->y = SIM_CLAMP( col->g / 255.0f, 0.0f, 1.0f );
	vcol->z = SIM_CLAMP( col->b / 255.0f, 0.0f, 1.0f );
	vcol->w = SIM_CLAMP( col->a / 255.0f, 0.0f, 1.0f );
}

static inline void Vec4ToColor( const TVec4 *vcol, TColor *col )
{
	col->r = ( u8 ) SIM_CLAMP( vcol->x * 255.0f, 0, 255 );
	col->g = ( u8 ) SIM_CLAMP( vcol->y * 255.0f, 0, 255 );
	col->b = ( u8 ) SIM_CLAMP( vcol->z * 255.0f, 0, 255 );
	col->a = ( u8 ) SIM_CLAMP( vcol->w * 255.0f, 0, 255 );
}

// ----------------------------------------------------------------------//
}; // namespace col
}; // namespace sim

#endif // __SIM_COLORS_H
