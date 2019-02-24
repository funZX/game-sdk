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
	virtual ~CStateMachine();

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
