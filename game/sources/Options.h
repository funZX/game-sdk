#ifndef __OPTIONS_H
#define __OPTIONS_H

#include <sim_engine.h>
#include <render/sim_texture.h>

namespace Options
{

namespace Video
{
	extern CTexture::Filter		TextureFilter;

}; // namespace Video;

namespace Camera
{
	extern f32					ZNear;
	extern f32					ZFar;
};

namespace World
{
	extern f32					MaxSize;
};

}; // namespace Options

#endif // __OPTIONS_H

