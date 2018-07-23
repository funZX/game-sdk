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

#include <core/sys/sim_mutex.h>

namespace sim
{
namespace sys
{
// ----------------------------------------------------------------------//

CMutex::CMutex()
{
    m_handle  = SIM_NEW pthread_mutex_t;

    pthread_mutex_init( m_handle, 0 );
}

// ----------------------------------------------------------------------//

CMutex::~CMutex()
{
	if( 0 != pthread_mutex_destroy( m_handle ) )
	{
		Unlock();
		pthread_mutex_destroy( m_handle );
	}

	SIM_SAFE_DELETE( m_handle );
}

// ----------------------------------------------------------------------//

bool CMutex::Lock()
{
    return ( 0 == pthread_mutex_lock( m_handle ) );
}

// ----------------------------------------------------------------------//

bool CMutex::Unlock()
{
    return ( 0 == pthread_mutex_unlock( m_handle ) );
}

// ----------------------------------------------------------------------//

CMutexScope::CMutexScope( CMutex *mutex )
		: m_mutex( mutex )
{
	m_mutex->Lock();
}

// ----------------------------------------------------------------------//

CMutexScope::~CMutexScope()
{
	m_mutex->Unlock();
}

// ----------------------------------------------------------------------//
}; // namespace sys
}; // namespace sim



