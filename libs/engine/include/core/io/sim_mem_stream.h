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
