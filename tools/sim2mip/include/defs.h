#ifndef __DEF_H_
#define __DEF_H_

#ifdef _DEBUG
#define _SAVE_MIPMAPS_AS_TGA
#endif

#pragma warning ( disable : 4996 )

#define SAFE_DEL(a)					{if(a){delete (a);a=NULL;}}
#define SAFE_DEL_ARRAY(a)			{if(a){delete[] (a);a=NULL;}}
#define SAFE_DEL_ARRAY_TYPE(a,t)	{if((t)a){delete[] ((t)a);a=NULL;}}

#define MIPMAP_MAX_NUM				(16)
#define MIPMAP_SIZE_MIN				(1)

//all formats
#define GL_RGB                       0x1907
#define GL_RGBA                      0x1908
#define GL_RGBA4                     0x8056
#define GL_RGB5_A1                   0x8057
#define GL_RGB565                    0x8D62				

#define FORMAT_RGBA8				(GL_RGBA)
#define FORMAT_RGB8					(GL_RGB)

#define FORMAT_RGBA4				(GL_RGBA4)
#define FORMAT_RGB565				(GL_RGB565)
#define FORMAT_RGB5A1				(GL_RGB5_A1)

#define MAX( a, b )		( ( a ) > ( b ) ? ( a ) : ( b ) )
#define MIN( a, b )		( ( a ) < ( b ) ? ( a ) : ( b ) )

static inline uint nextPowerOfTwo( int x )
{
	x--;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	return x+1;	
}

static inline bool isPowerOfTwo( int n )
{
	return (n & (n-1)) == 0;
}

static inline int countMipmaps(int w, int h)
{
	int mipmap	= 0;
	int minsize = MAX( w, h);

	while (w >= 1 && h >= 1 && mipmap < MIPMAP_MAX_NUM && minsize >= MIPMAP_SIZE_MIN ) 
	{
		w = w >> 1;
		h = h >> 1;
		
		minsize = MIN( w, h );

		mipmap++;
	}

	return mipmap;
}

namespace cc // color conversion
{
	static inline unsigned short toRGB565( int u )
	{
		return 
			((u & 0x00F80000) >> 19) | 
			((u & 0x0000FC00) >> 5) | 
			((u & 0x000000F8) << 8);		
	}

	static inline unsigned short toRGBA4( int u )
	{
		return 
			((u & 0xF0000000) >> 28) |
			((u & 0x00F00000) >> 16) |
			((u & 0x0000F000) >> 4) |
			((u & 0x000000F0) << 8);		
	}

	static inline unsigned short toRGB5A1( int u )
	{
		return
			((u & 0x80000000) >> 31) |
			((u & 0x00F80000) >> 18) |
			((u & 0x0000F800) >> 5) |
			((u & 0x000000F8) << 8);	
	}
};

#endif