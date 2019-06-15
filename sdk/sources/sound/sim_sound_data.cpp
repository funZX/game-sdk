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

#include <core/io/sim_mem_stream.h>
#include <core/sim_core.h>
#include <sound/sim_sound_data.h>

namespace sim
{
namespace snd
{
// ----------------------------------------------------------------------//

CSoundData::CSoundData()
{
	m_ID		= 0;
}

// ----------------------------------------------------------------------//

CSoundData::CSoundData(const std::string &name)
	: CSoundData()
{
	m_name = name;
}
// ----------------------------------------------------------------------//

CSoundData::~CSoundData()
{
	if( m_ID )
		alDeleteBuffers( 1, &m_ID );
}

// ----------------------------------------------------------------------//

u8* CSoundData::LoadWAV( io::CMemStream* ms, ALenum *format, u32 *size, u32 *rate )
{
	typedef struct
	{
		//big chunk
		unsigned int chunkID;
		unsigned int chunkSize;
		unsigned int chunkFormat;
		//fmt chunk
		unsigned int subchunk1ID;
		unsigned int subchunk1Size;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned int sampleRate;
		unsigned int byteRate;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		//data chunk
		unsigned int subchunk2ID;
		unsigned int subchunk2Size;
	} TWAVHeader;

	TWAVHeader		header;
	int header_size = sizeof( TWAVHeader );

	SIM_MEMCPY( &header, ms->Read( header_size ), header_size );
	
	if (header.chunkID != 0x46464952)		//RIFF
		return nullptr;
		
	if (header.chunkFormat != 0x45564157)	//WAVE
		return nullptr;
		
	if (header.subchunk1ID != 0x20746D66)	//fmt 
		return nullptr;
		
	if (header.subchunk2ID != 0x61746164)	//data
		return nullptr;
	
	unsigned char *buf = SIM_NEW unsigned char[ header.subchunk2Size ];
	SIM_MEMCPY( buf, ms->Read( header.subchunk2Size ), header.subchunk2Size );	

	if ( header.numChannels == 1 )
	{
		if ( header.bitsPerSample == 8 )
			*format = AL_FORMAT_MONO8;
		else
			*format = AL_FORMAT_MONO16;
	}
	else
	{
		if (header.bitsPerSample == 8)
			*format = AL_FORMAT_STEREO8;
		else
			*format = AL_FORMAT_STEREO16;
	}

	*size = header.subchunk2Size;
	*rate = header.sampleRate;

	return buf;
}

// ----------------------------------------------------------------------//

u32 CSoundData::Generate( u8 *buf, ALenum format, u32 size, u32 rate )
{
	alGenBuffers( 1, &m_ID );	
	alBufferData( m_ID, format, buf, size, rate );

	return m_ID;
}

// ----------------------------------------------------------------------//

u32 CSoundData::Generate( io::CMemStream* ms )
{
	ALenum format;
	u32 size;
	u32 rate;

	u8 *buf = LoadWAV( ms, &format, &size, &rate );

	if( buf != nullptr ) {
		Generate( buf, format, size, rate );
	}

	SIM_SAFE_DELETE_ARRAY( buf );

	return m_ID;
}

// ----------------------------------------------------------------------//
}; // namespace snd
}; // namespace sim