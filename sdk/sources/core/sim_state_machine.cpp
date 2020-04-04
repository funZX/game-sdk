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

CStateMachine::CStateMachine( ren::CCanvas* canvas, IState* initState ) : stl::CStack<IState*>()
{
    m_crtState      = initState;
    m_updateState   = initState;
    m_renderState   = initState;
    m_popState      = nullptr;

    initState->OnEnter();
    Push( initState );

    canvas->OnGui.Connect( this, &CStateMachine::OnGui);
}

// ----------------------------------------------------------------------//

CStateMachine::~CStateMachine()
{
	PopAll();
}

// ----------------------------------------------------------------------//

void CStateMachine::GoNext( IState* state )
{
    m_updateState   = state;
    Push( state );
}

// ----------------------------------------------------------------------//

void CStateMachine::GoPop( IState* state )
{
    m_popState = state;
}
// ----------------------------------------------------------------------//

void CStateMachine::GoBack()
{
	Pop();
    m_updateState   = *Top();
}

// ----------------------------------------------------------------------//

void CStateMachine::PopAll()
{
	while ( Count() )
	{
        IState* p = *Top();
        SIM_SAFE_DELETE(p);

		Pop();
	}
}

// ----------------------------------------------------------------------//

void CStateMachine::Update( f32 dt, void *userData )
{
    SIM_ASSERT( m_updateState );

    if ( m_popState != nullptr )
    {
        IState* p = *Top();
        SIM_SAFE_DELETE( p );
        Pop();

        GoNext( m_popState );

        m_popState      = nullptr;
        m_renderState   = nullptr;
    }

    if ( m_updateState != m_renderState )
        m_updateState->OnEnter();

    m_crtState = m_updateState;
    m_crtState->Update( dt, userData );

    m_renderState = m_crtState;
}

// ----------------------------------------------------------------------//

void CStateMachine::Render( ren::CDriver *driver )
{
    SIM_ASSERT( m_renderState );

    m_renderState->Render( driver );

    if ( m_updateState != m_renderState || m_popState != nullptr)
        m_renderState->OnExit();
}

// ----------------------------------------------------------------------//

void CStateMachine::OnGui( ren::CCanvas* canvas, sigcxx::SLOT slot )
{
    m_updateState->ShowGui( canvas );
    m_renderState = m_crtState;
}
// ----------------------------------------------------------------------//
}; // namespace sm
}; // namespace sim
