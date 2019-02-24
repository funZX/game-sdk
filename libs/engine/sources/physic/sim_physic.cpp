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

#include <btBulletDynamicsCommon.h>

#include <render/scene/sim_actor.h>
#include <physic/sim_physic.h>

namespace sim
{
namespace phy
{
// ----------------------------------------------------------------------//

CPhysic::CPhysic()
{
	m_collisionConfiguration	= SIM_NEW btDefaultCollisionConfiguration();
	m_dispatcher				= SIM_NEW btCollisionDispatcher(m_collisionConfiguration);
	m_broadphase				= SIM_NEW btDbvtBroadphase();
	m_solver					= SIM_NEW btSequentialImpulseConstraintSolver();
	m_dynamicsWorld				= SIM_NEW btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);

	m_dynamicsWorld->setGravity( btVector3(0.0f, -10.0f, 0.0f) );
}

// ----------------------------------------------------------------------//

CPhysic::~CPhysic()
{
	SIM_SAFE_DELETE( m_dynamicsWorld );
	SIM_SAFE_DELETE( m_solver );
	SIM_SAFE_DELETE( m_broadphase );
	SIM_SAFE_DELETE( m_dispatcher );
	SIM_SAFE_DELETE( m_collisionConfiguration );
}

// ----------------------------------------------------------------------//

void CPhysic::Update( f32 dt, void *userData )
{
	m_dynamicsWorld->stepSimulation(btScalar(dt));
	m_dynamicsWorld->debugDrawWorld();
}

// ----------------------------------------------------------------------//

void CPhysic::Add(rnr::CActor* actor)
{
	m_dynamicsWorld->addRigidBody(actor->GetRigidBody());
}

// ----------------------------------------------------------------------//

void CPhysic::Del(rnr::CActor* actor)
{
	m_dynamicsWorld->removeRigidBody(actor->GetRigidBody());
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
