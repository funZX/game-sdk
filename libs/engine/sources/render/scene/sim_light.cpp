/* 
*    SIM - OpenGL ES 2.0 portable game engine
*    Copyright (C) 2012  Adrian SIMINCIUC
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <render/scene/sim_light.h>
#include <render/sim_driver.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CLight::CLight( const std::string &name )
	: CSceneNode( name )
{
	m_channel	= CDriver::k_Select_Light_0;

	Vec4ToOne( &m_ambient );
	Vec4ToOne( &m_diffuse );
	Vec4ToOne( &m_specular );

	m_intensity	= 0.25f;

	Vec3ToZero( &m_position );
	Vec3ToRDirZ( &m_direction );

	m_isVisible			= true;
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
	driver->SelectLight( m_channel );

	driver->SetLightIntensity( m_intensity );

	driver->SetLightPosition( &m_position );
	driver->SetLightDirection( &m_direction );

	driver->SetLightAmbient( &m_ambient );
	driver->SetLightDiffuse( &m_diffuse );
	driver->SetLightSpecular( &m_specular );
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
