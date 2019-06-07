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

#include <render/scene/sim_light.h>
#include <render/sim_driver.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CLight::CLight()
	: CSceneNode()
{
	m_channel	= CDriver::LightChannel::Light_0;

	Vec4ToOne( &m_ambient );
	Vec4ToOne( &m_diffuse );
	Vec4ToOne( &m_specular );

	m_intensity	= 0.25f;
}

// ----------------------------------------------------------------------//

CLight::CLight( const std::string &name )
	: CLight()
{
	m_name = name;
}

// ----------------------------------------------------------------------//

CLight::~CLight()
{

}

// ----------------------------------------------------------------------//

void CLight::Update( f32 dt, void *userData )
{

}

// ----------------------------------------------------------------------//

void CLight::Render( CDriver *driver )
{
    driver->SetLightPosition(&m_transform.translation);
    driver->SetLightDirection(&m_transform.axis.direction);
    
    driver->SetLightChannel(m_channel);
	driver->SetLightIntensity( m_intensity );
	driver->SetLightAmbient( &m_ambient );
	driver->SetLightDiffuse( &m_diffuse );
	driver->SetLightSpecular( &m_specular );
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
