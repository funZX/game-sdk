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
