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

#ifndef __SIM_LZMA_STREAM_H
#define __SIM_LZMA_STREAM_H

#include <sim_engine.h>

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
