// Copyright NVIDIA Corporation 2007 -- Ignacio Castano <icastano@nvidia.com>
// 
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

#include <nvimage/NormalMap.h>
#include <nvimage/Filter.h>
#include <nvimage/FloatImage.h>
#include <nvimage/Image.h>

#include <nvmath/Color.h>

#include <nvcore/Ptr.h>


using namespace nv;

// Create normal map using the given kernels.
static FloatImage * createNormalMap(const Image * img, FloatImage::WrapMode wm, Vector4::Arg heightWeights, const Kernel2 * kdu, const Kernel2 * kdv)
{
	nvDebugCheck(kdu != NULL);
	nvDebugCheck(kdv != NULL);
	nvDebugCheck(img != NULL);
	
	const uint w = img->width();
	const uint h = img->height();
	
	AutoPtr<FloatImage> fimage(new FloatImage());
	fimage->allocate(4, w, h);
	
	// Compute height and store in alpha channel:
	float * alphaChannel = fimage->channel(3);
	for(uint i = 0; i < w*h; i++)
	{
		Vector4 color = toVector4(img->pixel(i));
		alphaChannel[i] = dot(color, heightWeights);
	}
	
	float heightScale = 1.0f / 16.0f;	// @@ Use a user defined factor.
	
	for(uint y = 0; y < h; y++)
	{
		for(uint x = 0; x < w; x++)
		{
			const float du = fimage->applyKernel(kdu, x, y, 3, wm);
			const float dv = fimage->applyKernel(kdv, x, y, 3, wm);
			
			Vector3 n = normalize(Vector3(du, dv, heightScale));
			
			fimage->pixel(x, y, 0) = 0.5f * n.x + 0.5f;
			fimage->pixel(x, y, 1) = 0.5f * n.y + 0.5f;
			fimage->pixel(x, y, 2) = 0.5f * n.z + 0.5f;
		}
	}
	
	return fimage.release();
}


// Create normal map using the given kernels.
static FloatImage * createNormalMap(const FloatImage * img, FloatImage::WrapMode wm, const Kernel2 * kdu, const Kernel2 * kdv)
{
	nvDebugCheck(kdu != NULL);
	nvDebugCheck(kdv != NULL);
	nvDebugCheck(img != NULL);

#pragma NV_MESSAGE("FIXME: Height scale parameter should go away. It should be a sensible value that produces good results when the heightmap is in the [0, 1] range.")
	const float heightScale = 1.0f / 16.0f;

	const uint w = img->width();
	const uint h = img->height();

	AutoPtr<FloatImage> img_out(new FloatImage());
	img_out->allocate(4, w, h);

	for (uint y = 0; y < h; y++)
	{
		for (uint x = 0; x < w; x++)
		{
			const float du = img->applyKernel(kdu, x, y, 3, wm);
			const float dv = img->applyKernel(kdv, x, y, 3, wm);

			Vector3 n = normalize(Vector3(du, dv, heightScale));

			img_out->pixel(x, y, 0) = n.x;
			img_out->pixel(x, y, 1) = n.y;
			img_out->pixel(x, y, 2) = n.z;
		}
	}

	// Copy alpha channel.
	for (uint y = 0; y < h; y++)
	{
		for (uint x = 0; x < w; x++)
		{
			img_out->pixel(x, y, 3) = img->pixel(x, y, 3);
		}
	}

	return img_out.release();
}


/// Create normal map using the given filter.
FloatImage * nv::createNormalMap(const Image * img, FloatImage::WrapMode wm, Vector4::Arg heightWeights, NormalMapFilter filter /*= Sobel3x3*/)
{
	nvDebugCheck(img != NULL);
	
	// Init the kernels.
	Kernel2 * kdu = NULL;
	Kernel2 * kdv = NULL;

	switch(filter)
	{
		case NormalMapFilter_Sobel3x3:
			kdu = new Kernel2(3);
			break;
		case NormalMapFilter_Sobel5x5:
			kdu = new Kernel2(5);
			break;
		case NormalMapFilter_Sobel7x7:
			kdu = new Kernel2(7);
			break;
		case NormalMapFilter_Sobel9x9:
			kdu = new Kernel2(9);
			break;
		default:
			nvDebugCheck(false);
	};

	kdu->initSobel();
	kdu->normalize();

	kdv = new Kernel2(*kdu);
	kdv->transpose();

	return ::createNormalMap(img, wm, heightWeights, kdu, kdv);
}


/// Create normal map combining multiple sobel filters.
FloatImage * nv::createNormalMap(const Image * img, FloatImage::WrapMode wm, Vector4::Arg heightWeights, Vector4::Arg filterWeights)
{
	nvDebugCheck(img != NULL);

	Kernel2 * kdu = NULL;
	Kernel2 * kdv = NULL;

	kdu = new Kernel2(9);
	kdu->initBlendedSobel(filterWeights);
	kdu->normalize();
	
	kdv = new Kernel2(*kdu);
	kdv->transpose();
	
	return ::createNormalMap(img, wm, heightWeights, kdu, kdv);
}


FloatImage * nv::createNormalMap(const FloatImage * img, FloatImage::WrapMode wm, Vector4::Arg filterWeights)
{
	nvDebugCheck(img != NULL);

	Kernel2 * kdu = NULL;
	Kernel2 * kdv = NULL;

	kdu = new Kernel2(9);
	kdu->initBlendedSobel(filterWeights);
	kdu->normalize();

	kdv = new Kernel2(*kdu);
	kdv->transpose();

	return ::createNormalMap(img, wm, kdu, kdv);
}


/// Normalize the given image in place.
void nv::normalizeNormalMap(FloatImage * img)
{
	nvDebugCheck(img != NULL);

#pragma NV_MESSAGE("TODO: Pack and expand normals explicitly?")

	img->expandNormals(0);
	img->normalize(0);
	img->packNormals(0);
}

