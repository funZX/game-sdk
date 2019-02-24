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

class CMesh: public IRenderable, public IEngineItem
{
public:
	CMesh();
	CMesh( const std::string &name );
	CMesh( const std::string &name, io::CFileSystem* fs );
	virtual ~CMesh();

	// ------------------------------------------------------------------//

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

	void							BindMaterial();
	CMaterial*						BindMaterial( CMaterial* material );
	// ------------------------------------------------------------------//

public:
	CVertexSource*					m_vertexSource;
	CVertexGroup*			        m_vertexGroup;

	std::string						m_material;

protected:

	// ------------------------------------------------------------------//
	TVec3							m_box;
	TVec3							m_center;
	f32					            m_radius;

	io::CFileSystem*				m_fs;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_MODEL_H
