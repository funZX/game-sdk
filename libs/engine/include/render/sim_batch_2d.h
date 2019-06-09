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

#ifndef __SIM_BATCHING_2D_H
#define __SIM_BATCHING_2D_H

#include <core/sim_core.h>
#include <core/sim_interfaces.h>
#include <render/sim_render.h>

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

class CBatch2D : public IRenderable, public IEngineItem
{
	friend class CVertexGroup;

public:
	CBatch2D( CDriver *driver );
	CBatch2D( const std::string& name, CDriver *driver );
	virtual ~CBatch2D();

	// ------------------------------------------------------------------//
	void 						AddQuad( CMaterial *material, const f32* v0, const f32* v1, const f32* v2, const f32* v3 );

	void 						Render( CDriver *driver );

	CVertexGroup*				GetVertexGroup()	{ return m_vertexGroup; }
	// ------------------------------------------------------------------//
	static const int			MaxQuads = 4096;

protected:

	// ------------------------------------------------------------------//
	void 						AllocateQuads( s32 numQuads );
	// ------------------------------------------------------------------//

	CVertexGroup*				m_vertexGroup;
	s32 						m_numQuads;

	CDriver*					m_driver;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim

#endif // __SIM_BATCHING_2D_H
