#include "nvcore/Ptr.h"
#include "nvcore/StrLib.h"
#include "nvcore/StdStream.h"
#include "nvimage/Image.h"
#include "nvimage/ImageIO.h"
#include "nvimage/Filter.h"
#include "nvimage/FloatImage.h"

#include "MipImage.h"
#include <iostream>
#include <vector>
#include <map>

void			onStart();

const char *	stristr( const char* str, const char* subStr );
const int		getformat( const char *str );
bool			loadImage(nv::Image & image, const char * fileName);

int main(int argc, char** argv)
{
	onStart();

	InParams inParams;

	inParams.exportType = e_TextureDiffuse;
	inParams.exportFilter = NULL;
	inParams.exportMipmaps = false;
	inParams.exportFormat = FORMAT_RGBA8;
	inParams.exportFlipped = false;
	inParams.alphaTestValue = 1.0f;
	inParams.exportPreScale = 1.0f;
	inParams.exportPreBias = 0.0f;
	inParams.exportRedScale = 1.0f;
	inParams.exportRedBias = 0.0f;
	inParams.exportGreenScale = 1.0f;
	inParams.exportGreenBias = 0.0f;
	inParams.exportBlueScale = 1.0f;
	inParams.exportBlueBias = 0.0f;
	inParams.exportPostScale = 1.0f;
	inParams.exportPostBias = 0.0f;
	inParams.keepAlpha = true;
	inParams.inputGamma = 2.2f;
	inParams.outputGamma = 2.2f;
	inParams.wrapMode = nv::FloatImage::WrapMode_Mirror;

	for (int i = 1; i < argc; i++)
	{
		if (strcmp("-t", argv[i]) == 0)
		{
			if (i+1 == argc) break;
			i++;
			
			if (strcmp("diffuse", argv[i]) == 0) inParams.exportType = e_TextureDiffuse;
			else if (strcmp("normal", argv[i]) == 0) inParams.exportType = e_TextureNormal;
		}
		else if (strcmp("-m", argv[i]) == 0)
		{
			inParams.exportMipmaps = true;
		}
		else if (strcmp("-flip", argv[i]) == 0)
		{
			inParams.exportFlipped = true;
		}
		else if (strcmp("-ps", argv[i]) == 0)
		{
			if (i+1 == argc) break;
			i++;
			
			sscanf( argv[i], "%f", &inParams.exportPreScale );
			inParams.exportPreScale = nv::clamp( inParams.exportPreScale, -2.0f, 2.0f );
		}
		else if (strcmp("-pb", argv[i]) == 0)
		{
			if (i+1 == argc) break;
			i++;
			
			sscanf( argv[i], "%f", &inParams.exportPreBias );
			inParams.exportPreBias = nv::clamp( inParams.exportPreBias, -2.0f, 2.0f );
		}
		else if (strcmp("-rs", argv[i]) == 0)
		{
			if (i+1 == argc) break;
			i++;
			
			sscanf( argv[i], "%f", &inParams.exportRedScale );
			inParams.exportRedScale = nv::clamp( inParams.exportRedScale, -2.0f, 2.0f );
		}
		else if (strcmp("-rb", argv[i]) == 0)
		{
			if (i+1 == argc) break;
			i++;
			
			sscanf( argv[i], "%f", &inParams.exportRedBias );
			inParams.exportRedBias = nv::clamp( inParams.exportRedBias, -2.0f, 2.0f );
		}
		else if (strcmp("-gs", argv[i]) == 0)
		{
			if (i+1 == argc) break;
			i++;
			
			sscanf( argv[i], "%f", &inParams.exportGreenScale );
			inParams.exportGreenScale = nv::clamp( inParams.exportGreenScale, -2.0f, 2.0f );
		}
		else if (strcmp("-gb", argv[i]) == 0)
		{
			if (i+1 == argc) break;
			i++;
			
			sscanf( argv[i], "%f", &inParams.exportGreenBias );
			inParams.exportGreenBias = nv::clamp( inParams.exportGreenBias, -2.0f, 2.0f );
		}
		else if (strcmp("-bs", argv[i]) == 0)
		{
			if (i+1 == argc) break;
			i++;
			
			sscanf( argv[i], "%f", &inParams.exportBlueScale );
			inParams.exportBlueScale = nv::clamp( inParams.exportBlueScale, -2.0f, 2.0f );
		}
		else if (strcmp("-bb", argv[i]) == 0)
		{
			if (i+1 == argc) break;
			i++;
			
			sscanf( argv[i], "%f", &inParams.exportBlueBias );
			inParams.exportBlueBias = nv::clamp( inParams.exportBlueBias, -2.0f, 2.0f );
		}
		else if (strcmp("-PS", argv[i]) == 0)
		{
			if (i+1 == argc) break;
			i++;
			
			sscanf( argv[i], "%f", &inParams.exportPostScale );
			inParams.exportPostScale = nv::clamp( inParams.exportPostScale, -2.0f, 2.0f );
		}
		else if (strcmp("-PB", argv[i]) == 0)
		{
			if (i+1 == argc) break;
			i++;
			
			sscanf( argv[i], "%f", &inParams.exportPostBias );
			inParams.exportPostBias = nv::clamp( inParams.exportPostBias, -2.0f, 2.0f );
		}
		else if (strcmp("-f", argv[i]) == 0)
		{
			if (i+1 == argc) break;
			i++;
			
			inParams.exportFormat = getformat(argv[i]);
		}
		else if (strcmp("-k", argv[i]) == 0)
		{
			if (i+1 == argc) break;
			i++;
			
			if (strcmp("box", argv[i]) == 0) inParams.exportFilter = new nv::BoxFilter();
			else if (strcmp("triangle", argv[i]) == 0) inParams.exportFilter = new nv::TriangleFilter();
			else if (strcmp("quadratic", argv[i]) == 0) inParams.exportFilter = new nv::QuadraticFilter();
			else if (strcmp("bspline", argv[i]) == 0) inParams.exportFilter = new nv::BSplineFilter();
			else if (strcmp("mitchell", argv[i]) == 0) inParams.exportFilter = new nv::MitchellFilter();
			else if (strcmp("lanczos", argv[i]) == 0) inParams.exportFilter = new nv::LanczosFilter();
			else if (strcmp("kaiser", argv[i]) == 0) {
				inParams.exportFilter = new nv::KaiserFilter(3);
				((nv::KaiserFilter *)inParams.exportFilter)->setParameters(4.0f, 1.0f);
			}
		}
		else if (strcmp("-w", argv[i]) == 0)
		{
			if (i+1 == argc) break;
			i++;

			if (strcmp("mirror", argv[i]) == 0) inParams.wrapMode = nv::FloatImage::WrapMode_Mirror;
			else if (strcmp("repeat", argv[i]) == 0) inParams.wrapMode = nv::FloatImage::WrapMode_Repeat;
			else if (strcmp("clamp", argv[i]) == 0) inParams.wrapMode = nv::FloatImage::WrapMode_Clamp;
		}
		else if (strcmp("-av", argv[i]) == 0)
		{
			if (i+1 == argc) break;
			i++;
			
			sscanf( argv[i], "%f", &inParams.alphaTestValue );
		}
		else if (strcmp("-ig", argv[i]) == 0)
		{
			if (i+1 == argc) break;
			i++;
			
			sscanf( argv[i], "%f", &inParams.inputGamma );
		}
		else if (strcmp("-og", argv[i]) == 0)
		{
			if (i+1 == argc) break;
			i++;
			
			sscanf( argv[i], "%f", &inParams.outputGamma );
		}
		else if (argv[i][0] != '-')
		{
			inParams.inImage = argv[i];

			if (i+1 < argc && argv[i+1][0] != '-') {
				inParams.outImage = argv[i+1];
			}

			break;
		}
	}

	if (inParams.inImage.isNull() || inParams.outImage.isNull())
	{
		printf("_____________________________________________________________________\n");
		printf("|                  Copyright (c) Adrian SIMINCIUC 2011               |\n");	
		printf("|             Authors: NVIDIA Corporation, Adrian SIMINCIUC          |\n");	
		printf("|____________________________________________________________________|\n\n");
		printf("usage: mipper [options] input output\n\n");
		
		printf("Options:\n");
		printf("  -t type      One of the following: (default = 'diffuse')\n");
		printf("                * diffuse\n");
		printf("                * normal\n");
		printf("  -m           Generate mipmaps\n");
		printf("  -flip        Flip vertically\n");
		printf("  -av value     Alpha coverage value: (default = '1.0')\n");
		printf("                * [ 0.0f : 1.0f ]\n");
		printf("  -ig value    Input gamma: (default = '2.2')\n");
		printf("  -og value    Output gamma: (default = '2.2')\n");
		printf("  -k filter    One of the following: (default = 'box')\n");
		printf("                * box\n");
		printf("                * triangle\n");
		printf("                * quadratic\n");
		printf("                * bspline\n");
		printf("                * mitchell\n");
		printf("                * lanczos\n");
		printf("                * kaiser\n");
		printf("  -f format    One of the following: (default = 'RGBA8')\n");
		printf("                * RGBA8,  RGB8\n"); 
		printf("                * RGBA4, RGB565\n"); 
		printf("                * RGB5A1 \n"); 
		printf("  -w mode      One of the following: (default = 'mirror')\n");
		printf("                * mirror\n");
		printf("                * repeat\n");
		printf("                * clamp\n");
		printf("  -ps value     Pre-scale value: (default = '1.0')\n");
		printf("                * [ -2.0 : +2.0 ]\n");
		printf("  -pb value     Pre-bias value: (default = '0.0')\n");
		printf("                * [ -2.0 : +2.0 ]\n");
		printf("  -rs value     Red channel scale value: (default = '1.0')\n");
		printf("                * [ -2.0 : +2.0 ]\n");
		printf("  -rb value     Red channel bias value: (default = '0.0')\n");
		printf("                * [ -2.0 : +2.0 ]\n");
		printf("  -gs value     Green channel scale value: (default = '1.0')\n");
		printf("                * [ -2.0 : +2.0 ]\n");
		printf("  -gb value     Green channel bias value: (default = '0.0')\n");
		printf("                * [ -2.0 : +2.0 ]\n");
		printf("  -bs value     Blue channel scale value: (default = '1.0')\n");
		printf("                * [ -2.0 : +2.0 ]\n");
		printf("  -bb value     Blue channel bias value: (default = '0.0')\n");
		printf("                * [ -2.0 : +2.0 ]\n");
		printf("  -PS value     Post-scale value: (default = '1.0')\n");
		printf("                * [ -2.0 : +2.0 ]\n");
		printf("  -PB value     Post-bias value: (default = '0.0')\n");
		printf("                * [ -2.0 : +2.0 ]\n");		

		printf("____________________________________________________________________\n");
		printf("|  *****************     IMPORTANT NOTES      **********************|\n");
		printf("|___________________________________________________________________|\n");
		printf("| \"mipper\" detects if the original texture had alpha channel     \n");
		printf("| and if it didn't but the format to export contains alpha channel  \n");
		printf("| it will export to the same memory footprint texture format        \n");
		printf("| without the alpha channel:                                        \n");
		printf("|		-RGBA8 converts to RGB8                                     \n");
		printf("|		-RGBA4 converts to RGB565                                   \n");
		printf("|		-RGB5A1 converts to RGB565                                  \n");
		printf("|___________________________________________________________________\n");

		return 1;
	}	

	if (inParams.exportFilter == NULL) 
		inParams.exportFilter = new nv::BoxFilter();

	printf( "\nmipper:" );			
	printf( "\n  Job: %s", inParams.inImage.str() );			

	if( inParams.exportFormat == -1 )
	{
		inParams.exportFormat = FORMAT_RGBA8;

		printf( "\n  \"WARNING: unknown export format, defaulting to RGBA8\". ");
	}

	nv::Image image;
	if (loadImage(image, inParams.inImage.str()))
	{
		inParams.keepAlpha = ( image.format() == nv::Image::Format_ARGB );

		if( isPowerOfTwo( image.width() ) && isPowerOfTwo( image.height() ) )
		{
			MipImage mipImage(&image);
			if( mipImage.Save( &inParams ) )
			{
				printf( "\n  Done: %s", inParams.outImage.str() );

				return 0;
			}

			printf( "\n  \"ERROR: unknown on converting or saving\". ");

			return 0;
		}
		
		printf( "\n  \"ERROR: size is not power of 2\". ");

		return 0;
	}

	printf( "\n  \"ERROR: can't load image\". " );

	return 0;
}


