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
