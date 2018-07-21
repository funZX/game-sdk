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

	typedef enum
	{
		k_Priority_Lowest	= 0,
		k_Priority_Low		= 1,
		k_Priority_Normal	= 2,
		k_Priority_High		= 3,
		k_Priority_Highest	= 4,

	} K_PRIORITY;

	// ------------------------------------------------------------------//
	CThread( RunFunc runFunc, void *arg, K_PRIORITY prio );
	virtual ~CThread();

	void				Start();
	void				Stop();
	static void			Wait( u64 micro );

	inline bool			IsRunning() { return m_isRunning; }

	// ------------------------------------------------------------------//
protected:
	s32					GetSystemPrio( K_PRIORITY prio );
	// ------------------------------------------------------------------//

protected:
    pthread_t			m_thread;
    pthread_attr_t		m_attr;

    K_PRIORITY			m_priority;
	bool				m_isRunning;
	RunFunc				m_runFunc;
	void*				m_arg;
};

// ----------------------------------------------------------------------//
}; // namespace sys
}; // namespace sim

#endif // __SIM_CTHREAD_H
