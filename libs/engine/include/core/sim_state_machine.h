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

#ifndef __SIM_STATE_MACHINE_H
#define __SIM_STATE_MACHINE_H

#include <core/sim_core.h>
#include <core/sim_stack.h>
#include <core/sim_interfaces.h>

namespace sim
{
namespace sm
{
// ----------------------------------------------------------------------//

class CStateMachine : public stl::CStack < IState* >, public sim::IUpdatable
{
protected:
	IState*				m_prevState;
	IState*				m_currState;
	IState*				m_nextState;

public:
	CStateMachine();
	~CStateMachine();

	IState*				GetCurrentState()			{ m_currState; }

	void				Update( f32 dt, void *userData );

	void				Render2D( rnr::CDriver *driver );
	void				Render3D( rnr::CDriver *driver );

	void				GoNext( IState* state );
	void				GoBack();

	void				PopAll();
};

// ----------------------------------------------------------------------//
} // namespace sm;
} // namespace sim;

#endif // __SIM_CSTACK_H
