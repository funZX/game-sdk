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

#ifndef __SIM_PHYSIC_H
#define __SIM_PHYSIC_H

#include <core/sim_interfaces.h>

class btDynamicsWorld;
class btBroadphaseInterface;
class btCollisionDispatcher;
class btConstraintSolver;
class btDefaultCollisionConfiguration;

namespace sim
{
// ----------------------------------------------------------------------//
namespace rnr { class CActor; };
// ----------------------------------------------------------------------//
namespace phy
{
// ----------------------------------------------------------------------//

class CPhysic : public IUpdatable
{
public:
	CPhysic();
	virtual ~CPhysic();
	// ------------------------------------------------------------------//
	void 								Update( f32 dt, void *userData );

	void								Add(rnr::CActor* actor);
	void								Del(rnr::CActor* actor);
	// ------------------------------------------------------------------//
protected:
	btDynamicsWorld*					m_dynamicsWorld;
	btBroadphaseInterface*				m_broadphase;
	btCollisionDispatcher*				m_dispatcher;
	btConstraintSolver*					m_solver;
	btDefaultCollisionConfiguration*	m_collisionConfiguration;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace phy
}; // namespace sim

#endif // __SIM_PHYSIC_H
