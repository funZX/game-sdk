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

#ifndef __SIM_SVG_IMAGE_H
#define __SIM_SVG_IMAGE_H

#include <svg/agg_svg_path_renderer.h>

#include <core/sim_interfaces.h>
#include <render/sim_render.h>
#include <render/sim_rect_2d.h>

namespace sim
{
namespace io { class CMemStream; }
namespace rnr
{
// ----------------------------------------------------------------------//

class CSvgImage: public IEngineItem
{
	friend class CWidgetSvg;
public:
	// ------------------------------------------------------------------//
	CSvgImage();
	CSvgImage( const std::string &name );
	virtual ~CSvgImage();
	// ------------------------------------------------------------------//
	agg::svg::path_renderer&			GetPath() { return m_path; }
	void								Load( io::CMemStream* ms );
	const CRect2D*						GetBounds() { return &m_bounds;  }
	// ------------------------------------------------------------------//

protected:
	// ------------------------------------------------------------------//
	agg::svg::path_renderer				m_path;
	CRect2D								m_bounds;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_SVG_IMAGE_H
