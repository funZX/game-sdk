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
#include <render/sim_batch_2d.h>
#include <render/sim_canvas.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CCanvas::CCanvas() 
	: CWidget()
{
}

// ----------------------------------------------------------------------//

CCanvas::CCanvas( const std::string& name )
	: CCanvas()
{
	m_name = name;
}
// ----------------------------------------------------------------------//

CCanvas::~CCanvas()
{
}

// ----------------------------------------------------------------------//

void CCanvas::Resize( f32 w, f32 h )
{
	Bound( 0, 0, w, h );
}

// ----------------------------------------------------------------------//

void CCanvas::PointerDown( u32 x, u32 y )
{

}

// ----------------------------------------------------------------------//

void CCanvas::PointerDrag( u32 x, u32 y )
{

}

// ----------------------------------------------------------------------//

void CCanvas::PointerUp( u32 x, u32 y )
{

}

// ----------------------------------------------------------------------//

void CCanvas::ClearEvents() 
{

}

// ----------------------------------------------------------------------//

void CCanvas::Render( CDriver* driver )
{
	driver->SetViewport( (u32) m_size.x, (u32) m_size.y );
}

// ----------------------------------------------------------------------//

void CCanvas::Update( f32 dt, void *userData )
{

}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
