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

#ifndef __SIM_WIDGET_RENDERTEXTURE_H
#define __SIM_WIDGET_RENDERTEXTURE_H

#include <core/sim_core.h>

#include <render/sim_render.h>
#include <render/gui/sim_widget.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//
class CCamera;
class CRenderTexture;
class CDriver;
// ----------------------------------------------------------------------//

class CWidgetDrawable: public CWidget
{
public:
	// ------------------------------------------------------------------//
	CWidgetDrawable();
	CWidgetDrawable( const std::string& name );
	virtual ~CWidgetDrawable();
	// ------------------------------------------------------------------//
	void							Draw( CDriver *driver );	
	CRenderTexture*					GetTexture() { return m_rendertexture; }
	// ------------------------------------------------------------------//
	sigcxx::Signal<CDriver*>		OnDraw;
	// ------------------------------------------------------------------//

protected:
	// ------------------------------------------------------------------//
	virtual void					OnResize();
	// ------------------------------------------------------------------//
	CRenderTexture*					m_rendertexture;
	CCamera*						m_camera;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_WIDGET_RENDERTEXTURE_H
