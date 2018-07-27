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

#ifndef __SIM_SOUND_DATA_H
#define __SIM_SOUND_DATA_H

#include <OpenAL/al.h>

#include <core/sim_interfaces.h>
#include <core/sim_core.h>

namespace sim
{
namespace io { class CMemStream; }
namespace snd
{
// ----------------------------------------------------------------------//

class CSoundData : public IEngineItem
{
protected:	
	ALuint			m_ID;
	
public:
	CSoundData();
	CSoundData( const std::string &name );
	virtual		~CSoundData();

	static u8*	            LoadWAV( io::CMemStream*, ALenum *format, u32 *size, u32 *rate );

	u32						Generate( u8 *buf, ALenum format, u32 size, u32 rate );
	u32						Generate( io::CMemStream* );
	
	ALuint					GetID() { return m_ID; }
};

// ----------------------------------------------------------------------//
}; // namespace snd
}; // namespace sim

#endif // __SIM_SOUND_DATA_H
