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

#include <core/sim_state_machine.h>
#include <render/sim_driver.h>

namespace sim
{
namespace sm
{
// ----------------------------------------------------------------------//

CStateMachine::CStateMachine() : stl::CStack<IState*>()
{
	m_prevState	= NULL;
	m_currState = NULL;
	m_nextState = NULL;
}

// ----------------------------------------------------------------------//

CStateMachine::~CStateMachine()
{
	while( Count() > 0 )
	{
		SIM_SAFE_DELETE(*Top());
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
	if (m_nextState != NULL)
	{
		m_prevState = m_currState;
		m_currState = m_nextState;
		m_nextState = NULL;
	}

	m_currState->Update( dt, userData );
}

// ----------------------------------------------------------------------//

void CStateMachine::Render2D( rnr::CDriver *driver )
{
	rnr::CDriver::K_SELECT_BATCH batchSelect =
		driver->SelectBatch(rnr::CDriver::k_Select_Batch_2D);

	m_currState->Render2D( driver );
	
	driver->SelectBatch(batchSelect);
}

// ----------------------------------------------------------------------//

void CStateMachine::Render3D( rnr::CDriver *driver )
{
	m_currState->Render3D( driver );
}

// ----------------------------------------------------------------------//
}; // namespace sm
}; // namespace sim
