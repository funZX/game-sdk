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

#ifndef __SIM_SPRITE_H
#define __SIM_SPRITE_H

#include <core/sim_interfaces.h>

#include <render/sim_render.h>
#include <render/sim_material.h>
#include <render/sim_texture.h>
#include <render/sim_rect_2d.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

class CWidget;
class CDriver;

class CSprite : public IEngineItem
{
public:
	// ------------------------------------------------------------------//
	CSprite();
	CSprite( const std::string& name );
	// ------------------------------------------------------------------//
	void						SetMaterial( CMaterial *mtl ) { m_material = mtl; }
	CMaterial*					GetMaterial() { return m_material; }

	s32							GetWidth() { return m_material->GetTexture( 0 )->GetWidth(); }
	s32							GetHeight() { return m_material->GetTexture( 0 )->GetHeight(); }

	void						AddModule( s32 modId, s32 x, s32 y, s32 w, s32 h );
	void						RenderModule( CDriver *driver, CWidget *widget, s32 modId );

	void						Load( const std::string &file );
	// ------------------------------------------------------------------//

protected:

	// ------------------------------------------------------------------//
	CMaterial*					m_material;
	std::map<s32, CRect2D>		m_modules;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_SPRITE_H
