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

#ifndef __SIM_RECT_2D_H
#define __SIM_RECT_2D_H

#include <core/sim_interfaces.h>
#include <math/sim_vec2.h>
#include <math/sim_matrix4.h>
#include <render/sim_render.h>

using namespace sim::mat;

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

class CEffect;
class CMaterial;
class CDriver;

class CRect2D : public IRenderable, public IUpdatable, public IEngineItem
{
public:
	// ------------------------------------------------------------------//
	CRect2D();
	CRect2D( const std::string& name );
	CRect2D(f32 x, f32 y, f32 width, f32 height);
	// ------------------------------------------------------------------//
	typedef enum
	{
		k_Align_LeftIn,
		k_Align_RightIn,
		k_Align_TopIn,
		k_Align_BottomIn,

		k_Align_LeftOut,
		k_Align_RightOut,
		k_Align_TopOut,
		k_Align_BottomOut,

		k_Align_HorCenter,
		k_Align_VerCenter,
		k_Align_Center

	} K_ALIGN;
	// ------------------------------------------------------------------//

	f32						    Left( void ) const;
	f32						    Right( void ) const;
	f32						    Top( void ) const;
	f32						    Bottom( void ) const;
	f32						    Width( void ) const;
	f32						    Height( void ) const;

	void						Move( TVec2 *d );
	void						Move( f32 x, f32 y );
	void						MoveTo( TVec2 *pos );
	void						MoveTo( f32 x, f32 y );

	void						Inflate( f32 dw, f32 dh );
	void						Zoom( f32 dw, f32 dh );
	void						Resize( f32 w, f32 h );
	void						Scale( f32 kw, f32 kh );
	void						Rotate( f32 rotation);

	void						SetCenter( TVec2 *pos );
	void						GetCenter( TVec2 *pos ) const;

	void						Align( CRect2D *, K_ALIGN align );

	bool						IsInside( TVec2 *pos );
	bool						IsInside( f32 x, f32 y );

	void						Bound( f32, f32, f32, f32);
	void						Bound( CRect2D * );

	static void					Union( CRect2D *, CRect2D *, CRect2D * );
	static void					Intersect( CRect2D *, CRect2D *, CRect2D* );

	virtual void				SetMaterial( CMaterial *material ) { m_material = material; }
	CMaterial*					GetMaterial() { return m_material; }

	virtual void				Render( CDriver *driver );
	void						Render( CDriver *driver, const CRect2D *texRect );

	virtual void				Update(f32 dt, void *userData);
	// ------------------------------------------------------------------//

	static const CRect2D*		OneSizeRect;
	static const CRect2D*		OneSizeRectFlip;
	// ------------------------------------------------------------------//
protected:
	// ------------------------------------------------------------------//
	virtual void				OnMove();
	virtual void				OnResize();
	virtual void				OnRotate();
	// ------------------------------------------------------------------//
	TVec2						m_position;
	TVec2						m_size;
	f32							m_rotation;

	CMaterial*					m_material;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_RECT_2D_H
