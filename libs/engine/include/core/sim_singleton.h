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

#ifndef __SIM_CSINGLETON_H
#define __SIM_CSINGLETON_H

#include <core/sim_core.h>

namespace sim
{
namespace stl
{
// ----------------------------------------------------------------------//

template <typename T>
class CSingleton
{
protected:
	static T* m_Singleton;

public:
	// ------------------------------------------------------------------//
	CSingleton()
	{
		SIM_ASSERT( !m_Singleton );
		m_Singleton = ( T* )( ( size_t ) this + ( ( size_t )( T* ) 1 - ( size_t )( CSingleton < T > * )( T * ) 1 ) );
	}

	~CSingleton()
	{
		SIM_ASSERT( m_Singleton );
		m_Singleton = nullptr;
	}
	// ------------------------------------------------------------------//
	static T& GetSingleton()
	{
		SIM_ASSERT( m_Singleton );
		return ( *m_Singleton);
	}

	static T* GetSingletonPtr()
	{
		return m_Singleton;
	}
	// ------------------------------------------------------------------//
};

template <typename T>
T* CSingleton< T >::m_Singleton = nullptr;

// ----------------------------------------------------------------------//
} // namespace stl;
} // namespace sim;

#endif // __SIM_CSINGLETON_H
