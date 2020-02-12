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

#include <core/sim_state_machine.h>
#include <render/sim_driver.h>
#include <render/sim_canvas.h>

namespace sim
{
namespace sm
{
// ----------------------------------------------------------------------//

CStateMachine::CStateMachine( rnr::CCanvas* canvas ) : stl::CStack<IState*>()
{
	m_prevState	= nullptr;
	m_currState = nullptr;
	m_nextState = nullptr;

    canvas->OnGui.Connect( this, &CStateMachine::ShowGui );
}

// ----------------------------------------------------------------------//

CStateMachine::~CStateMachine()
{
	while( Count() > 0 )
	{
		IState* top = *Top();

		SIM_SAFE_DELETE(top);
		Pop();
	}
}

// ----------------------------------------------------------------------//

void CStateMachine::GoNext( IState* state )
{
	m_prevState = m_currState;
	if ( m_prevState )
		m_prevState->OnExit();

	m_nextState = state;
	if ( m_nextState )
		m_nextState->OnEnter();

	Push( state );
}

// ----------------------------------------------------------------------//

void CStateMachine::GoBack()
{
	m_prevState = m_currState;
	Pop();
	m_nextState = *Top();
}

void CStateMachine::PopAll()
{
	while( Count() )
		Pop();
}

// ----------------------------------------------------------------------//

void CStateMachine::Update( f32 dt, void *userData )
{
	if (m_nextState != nullptr)
	{
		m_prevState = m_currState;
		m_currState = m_nextState;
		m_nextState = nullptr;
	}

	m_currState->Update( dt, userData );
}

// ----------------------------------------------------------------------//

void CStateMachine::Render( rnr::CDriver *driver )
{
	m_currState->Render( driver );
}

// ----------------------------------------------------------------------//

void CStateMachine::ShowGui( rnr::CCanvas* canvas, sigcxx::SLOT slot )
{
    if ( m_currState )
        m_currState->ShowGui( canvas );
}
// ----------------------------------------------------------------------//
}; // namespace sm
}; // namespace sim
