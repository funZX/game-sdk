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

CStateMachine::CStateMachine( ren::CCanvas* canvas ) : stl::CStack<IState*>()
{
    m_updateState   = nullptr;
    m_renderState   = nullptr;

    m_isPoped       = false;    

    canvas->OnGui.Connect( this, &CStateMachine::ShowGui );
}

// ----------------------------------------------------------------------//

CStateMachine::~CStateMachine()
{
	PopAll();
}

// ----------------------------------------------------------------------//

void CStateMachine::GoNext( IState* state )
{
    m_isPoped = false;

    m_updateState   = state;
    if (m_renderState)
        m_renderState->OnExit( false );
    m_updateState->OnEnter( true );
    Push( state );
}

// ----------------------------------------------------------------------//

void CStateMachine::GoBack()
{
	Pop();
    m_updateState   = *Top();
    if (m_renderState)
        m_renderState->OnExit( true );
    m_updateState->OnEnter( false );

    m_isPoped = true;
}

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

    IState* crtState = m_updateState;	
    crtState->Update( dt, userData );
    m_renderState = crtState;
}

// ----------------------------------------------------------------------//

void CStateMachine::Render( ren::CDriver *driver )
{
    SIM_ASSERT( m_renderState );
    m_renderState->Render( driver );
}

// ----------------------------------------------------------------------//

void CStateMachine::ShowGui( ren::CCanvas* canvas, sigcxx::SLOT slot )
{
    if ( m_updateState )
        m_updateState->ShowGui( canvas );
}
// ----------------------------------------------------------------------//
}; // namespace sm
}; // namespace sim
