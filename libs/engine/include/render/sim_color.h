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

extern TVec4 Cyan;
extern TVec4 Magenta;
extern TVec4 Yellow;

extern TVec4 Orange;
extern TVec4 Blueish;


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
