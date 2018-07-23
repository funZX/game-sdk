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

#ifndef __SIM_LZMA_STREAM_H
#define __SIM_LZMA_STREAM_H

#include <engine.h>

extern "C"
{
	#include <7z.h>
	#include <7zAlloc.h>
	#include <7zCrc.h>
	#include <7zFile.h>
	#include <7zVersion.h>
};

namespace sim
{
namespace io
{
// ----------------------------------------------------------------------//

class CLzmaStream
{
public:
	// ------------------------------------------------------------------//
	CLzmaStream( const std::string &lzmaName );
	 ~CLzmaStream();
	// ------------------------------------------------------------------//

	bool				Open();
	void				Close();

	bool				GetCurrent( std::string &name );
	void				OpenCurrent( u8 **out, u32* offset, s32 *size );
	void				CloseCurrent( u8 **out );

	void				OpenFile( const std::string &file, u8 **out, u32*offset, s32 *size );
	void				CloseFile( u8 **out );

	bool				Advance();
	// ------------------------------------------------------------------//

protected:
	void				LzmaInitOnce();

	// ------------------------------------------------------------------//
protected:
	// ------------------------------------------------------------------//
	std::string			m_lzmaName;

	CFileInStream		m_lzmaStream;
	CLookToRead			m_lzmaLook;

	CSzArEx				m_lzmaDB;
	ISzAlloc			m_lzmaAllocImp;
	ISzAlloc			m_lzmaAllocTempImp;

	u32					m_numEntries;
	u32					m_currentEntry;

	bool				m_isOpened;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace io
}; // namespace sim

#endif // __SIM_LZMA_STREAM_H
