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
	m_graph.Insert( node );
}
// ----------------------------------------------------------------------//

void CScene::Del( CSceneNode* node )
{
	m_graph.Delete(node);
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
