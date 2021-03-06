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

#ifndef __SIM_SCENE_H
#define __SIM_SCENE_H

#include <core/sim_core.h>
#include <core/sim_interfaces.h>
#include <core/sim_octree.h>

#include <render/sim_render.h>

namespace sim
{
// ----------------------------------------------------------------------//
namespace io { class CFileSystem; }
namespace ren
{
// ----------------------------------------------------------------------//
class CActor;
class CLight;
class CCamera;
class CSceneNode;
// ----------------------------------------------------------------------//

class CScene : public IUpdatable, public IRenderable, public IEngineItem
{
public:
    // ------------------------------------------------------------------//
    typedef stl::CBalanceTree<u32, CSceneNode*>	THierarchy;
    // ------------------------------------------------------------------//
	// ------------------------------------------------------------------//
	CScene( io::CFileSystem* fs );
	CScene( const std::string &name, io::CFileSystem* fs );
	virtual ~CScene();

	// ------------------------------------------------------------------//
	void 							Update( f32 dt, void *userData );
	void 							Render( CDriver *driver );

	// ------------------------------------------------------------------//
	void							Add( CSceneNode* node );
	void							Del( CSceneNode* node );

    virtual bool    				Load( io::CMemStream* ms );
    virtual bool    				Save( io::CMemStream* ms );
	// ------------------------------------------------------------------//

protected:
	// ------------------------------------------------------------------//
	void							Init( io::CFileSystem* fs );
	// ------------------------------------------------------------------//
	stl::COctree					m_octree;

    THierarchy                      m_hierarchy;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace ren
}; // namespace sim
#endif // __SIM_SCENE_H
