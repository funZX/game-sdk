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
