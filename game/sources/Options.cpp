#include "Options.h"

namespace Options
{

namespace Video
{
	CTexture::K_FILTER	TextureFilter = CTexture::k_Filter_Bilinear;
}; // namespace Video;

namespace Camera
{
	f32 ZNear	= 0.1f;
	f32 ZFar	= 1000.0f;
}; // namespace Camera;


}; // namespace Options