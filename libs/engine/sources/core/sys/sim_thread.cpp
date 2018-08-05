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

#include <core/sys/sim_thread.h>

namespace sim
{
namespace sys
{
// ----------------------------------------------------------------------//

CThread::CThread( RunFunc runFunc, void *arg, Priority prio )
{
	m_isRunning = false;
	m_runFunc	= runFunc;
	m_arg		= arg;
	m_priority	= prio;

    pthread_attr_init( &m_attr );

    if( prio != Priority::Normal )
    {
		struct sched_param param;
		SIM_MEMSET( &param, 0, sizeof( param ) );
		param.sched_priority =  GetSystemPrio( prio );

		pthread_attr_setinheritsched( &m_attr, PTHREAD_EXPLICIT_SCHED );
		pthread_attr_setschedpolicy( &m_attr, SCHED_FIFO );
		pthread_attr_setscope( &m_attr, PTHREAD_SCOPE_PROCESS );

		pthread_attr_setschedparam( &m_attr, &param );
    }
}

// ----------------------------------------------------------------------//

CThread::~CThread()
{
	Stop();
}

// ----------------------------------------------------------------------//

s32	CThread::GetSystemPrio( Priority prio )
{
	s32 minPrio		= sched_get_priority_min( SCHED_FIFO );
	s32 maxPrio		= sched_get_priority_max( SCHED_FIFO );

	s32 normalPrio	= minPrio + ( ( maxPrio - minPrio ) >> 1 );
	s32 lowPrio		= minPrio + ( ( maxPrio - minPrio ) >> 2 );
	s32 highPrio	= minPrio + ( ( maxPrio - minPrio ) >> 2 ) * 3;

	const s32 pArray[] = { minPrio, lowPrio, normalPrio, highPrio, maxPrio };

	return pArray[EnumValue(prio)];
}

// ----------------------------------------------------------------------//

void CThread::Start()
{
	if( !m_isRunning )
	{
		m_isRunning = true;
		pthread_create( &m_thread, &m_attr, m_runFunc, m_arg );
	}
}

// ----------------------------------------------------------------------//

void CThread::Stop()
{
	if( m_isRunning )
	{
		void *ret;

		m_isRunning = false;
		pthread_join( m_thread, &ret );
	}
}

// ----------------------------------------------------------------------//

void CThread::Wait( u64 micro )
{
	sys::Wait( micro );
}

// ----------------------------------------------------------------------//
}; // namespace sys
}; // namespace sim;
