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

#ifndef __SIM_UTF_H
#define __SIM_UTF_H

#include <core/sim_core.h>

namespace sim
{
// ----------------------------------------------------------------------//
bool	UTF16ToUTF8( u8 *dst, s32 *dstLen, const u16 *src, s32 srcLen );
// ----------------------------------------------------------------------//
};

#endif // __SIM_CORE_H