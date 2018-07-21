#include "nvcore/Ptr.h"
#include "nvmath/Color.h"
#include "nvimage/Image.h"
#include "nvimage/ImageIO.h"
#include "nvimage/NormalMap.h"

#include "MipImage.h"

#include <math.h>

void MipImage::doMipmap( InParams *inParams, OutParams *outParams, float &alphaCoverage, int level )
{
	uint scale = 1 << level;

	outParams->width	= width() / scale;
	outParams->height	= height() / scale;
	outParams->level	= level;

	nv::AutoPtr<nv::FloatImage> fresult(resize(*inParams->exportFilter, outParams->width, outParams->height, inParams->wrapMode));

	if( inParams->alphaTestValue != 1.0f && alphaCoverage != 1.0f && inParams->keepAlpha )
	{
		if( level == 0 ) {
			alphaCoverage = fresult->alphaTestCoverage( inParams->alphaTestValue, 3 );
		}
		else {
			fresult->scaleAlphaToCoverage( alphaCoverage, inParams->alphaTestValue, 3 );
		}
	}

	if( inParams->exportType == e_TextureNormal )
	{
		nv::normalizeNormalMap( fresult.ptr() );
		outParams->image = fresult->createImageGammaCorrect( 1.0f );
	}
	else
		outParams->image = fresult->createImageGammaCorrect( inParams->outputGamma );

	outParams->image->setFormat(nv::Image::Format_ARGB);
	outParams->memSize = fresult->componentNum() * outParams->width * outParams->height;
}

void MipImage::toRGB8( unsigned char **buf, int &size )
{
	int newSize					= ( size >> 2 ) * 3;
	unsigned char *converted	= new unsigned char[ newSize ];
	unsigned char *src			= *buf;

	for( int i = 0, j = 0; i < newSize; i += 3, j += 4 )
	{
		converted[ i + 0 ] = src[ j + 0 ];
		converted[ i + 1 ] = src[ j + 1 ];
		converted[ i + 2 ] = src[ j + 2 ];
	}

	SAFE_DEL( src );

	size = newSize;
	*buf = converted;
}

void MipImage::toRGBA4( unsigned char **buf, int &size )
{
	int newSize					= size >> 1;
	unsigned char *converted	= new unsigned char[ newSize ];
	unsigned char *src			= *buf;

	for( int i = 0, j = 0; i < newSize; i += 2, j+=4 )
	{
		unsigned short *dstShort	= ( unsigned short* ) &converted[ i ];
		unsigned int   *srcInt		= ( unsigned int* ) &src[ j ];

		*dstShort = cc::toRGBA4( *srcInt );
	}

	SAFE_DEL( src );

	size = newSize;
	*buf = (unsigned char *) converted;
}

void MipImage::toRGB565( unsigned char **buf, int &size )
{
	int newSize					= size >> 1;
	unsigned char *converted	= new unsigned char[ newSize ];
	unsigned char *src			= *buf;

	for( int i = 0, j = 0; i < newSize; i += 2, j+=4 )
	{
		unsigned short *dstShort	= ( unsigned short* ) &converted[ i ];
		unsigned int   *srcInt		= ( unsigned int* ) &src[ j ];

		*dstShort = cc::toRGB565( *srcInt );
	}

	SAFE_DEL( src );

	size = newSize;
	*buf = (unsigned char *) converted;
}

void MipImage::toRGB5A1( unsigned char **buf, int &size )
{
	int newSize					= size >> 1;
	unsigned char *converted	= new unsigned char[ newSize ];
	unsigned char *src			= *buf;

	for( int i = 0, j = 0; i < newSize; i += 2, j+=4 )
	{
		unsigned short *dstShort	= ( unsigned short* ) &converted[ i ];
		unsigned int   *srcInt		= ( unsigned int* ) &src[ j ];

		*dstShort = cc::toRGB5A1( *srcInt );
	}

	SAFE_DEL( src );

	size = newSize;
	*buf = (unsigned char *) converted;
}


void MipImage::convert( InParams *inParams, unsigned char **buf, int &size )
{
	switch( inParams->exportFormat )
	{
	case FORMAT_RGBA8:
		{
			if( !inParams->keepAlpha )
			{
				toRGB8( buf, size );

				inParams->exportFormat = FORMAT_RGB8;
			}
		}
		break;

	case FORMAT_RGB8:
		{
			toRGB8( buf, size );
		}
		break;

	case FORMAT_RGBA4:
		{
			if( inParams->keepAlpha ) {
				toRGBA4( buf, size );
			}
			else {
				toRGB565( buf, size );
				inParams->exportFormat = FORMAT_RGB565;
			};
		}
		break;

	case FORMAT_RGB565:
		{
			toRGB565( buf, size );
		}
		break;

	case FORMAT_RGB5A1:
		{
			if( inParams->keepAlpha ) {
				toRGB5A1( buf, size );
			}
			else {
				toRGB565( buf, size );
				inParams->exportFormat = FORMAT_RGB565;
			}
		}
		break;
	}
}

