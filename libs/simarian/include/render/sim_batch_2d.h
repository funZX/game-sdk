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

#ifndef __SIM_BATCHING_2D_H
#define __SIM_BATCHING_2D_H

#include <core/sim_interfaces.h>
#include <math/sim_matrix4.h>
#include <render/sim_render.h>

#include <render/sim_color.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

class CEffect;
class CMaterial;
class CDriver;
class CVertexSource;
class CVertexGroup;

class CBatch2D : public IRenderable
{
	friend class CVertexGroup;

public:
	CBatch2D( const std::string& name, CDriver *driver );
	virtual ~CBatch2D();

	// ------------------------------------------------------------------//
	void 						AddQuad( CMaterial *material, const f32* v0, const f32* v1, const f32* v2, const f32* v3 );

	void 						Render( CDriver *driver );

	CVertexGroup*				GetVertexGroup()	{ return m_vertexGroup; }
	// ------------------------------------------------------------------//
	const std::string&			GetName()			{ return m_name; }
	// ------------------------------------------------------------------//
	static const int			MaxQuads = 4096;

protected:

	// ------------------------------------------------------------------//
	void 						AllocateQuads( s32 numQuads );
	// ------------------------------------------------------------------//

	std::string					m_name;

	CVertexGroup*				m_vertexGroup;
	s32 						m_numQuads;

	CDriver*					m_driver;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim

#endif // __SIM_BATCHING_2D_H
