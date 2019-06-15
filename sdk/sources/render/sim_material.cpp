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
	m_ambient    = col::White;
	m_diffuse    = col::White;
	m_specular   = col::White;
	m_emissive   = col::White;
	m_reflective = col::Black;

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
	driver->SetMaterialAmbient( m_ambient );
	driver->SetMaterialDiffuse( m_diffuse );
	driver->SetMaterialSpecular( m_specular );
	driver->SetMaterialEmissive( m_emissive );
	driver->SetMaterialReflective( m_reflective );

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
bool CMaterial::Load(io::CMemStream* ms)
{
    return false;
}
// ----------------------------------------------------------------------//
bool CMaterial::Save(io::CMemStream* ms)
{
    return false;
}
// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
