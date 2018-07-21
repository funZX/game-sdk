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

#ifndef __SIM_RENDER_H
#define __SIM_RENDER_H

#include <core/sim_core.h>
#include <math/sim_math.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//
#if SIM_DEBUG
	void OpenGLCheck( const char *file, const char *func,  unsigned int line );
#define SIM_CHECK_OPENGL() OpenGLCheck( __FILE__, __FUNCTION__, __LINE__ )
#else
#	define SIM_CHECK_OPENGL()
#endif
// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim

#endif // __SIM_RENDER_H