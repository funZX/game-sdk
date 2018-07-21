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

#include <core/io/sim_file_stream.h>
#include <core/sim_core.h>
#include <sound/sim_sound_data.h>

namespace sim
{
namespace snd
{
// ----------------------------------------------------------------------//

CSoundData::CSoundData( const std::string &name )
{
	m_name		= name;
	m_ID		= 0;
}

// ----------------------------------------------------------------------//

CSoundData::~CSoundData()
{
	if( m_ID )
		alDeleteBuffers( 1, &m_ID );
}

// ----------------------------------------------------------------------//

u8* CSoundData::LoadWAV( const std::string &fileName, ALenum *format, u32 *size, u32 *rate )
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

	io::CFileStream stream( fileName );

	if( !stream.Open() ) 
		return NULL;

	SIM_MEMCPY( &header, stream.Read( header_size ), header_size );
	
	if (header.chunkID != 0x46464952)		//RIFF
		return NULL;
		
	if (header.chunkFormat != 0x45564157)	//WAVE
		return NULL;
		
	if (header.subchunk1ID != 0x20746D66)	//fmt 
		return NULL;
		
	if (header.subchunk2ID != 0x61746164)	//data
		return NULL;
	
	unsigned char *buf = SIM_NEW unsigned char[ header.subchunk2Size ];
	SIM_MEMCPY( buf, stream.Read( header.subchunk2Size ), header.subchunk2Size );	
	stream.Close();

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

u32 CSoundData::Generate()
{
	ALenum format;
	u32 size;
	u32 rate;

	u8 *buf = LoadWAV( m_name, &format, &size, &rate );

	if( buf != NULL ) {
		Generate( buf, format, size, rate );
	}

	SIM_SAFE_DELETE_ARRAY( buf );

	return m_ID;
}

// ----------------------------------------------------------------------//
}; // namespace snd
}; // namespace sim