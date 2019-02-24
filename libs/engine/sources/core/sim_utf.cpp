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
