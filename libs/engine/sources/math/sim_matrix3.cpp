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

#include <math/sim_matrix3.h>

namespace sim
{
namespace mat
{
// ----------------------------------------------------------------------//

void Matrix3Multiply( TMatrix3 *_this, const TMatrix3 *src )
{
	TMatrix3 tmp;

	for( u32 i = 0; i < 3; i++ )
		for( u32 j = 0; j < 3; j++ )
			tmp.raw[ i * 3 + j ] =  _this->raw[ i * 3 + 0 ] * src->raw[ 0 * 3 + j ] +
								 	_this->raw[ i * 3 + 1 ] * src->raw[ 1 * 3 + j ] +
								 	_this->raw[ i * 3 + 2 ] * src->raw[ 2 * 3 + j ];

	Matrix3Copy( _this, &tmp );
}

// ----------------------------------------------------------------------//

void Matrix3Multiply( const TMatrix3 *_this, const TMatrix3 *src, TMatrix3 *dst )
{
	SIM_ASSERT( _this != dst );

	for( u32 i = 0; i < 3; i++ )
		for( u32 j = 0; j < 3; j++ )
			dst->raw[ i * 3 + j ] =	 _this->raw[ i * 3 + 0 ] * src->raw[ 0 * 3 + j ] +
									 _this->raw[ i * 3 + 1 ] * src->raw[ 1 * 3 + j ] +
									 _this->raw[ i * 3 + 2 ] * src->raw[ 2 * 3 + j ];
}

// ----------------------------------------------------------------------//

void Matrix3TransformVec3( const TMatrix3 *_this, const TVec3 *src, TVec3 *dst )
{
	TVec3 tmp;

	tmp.x =		_this->raw[ 0 ] * src->x +
				_this->raw[ 3 ] * src->y +
				_this->raw[ 6 ] * src->z;

	tmp.y =		_this->raw[ 1 ] * src->x +
				_this->raw[ 4 ] * src->y +
				_this->raw[ 7 ] * src->z;

	tmp.z =		_this->raw[ 2 ] * src->x +
				_this->raw[ 5 ] * src->y +
				_this->raw[ 8 ] * src->z;

	Vec3Copy( dst, &tmp );
}

// ----------------------------------------------------------------------//

void Matrix3Print( const TMatrix3 *_this )
{
	SIM_PRINT( "\n" );
	for( s32 i = 0; i < 3; i++ )
	{
		SIM_PRINT( "\n" );
		for( s32 j = 0; j < 3; j++ )
		{
			SIM_PRINT( "%.3f\t", _this->raw[ j + i * 3 ] );
		}
	}
	SIM_PRINT( "\n" );
}

// ----------------------------------------------------------------------//
}; // namespace mat
}; // namespace sim
