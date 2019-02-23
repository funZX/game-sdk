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

#include <render/sim_driver.h>
#include <render/sim_texture.h>
#include <render/sim_material.h>

namespace sim
{
	namespace rnr
{
// ----------------------------------------------------------------------//

CMaterial::CMaterial()
{
	Vec4ToWhite( &m_ambient );
	Vec4ToWhite( &m_diffuse );
	Vec4ToWhite( &m_specular );
	Vec4ToWhite( &m_emissive );
	Vec4ToBlack( &m_reflective );

	m_shininess	 = 0.0f;
	m_refraction = 0.0f;

	SIM_MEMSET( m_textures, 0, sizeof( m_textures ) );
}
// ----------------------------------------------------------------------//

CMaterial::CMaterial( const std::string &name )
	: CMaterial()
{
	m_name = name;
}
// ----------------------------------------------------------------------//

CMaterial::~CMaterial()
{

}

// ----------------------------------------------------------------------//

void CMaterial::Update( f32 dt, void *userData )
{

}

// ----------------------------------------------------------------------//

void CMaterial::Render( CDriver *driver )
{
	driver->SetMaterialAmbient( &m_ambient );
	driver->SetMaterialDiffuse( &m_diffuse );
	driver->SetMaterialSpecular( &m_specular );
	driver->SetMaterialEmissive( &m_emissive );
	driver->SetMaterialReflective( &m_reflective );

	driver->SetMaterialShininess( m_shininess );
	driver->SetMaterialRefraction( m_refraction );

	for( u32 i = 0; i < CDriver::k_Texture_Channels_Count; i++ )
	{
		if ( m_textures[ i ] != nullptr )
		{
			driver->SetTextureChannel( static_cast<CDriver::TextureChannel>(i) );
			driver->BindTexture( m_textures[ i ]->GetID() );
		}
	}

	SIM_CHECK_OPENGL();
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
