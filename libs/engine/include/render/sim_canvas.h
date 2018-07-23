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

#ifndef __SIM_CANVAS_H
#define __SIM_CANVAS_H

#include <core/sim_singleton.h>
#include <core/sim_interfaces.h>

#include <render/sim_render.h>
#include <render/sim_widget.h>

#include <math/sim_vec2.h>
#include <math/sim_vec3.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

class CCamera;

class CCanvas : public CWidget
{
public:
	CCanvas( const std::string& name );
	virtual ~CCanvas();
	// ------------------------------------------------------------------//
	void				Resize( f32 width, f32 height );
	virtual void		Update( f32 dt, void *userData );
	virtual void		Render( CDriver* driver );

	virtual void		PointerDown( u32 x, u32 y );
	virtual void		PointerDrag( u32 x, u32 y );
	virtual void		PointerUp( u32 x, u32 y );

	virtual void		ClearEvents();
	// ------------------------------------------------------------------//
	const std::string&	GetName()		{ return m_name; }
	// ------------------------------------------------------------------//
protected:

	// ------------------------------------------------------------------//
	std::string			m_name;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_CANVAS_H

