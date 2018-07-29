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
#include <svg/agg_svg_parser.h>

#include <core/io/sim_mem_stream.h>

#include <render/sim_svg_image.h>
#include <render/sim_driver.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CSvgImage::CSvgImage() : 
	m_path()
{

}

// ----------------------------------------------------------------------//

CSvgImage::CSvgImage( const std::string &name )
	: CSvgImage()
{
	m_name = name;
}

// ----------------------------------------------------------------------//

CSvgImage::~CSvgImage()
{

}

// ----------------------------------------------------------------------//

void CSvgImage::Load( io::CMemStream* ms )
{
	agg::svg::parser p( m_path );
	
	p.parse( ( const char* )ms->Read( 0 ), ms->GetSize() );
	m_path.arrange_orientations();

	double minx = 0.0, miny = 0.0, maxx = 0.0, maxy = 0.0;
	m_path.bounding_rect( &minx, &miny, &maxx, &maxy );	
	m_bounds.Bound( minx, miny, maxx - minx, maxy - miny );
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
