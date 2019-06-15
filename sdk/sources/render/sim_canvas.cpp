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
#include <render/sim_batch_2d.h>
#include <render/sim_canvas.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

CCanvas::CCanvas( CFontAtlas* fontAtlas )
	: CWidget( fontAtlas )
{

}

// ----------------------------------------------------------------------//

CCanvas::CCanvas( const std::string& name, CFontAtlas* fontAtlas)
	: CCanvas( fontAtlas )
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
	std::cout << "PointerDown: " << x << ", " << y << std::endl;
}

// ----------------------------------------------------------------------//

void CCanvas::PointerDrag( u32 x, u32 y )
{
	std::cout << "PointerDrag: " << x << ", " << y << std::endl;
}

// ----------------------------------------------------------------------//

void CCanvas::PointerUp( u32 x, u32 y )
{
	std::cout << "PointerUp: " << x << ", " << y << std::endl;
}

// ----------------------------------------------------------------------//

void CCanvas::ClearEvents() 
{

}

// ----------------------------------------------------------------------//

void CCanvas::Update(f32 dt, void* userData)
{
    CWidget::Update( dt, userData );
}

// ----------------------------------------------------------------------//

void CCanvas::Render( CDriver* driver )
{
	driver->SetViewport( (u32) m_size.x, (u32) m_size.y );

    CWidget::Render( driver );
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
