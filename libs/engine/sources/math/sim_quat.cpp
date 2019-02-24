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

#include <math/sim_quat.h>

namespace sim
{
namespace mat
{
// ----------------------------------------------------------------------//

void QuatSlerp( const TQuat *_this, TQuat *q, const f32 t, TQuat *dst )
{
	if( t <= 0.0f )	{ QuatCopy( dst, _this ); return; }
	if( t >= 1.0f )	{ QuatCopy( dst, q ); return; }

	f32 cosOmega = _this->x * q->x + _this->y * q->y + _this->z * q->z + _this->w * q->w;
	f32 k0, k1;

	if( cosOmega < 0.0f )
	{
		q->x = -q->x;
		q->y = -q->y;
		q->z = -q->z;
		q->w = -q->w;
		cosOmega = -cosOmega;
	}

	if(cosOmega > 0.9999f)
	{
		k0 = 1.0f - t;
		k1 = t;
	}
	else
	{
		const f32 omega = acosf(cosOmega);
		const f32 sinOmega = sinf(omega);
		k0 = sinf( ( 1.0f - t ) * omega ) / sinOmega;
		k1 = sinf( t * omega ) / sinOmega;
	}

	dst->x = k0 * _this->x + k1 * q->x;
	dst->y = k0 * _this->y + k1 * q->y;
	dst->z = k0 * _this->z + k1 * q->z;
	dst->w = k0 * _this->w + k1 * q->w;
}

// ----------------------------------------------------------------------//

void QuatSlerp( const TQuat *_this, TQuat *q, const f32 t, f32 *_cosOmega,  TQuat *dst )
{
	if( t <= 0.0f )	{ QuatCopy( dst, _this ); return; }
	if( t >= 1.0f )	{ QuatCopy( dst, q ); return; }

	f32 cosOmega = _this->x * q->x + _this->y * q->y + _this->z * q->z + _this->w * q->w;
	f32 k0, k1;

	if( cosOmega < 0.0f )
	{
		q->x = -q->x;
		q->y = -q->y;
		q->z = -q->z;
		q->w = -q->w;
		cosOmega = -cosOmega;
	}

	*_cosOmega = cosOmega;

	if( cosOmega > 0.9999f )
	{
		k0 = 1.0f - t;
		k1 = t;
	}
	else
	{
		const f32 omega = acosf( cosOmega );
		const f32 sinOmega = sinf( omega );
		k0 = sinf( ( 1.0f - t ) * omega ) / sinOmega;
		k1 = sinf( t * omega ) / sinOmega;
	}

	dst->x = k0 * _this->x + k1 * q->x;
	dst->y = k0 * _this->y + k1 * q->y;
	dst->z = k0 * _this->z + k1 * q->z;
	dst->w = k0 * _this->w + k1 * q->w;
}

// ----------------------------------------------------------------------//
}; // namespace mat
}; // namespace sim
