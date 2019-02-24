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
#include <render/sim_render_texture.h>
#include <render/scene/sim_camera.h>
#include <render/sim_driver.h>

#include <render/gui/sim_widget_drawable.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CWidgetDrawable::CWidgetDrawable()
	:CWidget()
{
	Vec4Copy( &m_fillcolor, &col::Black );

	m_rendertexture = nullptr;
	m_camera		= new CCamera();

	// flip
	m_texRect		= SIM_NEW CRect2D();
	m_texRect->MoveTo(  1.0f,  1.0f );
	m_texRect->Resize( -1.0f, -1.0f );
}

// ----------------------------------------------------------------------//

CWidgetDrawable::CWidgetDrawable( const std::string& name )
	:CWidgetDrawable()
{
	m_name = name;
}
// ----------------------------------------------------------------------//

CWidgetDrawable::~CWidgetDrawable()
{
	SIM_SAFE_DELETE( m_camera );
	SIM_SAFE_DELETE( m_rendertexture );
	SIM_SAFE_DELETE( m_texRect );
}

// ----------------------------------------------------------------------//

void CWidgetDrawable::OnResize()
{
	if (m_rendertexture)
		return;

	m_rendertexture = SIM_NEW CRenderTexture();
	m_rendertexture->Generate( (u32)m_size.x, (u32)m_size.y );

	CRect2D r;
	r.Resize( (f32)m_rendertexture->GetWidth(), (f32)m_rendertexture->GetHeight() );
	m_camera->SetPerspective( &r );
}

// ----------------------------------------------------------------------//

void CWidgetDrawable::Draw( CDriver *driver )
{
	static CEngine *engine = CEngine::GetSingletonPtr();

	if ( m_rendertexture == nullptr )
		return;

	CRenderTexture* fb =
	driver->BindRenderTexture(m_rendertexture);
	driver->ClearColor( &m_fillcolor);
	engine->SetCamera( m_camera );

	OnDraw.Emit( driver );
	
	engine->SetCamera( 0 );
	driver->BindRenderTexture( 0 );
}

// ----------------------------------------------------------------------//
void CWidgetDrawable::Render( CDriver *driver )
{
	CWidget::Render( driver );
}
// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
