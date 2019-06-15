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
	enum class SeekFrom : u32
	{
		Start,
		Current,
		End
	};
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
	void			Seek( SeekFrom seekMode, u32 offset );

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
