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

#include <sound/sim_sound.h>
#include <sound/sim_sound_data.h>
#include <sound/sim_sound_source.h>

namespace sim
{
namespace snd
{
// ----------------------------------------------------------------------//

CSoundSource::CSoundSource()
{
	m_ID		= 0;
	m_type		= Type::Ambient;
	m_loop		= Loop::No;
	m_state		= AL_STOPPED;
	
	m_soundData = nullptr;
}

// ----------------------------------------------------------------------//

CSoundSource::CSoundSource( const std::string& name )
	: CSoundSource()
{
	m_name = name;
}

// ----------------------------------------------------------------------//

CSoundSource::~CSoundSource()
{
	alSourceStop( m_ID );

	alSourcei( m_ID, AL_BUFFER, 0 );
	
	alDeleteSources( 1, &m_ID );
}

// ----------------------------------------------------------------------//

void CSoundSource::Init( CSoundData *soundData )
{
	m_soundData = soundData;
	
	alGenSources( 1, &m_ID );
	
	alSourcei( m_ID, AL_BUFFER, soundData->GetID() );

	SIM_CHECK_OPENAL();
}

// ----------------------------------------------------------------------//

void CSoundSource::SetAmbient( void )
{
	m_type = Type::Ambient;

	SetDefault();
}

// ----------------------------------------------------------------------//

void CSoundSource::SetLoop( Loop loop )
{
	m_loop = loop;
}

// ----------------------------------------------------------------------//

void CSoundSource::SetFX( Vec3 *pos, f32 rad, f32 maxrad )
{
	m_type = Type::Fx;

    alSourcei( m_ID, AL_SOURCE_RELATIVE, AL_FALSE );
	alSourcef( m_ID, AL_REFERENCE_DISTANCE, rad );
	alSourcef( m_ID, AL_MAX_DISTANCE, maxrad );

	alSource3f( m_ID,
				AL_POSITION,
				pos->x,
				pos->y,
				pos->z );
				
	SIM_CHECK_OPENAL();
}

// ----------------------------------------------------------------------//

void CSoundSource::SetDefault( void )
{
    alSource3f( m_ID, AL_POSITION, 0.0f, 0.0f, 0.0f );
    alSource3f( m_ID, AL_VELOCITY, 0.0f, 0.0f, 0.0f );
    alSource3f( m_ID, AL_DIRECTION, 0.0f, 0.0f, 0.0f );
    alSourcef( m_ID, AL_ROLLOFF_FACTOR, 1.0f );
    alSourcei( m_ID, AL_SOURCE_RELATIVE, AL_TRUE );
	alSourcef( m_ID, AL_GAIN, 1.0 );
	alSourcef( m_ID, AL_MIN_GAIN, 0.0f);
	alSourcef( m_ID, AL_MAX_GAIN, 1.0f);
	alSourcef( m_ID, AL_PITCH, 1.0f ); 

	SIM_CHECK_OPENAL();
}

// ----------------------------------------------------------------------//

void CSoundSource::Play( void )
{
	alGetSourcei( m_ID, AL_SOURCE_STATE, &m_state );
	
	if( m_state != AL_PLAYING )
	{	
		m_state = AL_PLAYING;
		
		alSourcei( m_ID, AL_LOOPING, m_loop == Loop::Yes );
		
		alSourcePlay( m_ID );
		
		SIM_CHECK_OPENAL();
	}
}

// ----------------------------------------------------------------------//

void CSoundSource::Pause( void )
{
	if( m_state == AL_PLAYING )
	{
		m_state = AL_PAUSED;
		
		alSourcePause( m_ID );

		SIM_CHECK_OPENAL();
	}
}

// ----------------------------------------------------------------------//

void CSoundSource::Resume( void )
{
	if( m_state == AL_PAUSED )
	{
		m_state = AL_PLAYING;
		
		alSourcei( m_ID, AL_LOOPING, m_loop == Loop::Yes );
		
		alSourcePlay( m_ID );
		
		SIM_CHECK_OPENAL();
	}
}

// ----------------------------------------------------------------------//

void CSoundSource::Stop( void )
{
	if( m_state != AL_STOPPED )
	{
		m_state = AL_STOPPED;
		
		alSourceStop( m_ID );

		SIM_CHECK_OPENAL();
	}
}

// ----------------------------------------------------------------------//

void CSoundSource::SetSpeed( f32 _speed )
{
	alSourcef( m_ID, AL_PITCH, _speed ); 

	SIM_CHECK_OPENAL();
}

// ----------------------------------------------------------------------//

void CSoundSource::SetVolume(  f32 _volume )
{
	alSourcef( m_ID, AL_GAIN, _volume );

	SIM_CHECK_OPENAL();
}

// ----------------------------------------------------------------------//

void CSoundSource::Rewind( void )
{
	alSourceRewind( m_ID );

	SIM_CHECK_OPENAL();
}

// ----------------------------------------------------------------------//
}; // namespace snd
}; // namespace sim