const char * stristr( const char* str, const char* subStr )
{
	static char buffA[256];
	static char buffB[256];

	int lenA = strlen( str );
	int lenB = strlen( subStr );
	int i;

	if( lenB > lenA )
	{
		return 0;
	}

	strcpy( buffA, str );
	strcpy( buffB, subStr );

	for( i = 0; i < lenA; i++ )
	{
		buffA[i] = tolower(buffA[i]);
	}

	for( i = 0; i < lenB; i++ )
	{
		buffB[i] = tolower(buffB[i]);
	}

	return strstr( buffA, buffB );
}

const int getformat( const char *str )
{
	static const int numFormats = 5;
	static const char *inStr[] = { 
		"RGBA8", "RGB8", "RGBA4", "RGB565", "RGB5A1",
	};
	static const int  outFormat[] = { 
		FORMAT_RGBA8, FORMAT_RGB8, FORMAT_RGBA4, FORMAT_RGB565, FORMAT_RGB5A1,
	};

	for( int k = 0; k < numFormats; k++ )
		if( stristr( str, inStr[k] ) )
			return outFormat[ k ];

	return -1;
}

bool loadImage(nv::Image & image, const char * fileName)
{
	if ( !image.load( fileName ) )
	{
		printf("The file '%s' is not a supported image type.\n", fileName);
		return false;
	}

	return true;
}




















void onQuit()
{
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
}

void onStart()
{
#ifdef _DEBUG
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_crtBreakAlloc = 75;
#endif

	atexit(onQuit);
}

