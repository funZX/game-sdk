// This code is in the public domain -- castanyo@yahoo.es

#pragma once
#ifndef NV_IMAGE_H
#define NV_IMAGE_H

#include <nvcore/nvcore.h>

// Function linkage
#if NVIMAGE_SHARED
#ifdef NVIMAGE_EXPORTS
#define NVIMAGE_API DLL_EXPORT
#define NVIMAGE_CLASS DLL_EXPORT_CLASS
#else
#define NVIMAGE_API DLL_IMPORT
#define NVIMAGE_CLASS DLL_IMPORT
#endif
#else
#define NVIMAGE_API
#define NVIMAGE_CLASS
#endif


// Some utility functions:

inline uint computePitch(uint w, uint bitsize, uint alignment)
{
	return ((w * bitsize +  8 * alignment - 1) / (8 * alignment)) * alignment;
}



#endif // NV_IMAGE_H