unsigned char* MipImage::process( InParams *inParams, int &size, int &numMipmaps )
{
	OutParams mipParams[ 32 ];
	float alphaCoverage = 0.0f;

	if( inParams->exportFlipped )
		flip();

	if( inParams->exportType != e_TextureNormal )
		toLinear( 0, 3, inParams->inputGamma );

	if( inParams->exportPreScale != 1.0f || inParams->exportPreBias != 0.0f )
		scaleBias( 0, 3, inParams->exportPreScale, inParams->exportPreBias );

	if( inParams->exportRedScale != 1.0f || inParams->exportRedBias != 0.0f )
		scaleBias( 2, 1, inParams->exportRedScale, inParams->exportRedBias );

	if( inParams->exportGreenScale != 1.0f || inParams->exportGreenBias != 0.0f )
		scaleBias( 1, 1, inParams->exportGreenScale, inParams->exportGreenBias );

	if( inParams->exportBlueScale != 1.0f || inParams->exportBlueBias != 0.0f )
		scaleBias( 0, 1, inParams->exportBlueScale, inParams->exportBlueBias );

	if( inParams->exportPostScale != 1.0f || inParams->exportPostBias != 0.0f )
		scaleBias( 0, 3, inParams->exportPostScale, inParams->exportPostBias );

	size = 0;
	numMipmaps = inParams->exportMipmaps ? countMipmaps( width(), height() ) : 1;

	doMipmap( inParams, &mipParams[0], alphaCoverage, 0 ); // level 0, compute alpha coverage for alpha mipmaps
	size += mipParams[ 0 ].memSize;

#ifdef _SAVE_MIPMAPS_AS_TGA
	nv::Path outMipmap;
	outMipmap.format( "%s_%dx%d.tga", inParams->outImage.str(), mipParams[0].width, mipParams[0].height);
	nv::StdOutputStream stream(outMipmap.str());
	nv::ImageIO::save(outMipmap.str(), stream, mipParams[0].image.ptr());
#endif

	for( int k = 1; k < numMipmaps; k++ )
	{
		doMipmap( inParams, &mipParams[k], alphaCoverage, k );
		size += mipParams[ k ].memSize;

#ifdef _SAVE_MIPMAPS_AS_TGA
		nv::Path outMipmap;
		outMipmap.format( "%s_%dx%d.tga", inParams->outImage.str(), mipParams[k].width, mipParams[k].height);
		nv::StdOutputStream stream(outMipmap.str());
		nv::ImageIO::save(outMipmap.str(), stream, mipParams[k].image.ptr());
#endif
	}

	unsigned char	*buf = new unsigned char[ size ];
	unsigned char	*pos = buf;

	for( int k = 0; k < numMipmaps; k++ )
	{
		memcpy( pos, mipParams[k].image->pixels(), mipParams[k].memSize );
		pos += mipParams[k].memSize;
	}

	convert( inParams, &buf, size );

	return buf;
}

int	MipImage::getPixelSize( int format )
{
	int pixelSize = 16;

	if( format == FORMAT_RGB8 )
		pixelSize = 24;

	if( format == FORMAT_RGBA8 )
		pixelSize = 32;

	return pixelSize;
}

int	MipImage::getBits( int format )
{
	int bits = 4;

	if( format == FORMAT_RGB565 || format == FORMAT_RGB8 )
		bits = 3;

	return bits;
}

bool MipImage::Save( InParams *inParams )
{
	OutParams outParams;
	int w					= width();
	int h					= height();
	int size				= 0;
	int nMipmaps			= 0;
	unsigned char *buf		= process( inParams, size, nMipmaps );
	int format				= inParams->exportFormat;

	TMIPHeader header;

	header.dwWidth		= w;
	header.dwHeight		= h;
	header.dwBits		= getBits( format );
	header.dwPixelSize	= getPixelSize( format );
	header.dwFormat		= format;
	header.dwDataSize	= size;
	header.dwNumMipmaps	= nMipmaps;

	FILE *file= fopen( inParams->outImage.str(), "wb" );

	if( file )
	{
		fwrite( &header, sizeof( TMIPHeader ), 1, file );
		fwrite( buf, 1, size, file );

		fclose( file );
		SAFE_DEL( buf );

		return true;
	}

	SAFE_DEL( buf );
	return false;
}


