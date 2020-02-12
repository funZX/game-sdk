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

#ifndef __SIM_VERTEX_GROUP_H
#define __SIM_VERTEX_GROUP_H

#include <core/sim_core.h>

#include <render/sim_render.h>

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

struct Sphere;
struct Cube;

class CDriver;
class CMaterial;
class CRect2D;
class CFontChar;
class CVertexSource;
class CBoneHierarchy;

class CVertexGroup
{
	// ------------------------------------------------------------------//

public:
	// ------------------------------------------------------------------//
	CVertexGroup();
	virtual ~CVertexGroup();
	// ------------------------------------------------------------------//
	inline u16						GetVboSize() const		{ return m_vboSize; }
	inline u16*						GetVboData() const		{ return m_vboData; }
    inline u32                      GetVboOffset() const    { return m_vboOffset; }

	CMaterial*						GetMaterial()			{ return m_material; }
	void							SetMaterial( CMaterial *mtl );

	CVertexSource*					GetVertexSource()		{ return m_vertexSource; }
	void							SetVertexSource( CVertexSource* vertexSource )	{ m_vertexSource = vertexSource; }

	inline u32						GetID()					{ return m_iD; }
    void                            BufferData( u32 glUsage = GL_STATIC_DRAW, bool isDataOwned = true );
	// ------------------------------------------------------------------//

    virtual bool					Load(io::CMemStream* ms);
    virtual bool					Save(io::CMemStream* ms);

protected:

	// ------------------------------------------------------------------//
	u32								m_iD;
	CMaterial*						m_material;

public:
	CVertexSource*					m_vertexSource;

	u16								m_vboSize;
	u16*							m_vboData;
    u32                             m_vboOffset;

	CBoneHierarchy*					m_boneHierarchy;
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_VERTEX_GROUP_H
