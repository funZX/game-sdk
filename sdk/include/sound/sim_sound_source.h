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

#ifndef __SIM_SOUND_SOURCE_H
#define __SIM_SOUND_SOURCE_H

#include <OpenAL/al.h>

#include <core/sim_core.h>
#include <core/sim_interfaces.h>

namespace sim
{
namespace snd
{
// ----------------------------------------------------------------------//

class CSoundData;

class CSoundSource : public IEngineItem
{
public:
	// ------------------------------------------------------------------//
	enum class Loop : u32
	{
		Yes,
		No
	};
	// ------------------------------------------------------------------//
	enum class Type : u32
	{
		Ambient,
		Fx
	};
	// ------------------------------------------------------------------//

public:
	// ------------------------------------------------------------------//
	CSoundSource();
	CSoundSource( const std::string& name );
	 ~CSoundSource();
	// ------------------------------------------------------------------//

	void Init( CSoundData *data );
	
	void SetAmbient( void );
	void SetFX( Vec3 *pos, f32 rad, f32 maxrad );
	void SetDefault( void );
	void Play( void );
	void Pause( void );
	void Resume( void );
	void Stop( void );

	void SetSpeed( f32 val );
	void SetVolume( f32 val );
	void SetLoop( Loop loop );
	void Rewind( void );
	// ------------------------------------------------------------------//

protected:
	ALuint			m_ID;

	ALint			m_state;
	Type			m_type;
	Loop			m_loop;

	CSoundData*		m_soundData;
};

// ----------------------------------------------------------------------//
}; // namespace snd
}; // namespace sim

#endif // __SIM_SOUND_SOURCE_H
