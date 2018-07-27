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

#ifndef __SIM_CMUTEX_H_
#define __SIM_CMUTEX_H_

#include <core/sim_core.h>

namespace sim
{
namespace sys
{
// ----------------------------------------------------------------------//

class CMutex
{
protected:
    pthread_mutex_t      *m_handle;

public:
    CMutex();
    virtual ~CMutex();

    bool Lock();
    bool Unlock();
};

// ----------------------------------------------------------------------//

class CMutexScope
{
protected:
    CMutex				*m_mutex;

public:
    CMutexScope( CMutex *m_mutex );
    virtual ~CMutexScope();
};

// ----------------------------------------------------------------------//
}; // namespace sys
}; // namespace sim

#endif // __SIM_MUTEX_H_

