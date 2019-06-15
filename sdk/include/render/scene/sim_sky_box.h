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

#ifndef __SIM_SKY_BOX_H
#define __SIM_SKY_BOX_H

#include <core/sim_core.h>

#include <render/sim_render.h>
#include <core/sim_interfaces.h>

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

class CSkyBox : public IRenderable, public IEngineItem
{
public:
	// ------------------------------------------------------------------//
	CSkyBox();
	CSkyBox( const std::string &name );
	virtual ~CSkyBox();

	// ------------------------------------------------------------------//
	virtual void						Render( CDriver *driver );

	void								Generate( f32 size, 
												  io::CMemStream* front,	io::CMemStream* back,
												  io::CMemStream* left,		io::CMemStream* right,
												  io::CMemStream* top,		io::CMemStream* bottom );

	CTexture*							GetTexture() { return (CTexture*) m_texture; }
	
    virtual bool    					Load(io::CMemStream* ms);
    virtual bool    					Save(io::CMemStream* ms);
    // ------------------------------------------------------------------//

protected:
	// ------------------------------------------------------------------//
	void								InitEffect();
	// ------------------------------------------------------------------//
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
