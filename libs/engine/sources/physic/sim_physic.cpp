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
