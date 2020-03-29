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

#ifndef __SIM_ACTOR_H
#define __SIM_ACTOR_H

#include <render/sim_render.h>
#include <render/scene/sim_scene_node.h>

class btCollisionShape;
class btRigidBody;

namespace sim
{
// ----------------------------------------------------------------------//
namespace phy { class CPhysic; };
// ----------------------------------------------------------------------//

namespace ren
{
// ----------------------------------------------------------------------//

class CMaterial;
class CMesh;
class CTexture;
class CEffect;
class CCamera;

class CActor : public CSceneNode
{
public:
	CActor();
	CActor( const std::string &name );
	virtual ~CActor();

	// ------------------------------------------------------------------//

	void						BindSize();

	void						AddPhysic(phy::CPhysic* physic);
	void						DelPhysic(phy::CPhysic* physic);

	void						Update( f32 dt, void *userData  );
	void						Render( CDriver *driver );

    void						SetMesh( CMesh *mesh );
	inline CMesh*				GetMesh()						{ return m_mesh; }

	inline btRigidBody*			GetRigidBody()					{ return m_rigidBody; }
	inline btCollisionShape*	GetCollisionShape()				{ return m_collisionShape; }
	// ------------------------------------------------------------------//

protected:
	CMesh*						m_mesh;

	btCollisionShape*			m_collisionShape;
	btRigidBody*				m_rigidBody;

    // ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace ren
}; // namespace sim

#endif // __SIM_ACTOR_H
