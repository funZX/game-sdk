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

#ifndef __SIM_JSON_STREAM_H
#define __SIM_JSON_STREAM_H

#include <engine.h>
#include <jansson.h>

namespace sim
{
namespace io
{
// ----------------------------------------------------------------------//

class CJsonStream
{
public:
	// ------------------------------------------------------------------//
	CJsonStream( const std::string &filename );
	CJsonStream( const u8* memfile );
	 ~CJsonStream();
	// ------------------------------------------------------------------//

	json_t*				GetRoot()		{ return m_jsonRoot; }
	json_error_t*		GetError()		{ return &m_jsonError; }

protected:

	json_t*				m_jsonRoot;
	json_error_t		m_jsonError;
};

// ----------------------------------------------------------------------//
}; // namespace io
}; // namespace sim

#endif // __SIM_JSON_STREAM_H
