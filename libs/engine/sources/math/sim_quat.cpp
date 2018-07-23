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
