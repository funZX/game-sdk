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

#include <core/sim_shellsort.h>

namespace sim
{
// ----------------------------------------------------------------------//

void ShellSort( ISortable *base, s32 count )
{
	ISortable *pj;
	ISortable *pjg;

	u32 elementSize = sizeof( base[ 0 ] );
	s8 *memblock = SIM_NEW s8[ elementSize ];

	for( s32 gap = count >> 1; 0 < gap; gap >>= 2 )
		for( s32 i = gap; i < count; i++ )
			for( s32 j = i - gap; 0 <= j; j -= gap )
			{
				s8* b = reinterpret_cast< s8* >( base );
				pj  = ( ISortable* ) ( b + ( j * elementSize ) );
				pjg = ( ISortable* ) ( b + ( ( j + gap ) * elementSize ) );

				if( pjg->Compare( pj ) < 0)
				{
					SIM_MEMCPY( memblock, pj, elementSize );
					SIM_MEMCPY( pj, pjg, elementSize );
					SIM_MEMCPY( pjg, memblock, elementSize );
				}
				else
					break;
			}

	SIM_SAFE_DELETE_ARRAY( memblock );
}

// ----------------------------------------------------------------------//
}; // namespace sim
