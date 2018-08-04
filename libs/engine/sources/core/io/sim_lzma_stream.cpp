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

#include <core/sim_utf.h>
#include <core/io/sim_lzma_stream.h>

namespace sim
{
namespace io
{
// ----------------------------------------------------------------------//

CLzmaStream::CLzmaStream( const std::string &lzma )
{
	m_lzmaName 	= lzma;
	
	m_lzmaAllocImp.Alloc		= SzAlloc;
	m_lzmaAllocImp.Free			= SzFree;
	
	m_lzmaAllocTempImp.Alloc	= SzAllocTemp;
	m_lzmaAllocTempImp.Free		= SzFreeTemp;

	m_numEntries				= 0;
	m_currentEntry				= 0;

	m_isOpened					= false;

	LzmaInitOnce();
}

// ----------------------------------------------------------------------//

CLzmaStream::~CLzmaStream()
{
	Close();
}

// ----------------------------------------------------------------------//

void CLzmaStream::LzmaInitOnce()
{
	static bool lzmaCRC = false;

	if( !lzmaCRC )
	{
		CrcGenerateTable();
		lzmaCRC = true;
	}

}

// ----------------------------------------------------------------------//

bool CLzmaStream::Open()
{
	SRes res = SZ_OK;

	if( !m_isOpened )
	{
		if( !InFile_Open( &m_lzmaStream.file, m_lzmaName.c_str() ) )
		{
			FileInStream_CreateVTable( &m_lzmaStream );
			LookToRead_CreateVTable( &m_lzmaLook, false );

			m_lzmaLook.realStream = &m_lzmaStream.s;
			LookToRead_Init( &m_lzmaLook );

			SzArEx_Init( &m_lzmaDB );
			res = SzArEx_Open( &m_lzmaDB, &m_lzmaLook.s, &m_lzmaAllocImp, &m_lzmaAllocTempImp );

			m_numEntries	= m_lzmaDB.db.NumFiles;
			m_isOpened		= ( res == SZ_OK );
		}
	}

	return m_isOpened;
}

// ----------------------------------------------------------------------//

void CLzmaStream::Close( void )
{
	if( m_isOpened )
	{
		SzArEx_Free( &m_lzmaDB, &m_lzmaAllocImp );
		File_Close( &m_lzmaStream.file );

		m_isOpened = false;
	}
}

// ----------------------------------------------------------------------//

bool CLzmaStream::GetCurrent( std::string &name )
{
	std::string crtName = "";

	const CSzFileItem *f = m_lzmaDB.db.Files + m_currentEntry;
	bool isDir = f->IsDir || ( f->AttribDefined & FILE_ATTRIBUTE_DIRECTORY );

	if( !isDir )
	{
		s32 len16	= SzArEx_GetFileNameUtf16( &m_lzmaDB, m_currentEntry, nullptr );
		u16 *tu16	= SIM_NEW u16[ len16 ];
		u8  *tu8	= SIM_NEW u8[ len16 ];
		SzArEx_GetFileNameUtf16( &m_lzmaDB, m_currentEntry, tu16 );

		s32 len8 = 0;

		bool success = UTF16ToUTF8( tu8, &len8, tu16, len16 );
		SIM_ASSERT( success );

		crtName = (char*)tu8;

		SIM_SAFE_DELETE_ARRAY( tu8 );
		SIM_SAFE_DELETE_ARRAY( tu16 );
	}

	name = crtName;

	return !f->IsDir;
}

// ----------------------------------------------------------------------//

bool CLzmaStream::Advance()
{
	return ++m_currentEntry < m_numEntries;
}

// ----------------------------------------------------------------------//

void CLzmaStream::OpenCurrent( u8 **out, u32* offset, s32 *outSize )
{
	SIM_ASSERT( out != nullptr );
	SIM_ASSERT( outSize != nullptr );

	size_t blockOffset		= 0;
	size_t outSizeProcessed = 0;
	u32 blockIndex			= 0;
	size_t size				= 0;

	SRes res = SzArEx_Extract( &m_lzmaDB, &m_lzmaLook.s, m_currentEntry,
		&blockIndex, out, &size,
		&blockOffset, &outSizeProcessed,
		&m_lzmaAllocImp, &m_lzmaAllocTempImp );

	*offset = blockOffset;

	SIM_ASSERT( res == SZ_OK );

	*outSize = outSizeProcessed;
}

// ----------------------------------------------------------------------//

void CLzmaStream::CloseCurrent( u8 **out )
{
	SIM_ASSERT( *out != nullptr );

	IAlloc_Free( &m_lzmaAllocImp, *out );
	*out = nullptr;
}

// ----------------------------------------------------------------------//

void CLzmaStream::OpenFile( const std::string &file, u8 **out, u32* offset, s32 *size )
{
	s32 saveCurrent = m_currentEntry;
	std::string n = "";

	m_currentEntry	= 0;

	while (true)
	{
		GetCurrent( n );

		if (!file.compare( n ))
			break;

		if( !Advance() )
			break;
	};

	*out = nullptr;
	*size = 0;

	if( m_currentEntry < m_numEntries )
		OpenCurrent( out, offset, size );

	m_currentEntry = saveCurrent;
}

// ----------------------------------------------------------------------//

void CLzmaStream::CloseFile( u8 **out )
{
	SIM_ASSERT( *out != nullptr );

	IAlloc_Free( &m_lzmaAllocImp, *out );
	*out = nullptr;
}

// ----------------------------------------------------------------------//
}; // namespace io
}; // namespace sim

