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

#include <agg_basics.h>
#include <agg_rendering_buffer.h>
#include <agg_rasterizer_scanline_aa.h>
#include <agg_scanline_p.h>
#include <agg_renderer_scanline.h>
#include <agg_pixfmt_rgb_packed.h>

#include <render/sim_svg_image.h>
#include <render/sim_driver.h>

#include <render/gui/sim_widget_svg.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CWidgetSvg::CWidgetSvg()
	:CWidget()
{
	m_svgimage	= NULL;
	m_texture	= NULL;
}

// ----------------------------------------------------------------------//

CWidgetSvg::CWidgetSvg(const std::string& name)
	:CWidgetSvg()
{
	m_name = name;
}
// ----------------------------------------------------------------------//

CWidgetSvg::~CWidgetSvg()
{
	SIM_SAFE_DELETE( m_svgimage );
	SIM_SAFE_DELETE( m_texture );
}

// ----------------------------------------------------------------------//

void CWidgetSvg::OnResize()
{
	SIM_ASSERT( m_svgimage != NULL );

	SIM_SAFE_DELETE(m_texture);

	agg::svg::path_renderer& path = m_svgimage->GetPath();

	typedef agg::pixfmt_rgb565 pixfmt;
	typedef agg::renderer_base<pixfmt> renderer_base;
	typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;

	TVec2 center;
	const CRect2D* bounds = m_svgimage->GetBounds();
	bounds->GetCenter( &center );

	u32 w = mat::nextPowerOfTwo( Width() );
	u32 h = mat::nextPowerOfTwo ( Height() );

	u8* renderbuf = SIM_NEW u8[ w * h * 2 ];
	m_renderbuffer.attach( renderbuf, w, h, w * 2 );

	pixfmt pixf( m_renderbuffer );
	renderer_base rb( pixf );
	renderer_solid ren( rb );

	rb.clear( agg::rgba( m_fillcolor.x, m_fillcolor.y, m_fillcolor.z, m_fillcolor.w ) );

	agg::rasterizer_scanline_aa<> ras;
	agg::scanline_p8 sl;
	agg::trans_affine mtx;

	mtx *= agg::trans_affine_scaling( w / bounds->Width(), h / bounds->Height() );
	path.render( ras, sl, ren, mtx, rb.clip_box(), 1.0 );
	agg::render_scanlines( ras, sl, ren );

	m_texture = SIM_NEW CTexture();
	m_texture->Generate(
		renderbuf, 
		w,
		h,
		CTexture::k_Type_RGB,
		CTexture::k_Wrap_Clamp,
		CTexture::k_Filter_Nearest, 
		CTexture::k_Format_RGB565
	);

	SIM_SAFE_DELETE(renderbuf);
}

// ----------------------------------------------------------------------//

void CWidgetSvg::Render( CDriver *driver )
{
	if ( m_texture == NULL )
		return;

	CWidget::Render(driver);
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
