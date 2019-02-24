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

#include <core/io/sim_file_stream.h>
#include <core/io/sim_json_stream.h>

namespace sim
{
namespace io
{
// ----------------------------------------------------------------------//

CJsonStream::CJsonStream( const std::string &filename )
	: m_jsonRoot(nullptr)
{
	CFileStream* fileStream = SIM_NEW CFileStream( filename );

	if( fileStream->Open() )
	{
		m_jsonRoot = json_loads( ( const char* ) fileStream->Read( 0 ), 0, &m_jsonError );
	}

	SIM_SAFE_DELETE( fileStream );
}

// ----------------------------------------------------------------------//

CJsonStream::CJsonStream( const u8* memfile )
{
	m_jsonRoot = json_loads( ( const char* ) memfile, 0, &m_jsonError );
}

// ----------------------------------------------------------------------//

CJsonStream::~CJsonStream()
{
	if ( m_jsonRoot != nullptr )
	{
		json_decref( m_jsonRoot );
		m_jsonRoot = nullptr;
	}
}

// ----------------------------------------------------------------------//
}; // namespace json
}; // namespace sim

