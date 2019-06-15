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

	return pArray[Value(prio)];
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
		void *ret = nullptr;

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
