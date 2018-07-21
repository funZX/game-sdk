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

#ifndef __SIM_FRAME_BUFFER_H
#define __SIM_FRAME_BUFFER_H

#include <render/sim_render.h>
#include <core/io/sim_mem_stream.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

class CDriver;

class CFrameBuffer
{
public:
	// ------------------------------------------------------------------//
	CFrameBuffer( const std::string &name );
	virtual ~CFrameBuffer();

	void					Generate( u32 width, u32 height );

	void					Bind( CDriver* driver );
	void					UnBind( CDriver* driver );

	inline u32				GetID() { return m_iD; }
	inline u32				GetColorID() { return m_texID; }
	inline u32				GetDepthID() { return m_depthBuffer; }

	inline u32				GetWidth() { return m_width; }
	inline u32				GetHeight() { return m_height; }

protected:
	std::string				m_name;

	u32			            m_width;
	u32			            m_height;

	u32			            m_iD;
	u32			            m_depthBuffer;
	u32						m_texID;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_TEXTURE_H
