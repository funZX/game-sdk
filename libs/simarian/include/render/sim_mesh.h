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

#ifndef __SIM_MODEL_H
#define __SIM_MODEL_H

#include <core/sim_interfaces.h>

#include <render/sim_render.h>
#include <math/sim_matrix4.h>
#include <math/sim_vec2.h>
#include <math/sim_vec3.h>

using namespace sim::mat;

namespace sim
{
// ----------------------------------------------------------------------//
namespace io
{
	class CMemStream;
	class CFileSystem;
}
// ----------------------------------------------------------------------//
namespace rnr
{
// ----------------------------------------------------------------------//

class CShader;
class CMaterial;
class CVertexGroup;
class CFileStream;
class CVertexSource;

class CMesh: public IRenderable
{
public:
	CMesh( const std::string &name );
	virtual ~CMesh();

	// ------------------------------------------------------------------//

	inline const std::string&		GetName()	const { return m_name; }

	inline void						SetBox( const TVec3* box)					{ Vec3Copy( &m_box, box ); }
	inline const TVec3*				GetBox() const								{ return &m_box; }

	inline void						SetCenter( const TVec3* center)				{ Vec3Copy( &m_center, center ); }
	inline const TVec3*				GetCenter() const							{ return &m_center; }

	inline void			            SetRadius( f32 radius )						{ m_radius = radius; }
	inline f32			            GetRadius() const							{ return m_radius; }

	void							SetDetail( u32 detail );
	void					        Render( CDriver *driver );

	void							Load( io::CMemStream* ms );
	void							Save( io::CMemStream* ms );

	void							BindMaterial( io::CFileSystem* fs );
	// ------------------------------------------------------------------//

public:
	CVertexSource*					m_vertexSource;
	CVertexGroup*			        m_vertexGroup;

	std::string						m_material;

protected:

	// ------------------------------------------------------------------//
	std::string						m_name;

	TVec3							m_box;
	TVec3							m_center;
	f32					            m_radius;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_MODEL_H
