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

#ifndef __SIM_SKY_BOX_H
#define __SIM_SKY_BOX_H

#include <render/sim_render.h>
#include <core/sim_interfaces.h>

#include <math/sim_vec4.h>
#include <math/sim_vec3.h>

namespace sim
{
// ----------------------------------------------------------------------//
namespace io { class CMemStream; };
// ----------------------------------------------------------------------//
namespace rnr
{
// ----------------------------------------------------------------------//

class CDriver;
class CEffect;
class CCubeTexture;
class CMaterial;
class CTexture;
class CVertexSource;
class CVertexGroup;

class CSkyBox : public IRenderable
{
public:
	// ------------------------------------------------------------------//

	CSkyBox( const std::string &name );
	virtual ~CSkyBox();

	// ------------------------------------------------------------------//
	inline const std::string&			GetName()			{ return m_name; }

	virtual void						Render( CDriver *driver );

	void								Generate( f32 size, 
												  io::CMemStream* front,	io::CMemStream* back,
												  io::CMemStream* left,		io::CMemStream* right,
												  io::CMemStream* top,		io::CMemStream* bottom );

	CTexture*							GetTexture() { return (CTexture*) m_texture; }
	// ------------------------------------------------------------------//

protected:
	// ------------------------------------------------------------------//
	void								InitEffect();
	// ------------------------------------------------------------------//
	std::string							m_name;

	CVertexGroup*						m_vertexGroup;
	CMaterial*							m_material;
	CCubeTexture*						m_texture;
	CEffect*							m_effect;

	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim

#endif // __SIM_SKY_BOX_H
