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

#include <sim_engine.h>
#include <render/sim_material.h>
#include <render/sim_effect.h>
#include <render/sim_render_texture.h>
#include <render/scene/sim_camera.h>
#include <render/sim_driver.h>

#include <render/sim_drawable.h>

#include <imgui.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CDrawable::CDrawable()
	: CRect2D()
{
	m_color         = col::Black;

	m_rendertexture = nullptr;
	m_camera		= new CCamera();

    m_material      = SIM_NEW CMaterial();

	m_isVisible		= true;
}

// ----------------------------------------------------------------------//

CDrawable::CDrawable( const std::string& name )
	:CDrawable()
{
	m_name = name;
}
// ----------------------------------------------------------------------//

CDrawable::~CDrawable()
{
	SIM_SAFE_DELETE( m_camera );
	SIM_SAFE_DELETE( m_rendertexture );
	SIM_SAFE_DELETE( m_material );
}

// ----------------------------------------------------------------------//

void CDrawable::OnResize()
{
	SIM_ASSERT( m_rendertexture == nullptr );

    CDriver* driver = CDriver::GetSingletonPtr();

    u32 tex = driver->BindTexture( CDriver::TextureTarget::Texture2D, 0 );
    m_rendertexture = SIM_NEW CRenderTexture();
	m_rendertexture->Generate( (u32)m_size.x, (u32)m_size.y );
    driver->BindTexture( CDriver::TextureTarget::Texture2D, tex );
}
// ----------------------------------------------------------------------//

void CDrawable::SetMaterial( CMaterial* material )
{
    SIM_ASSERT( material != nullptr );
    SIM_ASSERT( material->GetEffect() != nullptr );

	SIM_MEMCPY( m_material, material, sizeof( CMaterial ) );
	m_material->SetTexture( m_rendertexture, 0 );
}

// ----------------------------------------------------------------------//

void CDrawable::Draw( CDriver *driver )
{
	SIM_ASSERT( m_rendertexture != nullptr );

	if ( !m_isVisible )
		return;

	static CEngine *engine = CEngine::GetSingletonPtr();

	driver->BindRenderTexture( m_rendertexture );
	driver->ClearColor( m_color );

	driver->SetMatrixMode( CDriver::MatrixMode::Projection );
	driver->MatrixPush( false );
	driver->MatrixLoad( m_camera->GetPerspectiveMatrix() );

	driver->SetMatrixMode( CDriver::MatrixMode::View );
	driver->MatrixPush( false );
	driver->MatrixLoad( m_camera->GetMatrix() );

	driver->SetMatrixMode( CDriver::MatrixMode::World );
	driver->MatrixPush( false );
	driver->MatrixLoadIdentity();

	m_camera->Render( driver );
	OnDraw.Emit( driver );
	
    driver->SetMatrixMode(CDriver::MatrixMode::Projection);
    driver->MatrixPop();

    driver->SetMatrixMode(CDriver::MatrixMode::View);
    driver->MatrixPop();

    driver->SetMatrixMode(CDriver::MatrixMode::World);
    driver->MatrixPop();

	driver->BindRenderTexture(0);
}
// ----------------------------------------------------------------------//
void CDrawable::Update(f32 dt, void* userData)
{
	m_camera->Update( dt, userData );

    CRect2D r;
    r.Resize((f32)m_rendertexture->GetWidth(), (f32)m_rendertexture->GetHeight());
    m_camera->SetPerspective( &r );
}
// ----------------------------------------------------------------------//
void CDrawable::Render( CDriver *driver )
{
	SIM_ASSERT( m_material != nullptr );
	SIM_ASSERT( m_material->GetEffect() != nullptr );

	m_isVisible = !ImGui::IsWindowCollapsed() || ImGui::IsWindowAppearing();

    ImVec2 vMin = ImGui::GetWindowContentRegionMin();
    ImVec2 vMax = ImGui::GetWindowContentRegionMax();

    vMin.x += ImGui::GetWindowPos().x;
    vMin.y += ImGui::GetWindowPos().y;
    vMax.x += ImGui::GetWindowPos().x;
    vMax.y += ImGui::GetWindowPos().y;

	ImGui::Image((void*)m_material, ImVec2(vMax.x - vMin.x, vMax.y - vMin.y), ImVec2(0, 1), ImVec2(1, 0));
}
// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
