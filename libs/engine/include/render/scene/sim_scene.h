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

#ifndef __SIM_SCENE_H
#define __SIM_SCENE_H

#include <core/sim_interfaces.h>
#include <core/sim_octree.h>

#include <math/sim_vec3.h>
#include <math/sim_vec4.h>

#include <render/sim_render.h>

namespace sim
{
// ----------------------------------------------------------------------//
namespace rnr
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
	CScene();
	CScene( const std::string &name );
	virtual ~CScene();

	// ------------------------------------------------------------------//
	void 							Update( f32 dt, void *userData );
	void 							Render( CDriver *driver );

	// ------------------------------------------------------------------//
	void							Add( CSceneNode* node );
	void							Del( CSceneNode* node );
	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	COctree					m_octree;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_SCENE_H
