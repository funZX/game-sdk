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

#include <core/sim_core.h>
#include <core/sim_interfaces.h>

#include <render/sim_render.h>

namespace sim
{
// ----------------------------------------------------------------------//
namespace io
{
	class CMemStream;
	class CFileSystem;
}
// ----------------------------------------------------------------------//
namespace ren
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

    inline void						SetBox( Vec3 box );
    inline Vec3				        GetBox();

    inline void						SetCenter( Vec3 center );
    inline Vec3				        GetCenter();

    inline void			            SetRadius( f32 radius );
    inline f32			            GetRadius();

	void					        Render( CDriver *driver );

	virtual bool					Load( io::CMemStream* ms );
	virtual bool					Save( io::CMemStream* ms );

	void							BindMaterial();
	CMaterial*						BindMaterial( CMaterial* material );
	// ------------------------------------------------------------------//

public:
	CVertexSource*					m_vertexSource;
	CVertexGroup*			        m_vertexGroup;

	std::string						m_material;

protected:

	// ------------------------------------------------------------------//
	Vec3							m_box;
	Vec3							m_center;
	f32					            m_radius;

	io::CFileSystem*				m_fs;
	// ------------------------------------------------------------------//
};

inline void	CMesh::SetBox(Vec3 box) 
{
    m_box = box;
}

inline Vec3	CMesh::GetBox()
{ 
    return m_box; 
}

inline void	CMesh::SetCenter(Vec3 center) 
{ 
    m_center = center;
}

inline Vec3	CMesh::GetCenter()
{ 
    return m_center; 
}

inline void	CMesh::SetRadius(f32 radius) 
{ 
    m_radius = radius; 
}

inline f32	CMesh::GetRadius()
{ 
    return m_radius; 
}

// ----------------------------------------------------------------------//
}; // namespace ren
}; // namespace sim
#endif // __SIM_MODEL_H
