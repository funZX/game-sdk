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

#include <core/sim_utf.h>

namespace sim
{
// ----------------------------------------------------------------------//
bool UTF16ToUTF8( u8 *dst, s32 *dstLen, const u16 *src, s32 srcLen )
{
	static u8 kUtf8Limits[5] = { 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

	size_t dstPos	= 0;
	size_t srcPos	= 0;

	SIM_ASSERT( dst != nullptr );

	while( true )
	{
		u32 numAdds;
		u32 value;
		
		if( srcPos == srcLen )
		{
			*dstLen = dstPos;
			return true;
		}
		
		value = src[ srcPos++ ];
		
		if( value < 0x80 )
		{
			dst[ dstPos++ ] = (s8) value;
			continue;
		}
		
		if( value >= 0xD800 && value < 0xE000 )
		{
			u32 c2;
			
			if( value >= 0xDC00 || srcPos == srcLen )
				break;

			c2 = src[ srcPos++ ];
			
			if( c2 < 0xDC00 || c2 >= 0xE000 )
				break;
			
			value = ( ( ( value - 0xD800 ) << 10) | ( c2 - 0xDC00 ) ) + 0x10000;
		}
		
		for( numAdds = 1; numAdds < 5; numAdds++ )
			if( value < ( ( ( u32 ) 1 ) << ( numAdds * 5 + 6 ) ) )
				break;
		
		dst[ dstPos++ ] = (char)( kUtf8Limits[ numAdds - 1 ] + ( value >> ( 6 * numAdds ) ) );
		
		do
		{
			numAdds--;
			dst[ dstPos++ ] = (char)( 0x80 + ( ( value >> ( 6 * numAdds ) ) & 0x3F ) );
		}
		while( numAdds != 0 );
	}
	
	*dstLen = dstPos;

	return false;
}

// ----------------------------------------------------------------------//
}; // namespace sim;
