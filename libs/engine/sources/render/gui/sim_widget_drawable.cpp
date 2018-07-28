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

	m_rendertexture = NULL;
	m_camera = new CCamera();
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
}

// ----------------------------------------------------------------------//

void CWidgetDrawable::OnResize()
{
	SIM_SAFE_DELETE( m_rendertexture );

	m_rendertexture = SIM_NEW CRenderTexture();
	m_rendertexture->Generate( (u32)m_size.x, (u32)m_size.y );

	CRect2D r;
	r.Resize( (u32)m_rendertexture->GetWidth(), (u32)m_rendertexture->GetHeight() );
	m_camera->SetPerspective( &r );
}

// ----------------------------------------------------------------------//

void CWidgetDrawable::Draw( CDriver *driver )
{
	static CEngine *engine = CEngine::GetSingletonPtr();

	if ( m_rendertexture == NULL )
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
	CRect2D::Render( driver, CRect2D::OneSizeRectFlip );
	CWidget::Render( driver );
}
// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
