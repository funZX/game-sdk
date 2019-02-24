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

#ifndef __SIM_MEM_STREAM_H
#define __SIM_MEM_STREAM_H

#include <core/sim_core.h>

namespace sim
{
namespace io
{
// ----------------------------------------------------------------------//

class CMemStream
{
public:
	CMemStream( void *mem, u32 maxsize );
	CMemStream( u32 maxsize );
	virtual ~CMemStream();

	s8				ReadS8();
	u8				ReadU8();
	s16				ReadS16();
	u16				ReadU16();
	s32				ReadS32();
	u32				ReadU32();
	f32				ReadF32();
	std::string		ReadString();
	void*			Read( u32 nBytes );

	void			WriteS8( s8 c );
	void			WriteU8( u8 c );
	void			WriteS16( s16 h );
	void			WriteU16( u16 h );
	void			WriteS32( s32 i );
	void			WriteU32( u32 i );
	void			WriteF32( f32 f );
	void			WriteString( const std::string& s );
	void			Write( void *p, u32 nBytes );

	bool			CanWrite( u32 nBytes);
	bool			CanRead( u32 nBytes );

	void*			Drop();
	// ------------------------------------------------------------------//

	inline void		SkipForward( u32 fw )  { m_cursor += fw; }
	inline void		SkipBackward( u32 bw ) { m_cursor += bw; }

	inline void		Rewind()	{ m_cursor = 0; }
	inline u32		GetCursor() { return m_cursor; }
	inline u32		GetSize() { return m_maxsize; }
	// ------------------------------------------------------------------//

protected:
	u32				m_cursor;
	u32				m_maxsize;

	s8*				m_memory;
	bool			m_isMemOwn;
};

// ----------------------------------------------------------------------//
}; // namespace io
}; // namespace sim

#endif // __SIM_MEM_STREAM_H
