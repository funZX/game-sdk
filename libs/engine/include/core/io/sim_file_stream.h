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

#ifndef __SIM_FILE_STREAM_H
#define __SIM_FILE_STREAM_H

#include <core/sim_core.h>

namespace sim
{
namespace io
{
// ----------------------------------------------------------------------//

class CFileStream
{
public:
	// ------------------------------------------------------------------//
	typedef enum
	{
		k_SeekFrom_Start,
		k_SeekFrom_Current,
		k_SeekFrom_End

	} K_SEEK_FROM;
	// ------------------------------------------------------------------//

public:
	// ------------------------------------------------------------------//
	CFileStream( const std::string &filename );
	 ~CFileStream();
	// ------------------------------------------------------------------//

	bool			Open();
	void			Close();
	u32				GetSize() { return m_size; }

	void*			Drop();
	void*			Read( u32 size );
	bool			Eof( void );
	void			Seek( K_SEEK_FROM seekMode, u32 offset );

protected:
	std::string		m_filename;
	u32		 		m_cursor;
	u32		 		m_size;

	s8*				m_memory;
	s8*				m_current;
};

// ----------------------------------------------------------------------//
}; // namespace io
}; // namespace sim

#endif // __SIM_FILE_STREAM_H
