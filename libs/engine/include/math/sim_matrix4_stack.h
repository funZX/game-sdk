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

#ifndef __SIM_MATRIX4_STACK_H
#define __SIM_MATRIX4_STACK_H

#include <math/sim_math.h>
#include <math/sim_matrix4.h>

namespace sim
{
namespace mat
{
// ----------------------------------------------------------------------//
static const u32 Matrix4StackMaxSize	= 32;
// ----------------------------------------------------------------------//

struct TMatrix4Stack
{
	TMatrix4 	 matrices[ Matrix4StackMaxSize ];
	TMatrix4*	 topmatrix;
	u32	 		 top;
};

static inline void Matrix4StackClear( TMatrix4Stack *mstack )
{
	mstack->topmatrix 	= &mstack->matrices[ 0 ];
	mstack->top 		= 0;

	Matrix4ToIdentity( mstack->topmatrix );
}

static inline void Matrix4StackPush( TMatrix4Stack *_this )
{
	if( _this->top < Matrix4StackMaxSize )
	{
		++_this->top;

		_this->topmatrix = &_this->matrices[ _this->top ];

		Matrix4Copy( _this->topmatrix, &_this->matrices[ _this->top - 1 ] );
	}
}

static inline void Matrix4StackPop( TMatrix4Stack *_this )
{
	if( _this->top > 0 )
	{
		--_this->top;

		_this->topmatrix = &_this->matrices[ _this->top ];
	}
}

// ----------------------------------------------------------------------//
}; // namespace mat
}; // namespace sim

#endif // __SIM_MATRIX4_STACK_H
