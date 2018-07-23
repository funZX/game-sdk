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

#ifndef __SIM_SOUND_SOURCE_H
#define __SIM_SOUND_SOURCE_H

#include <OpenAL/al.h>

#include <math/sim_vec3.h>

using namespace sim::mat;

namespace sim
{
namespace snd
{
// ----------------------------------------------------------------------//

class CSoundData;

class CSoundSource
{
public:
	// ------------------------------------------------------------------//
	typedef enum
	{
		k_Loop_Yes,
		k_Loop_No

	} K_LOOP;
	// ------------------------------------------------------------------//
	typedef enum
	{
		k_Type_Ambient,
		k_Type_Fx

	} K_TYPE;
	// ------------------------------------------------------------------//

public:
	// ------------------------------------------------------------------//
	CSoundSource();
	 ~CSoundSource();
	// ------------------------------------------------------------------//

	void Init( CSoundData *data );
	
	void SetAmbient( void );
	void SetFX( TVec3 *pos, f32 rad, f32 maxrad );
	void SetDefault( void );
	void Play( void );
	void Pause( void );
	void Resume( void );
	void Stop( void );

	void SetSpeed( f32 val );
	void SetVolume( f32 val );
	void SetLoop( K_LOOP loop );
	void Rewind( void );
	// ------------------------------------------------------------------//

protected:
	ALuint			m_ID;

	ALint			m_state;
	K_TYPE			m_type;
	K_LOOP			m_loop;

	CSoundData*		m_soundData;
};

// ----------------------------------------------------------------------//
}; // namespace snd
}; // namespace sim

#endif // __SIM_SOUND_SOURCE_H
