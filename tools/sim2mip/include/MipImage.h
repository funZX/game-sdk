
#ifndef __IMAGEETC_H
#define __IMAGEETC_H

#include <iostream>
#include "nvcore/StrLib.h"
#include "nvcore/StdStream.h"
#include "nvimage/FloatImage.h"

#include "defs.h"

typedef enum
{
	e_TextureDiffuse,
	e_TextureNormal

} ETextureType;

typedef struct 
{
	nv::Path inImage;					// 
	nv::Path outImage;					// 

	ETextureType exportType;			// -t : diffuse, normal
	bool	exportMipmaps;				// -m : yes, no

	int		exportFormat;				// -f : 
	nv::Filter *exportFilter;			// -k : box, triangle, quadratic, bspline, mitchell, lanczos, kaiser
	bool	exportFlipped;				// -flip
	float   exportPreScale;				// -ps
	float   exportPreBias;				// -pb
	float   exportPostScale;			// -PS
	float   exportPostBias;				// -PB
	float   exportRedScale;				// -rs
	float   exportRedBias;				// -rb
	float   exportGreenScale;			// -gs
	float   exportGreenBias;			// -gb
	float   exportBlueScale;			// -bs
	float   exportBlueBias;				// -bb
	bool	keepAlpha;					

	float	alphaTestValue;				// -av
	float	inputGamma;					// -ig
	float	outputGamma;				// -og
	nv::FloatImage::WrapMode wrapMode;	// -w : clamp, repeat, mirror

} InParams;

typedef struct 
{
	int				width;					
	int				height;
	int				memSize;
	int				level;

	nv::AutoPtr<nv::Image> image;

} OutParams;

typedef struct
{
	int dwWidth;
	int dwHeight;
	int dwBits;
	int dwPixelSize;
	int dwFormat;
	int dwDataSize;
	int dwNumMipmaps;
} TMIPHeader;

class MipImage : public nv::FloatImage
{
protected:
	void			toRGB8( unsigned char **buf, int &size );
	void			toRGBA4( unsigned char **buf, int &size );
	void			toRGB565( unsigned char **buf, int &size );
	void			toRGB5A1( unsigned char **buf, int &size );

	void			doMipmap( InParams *inParams, OutParams *outParams, float &alphaCoverage, int level );
	void			convert( InParams *inParams, unsigned char **buf, int &size );
	unsigned char*	process( InParams *inParams, int &size, int &numMipmaps );

	static int		getPixelSize( int format );
	static int		getBits( int format );
public:
	MipImage( const nv::Image * img ):nv::FloatImage(img){};
	~MipImage(){};

	bool Save( InParams *params );
};

#endif