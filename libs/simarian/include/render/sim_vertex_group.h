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

#ifndef __SIM_VERTEX_GROUP_H
#define __SIM_VERTEX_GROUP_H

#include <render/sim_render.h>
#include <math/sim_matrix4.h>

namespace sim
{
// ----------------------------------------------------------------------//
namespace io
{
	class CMemStream;
}
// ----------------------------------------------------------------------//
namespace rnr
{
// ----------------------------------------------------------------------//

struct TSphere;
struct TCube;

class CDriver;
class CMaterial;
class CRect2D;
class CBatch2D;
class CBatch3D;
class CFontChar;
class CVertexSource;

class CVertexGroup
{
	// ------------------------------------------------------------------//

public:
	// ------------------------------------------------------------------//
	CVertexGroup();
	virtual ~CVertexGroup();
	// ------------------------------------------------------------------//
	inline u16						GetVboSize() const		{ return m_detail->m_vboSize; }
	inline u16*						GetVboData() const		{ return m_detail->m_vboData; }

	CMaterial*						GetMaterial()			{ return m_material; }
	void							SetMaterial( CMaterial *mtl );

	CVertexSource*					GetVertexSource()		{ return m_vertexSource; }
	void							SetVertexSource( CVertexSource* vertexSource )	{ m_vertexSource = vertexSource; }

	inline u32						GetID()					{ return m_iD; }
	u32								Generate();

	void							SetDetail( u32 detail );
	// ------------------------------------------------------------------//

	void							Load( io::CMemStream* ms );
	void							Save( io::CMemStream* ms );

protected:
	// ------------------------------------------------------------------//
	u32								m_iD;
	CMaterial*						m_material;

public:
	CVertexSource*					m_vertexSource;

	u16								m_vboSize;
	u16*							m_vboData;

	CVertexGroup*					m_lod;
	CVertexGroup*					m_detail;
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_VERTEX_GROUP_H
