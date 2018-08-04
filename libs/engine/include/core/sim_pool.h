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

#ifndef __SIM_POOL_H
#define __SIM_POOL_H

#include <core/sim_core.h>

namespace sim
{
namespace stl
{
// ----------------------------------------------------------------------//

template <class T>
class CPool
{
public:
	CPool() : m_cache( nullptr ), m_size( 0 ), m_currUsed( 0 ) {}
	~CPool()  { Release(); }

	// ------------------------------------------------------------------//
	void 						Release()
								{
									SIM_SAFE_DELETE_ARRAY( m_cache );
									m_size 		= 0;
									m_currUsed 	= 0;
								}

	void 						Resize( s32 size )
								{
									if( size > m_size )
									{
										T* newCache = SIM_NEW T[ size ];

										if( newCache == nullptr ) {
											SIM_ASSERT( 0 &&  SIM_PRINT("not enough memory") );
										}

										if( m_currUsed > 0 ) {
											SIM_MEMCPY( newCache, m_cache, m_currUsed * sizeof( T ) );
										}

										Release();

										m_cache 	= newCache;
										m_size 		= size;
									}
								}
	T*							New()
								{
									if( m_currUsed >= m_size)
									{
										SIM_ASSERT( 0 && SIM_PRINT( "No space for another instance!!\n" ) );
										return nullptr;
									}
									return &m_cache[ m_currUsed++ ];
								}

	void 						Delete()        { m_currUsed--; }
	void 						Clear()			{ m_currUsed = 0; }

	T* 							GetCache()      { return m_cache; }
	T& 							Get(s32 index)  { return m_cache[ index ]; }
	T* 							GetPtr(s32 index) { return &m_cache[ index ]; }
	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	T* 							m_cache;
	s32 						m_size;
	s32 						m_currUsed;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
} // namespace stl;
} // namespace sim;
#endif // __SIM_POOL_H
