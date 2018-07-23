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

#ifndef __SIM_SOUND_H
#define __SIM_SOUND_H

#include <core/sim_core.h>

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

namespace sim
{
namespace snd
{
// ----------------------------------------------------------------------//
#if SIM_DEBUG
	void OpenALCheck( const char *file, const char *func,  unsigned int line );
#define SIM_CHECK_OPENAL() OpenALCheck( __FILE__, __FUNCTION__, __LINE__ )
#else
#	define SIM_CHECK_OPENAL()
#endif
// ----------------------------------------------------------------------//
}; // namespace snd
}; // namespace sim

#endif // __SIM_SOUND_H
