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
