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
