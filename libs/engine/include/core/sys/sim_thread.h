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

#ifndef __SIM_CTHREAD_H
#define __SIM_CTHREAD_H

#include <core/sim_core.h>


namespace sim
{
namespace sys
{
// ----------------------------------------------------------------------//

class CThread
{
public:
	// ------------------------------------------------------------------//

	typedef void* (*RunFunc) ( void *pParam );

	// ------------------------------------------------------------------//

	enum class Priority : u32
	{
		Lowest	= 0,
		Low		= 1,
		Normal	= 2,
		High	= 3,
		Highest	= 4,

	};

	// ------------------------------------------------------------------//
	CThread( RunFunc runFunc, void *arg, Priority prio );
	virtual ~CThread();

	void				Start();
	void				Stop();
	static void			Wait( u64 micro );

	inline bool			IsRunning() { return m_isRunning; }

	// ------------------------------------------------------------------//
protected:
	s32					GetSystemPrio( Priority prio );
	// ------------------------------------------------------------------//

protected:
    pthread_t			m_thread;
    pthread_attr_t		m_attr;

	Priority			m_priority;
	bool				m_isRunning;
	RunFunc				m_runFunc;
	void*				m_arg;
};

// ----------------------------------------------------------------------//
}; // namespace sys
}; // namespace sim

#endif // __SIM_CTHREAD_H
