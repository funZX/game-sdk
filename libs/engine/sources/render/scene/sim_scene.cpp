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

#include <core/io/sim_file_system.h>

#include <render/scene/sim_actor.h>
#include <render/scene/sim_light.h>
#include <render/scene/sim_camera.h>
#include <render/scene/sim_sky_box.h>
#include <render/scene/sim_scene.h>
#include <render/scene/sim_scene_node.h>

#include <render/sim_driver.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CScene::CScene()
{
}
// ----------------------------------------------------------------------//

CScene::CScene( const std::string &name )
	: CScene()
{
	m_name = name;
}

// ----------------------------------------------------------------------//

CScene::~CScene()
{
}

// ----------------------------------------------------------------------//

void CScene::Update( f32 dt, void *userData )
{

}

// ----------------------------------------------------------------------//

void CScene::Render( CDriver *driver )
{

}

// ----------------------------------------------------------------------//

void CScene::Add( CSceneNode* node )
{
	m_octree.Insert( node );
}
// ----------------------------------------------------------------------//

void CScene::Del( CSceneNode* node )
{
	m_octree.Delete( node );
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
