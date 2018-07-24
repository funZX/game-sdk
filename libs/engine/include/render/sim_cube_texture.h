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

#ifndef __SIM_CUBE_TEXTURE_H
#define __SIM_CUBE_TEXTURE_H

#include <render/sim_render.h>
#include <render/sim_texture.h>

namespace sim
{
namespace io { class CMemStream; }
namespace rnr
{
// ----------------------------------------------------------------------//

class CCubeTexture : public CTexture
{
public:
	CCubeTexture();
	CCubeTexture(  const std::string &name );
	virtual ~CCubeTexture();

	u32			    Generate( io::CMemStream* front,	io::CMemStream* back,
							  io::CMemStream* left,		io::CMemStream* right,
							  io::CMemStream* top,		io::CMemStream* bottom );
	// ------------------------------------------------------------------//

protected:
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_CUBE_TEXTURE_H
