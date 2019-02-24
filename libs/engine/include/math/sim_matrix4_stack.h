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
