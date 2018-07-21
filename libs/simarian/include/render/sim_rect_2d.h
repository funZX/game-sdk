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

class CRect2D
{
public:
	// ------------------------------------------------------------------//
	CRect2D( const std::string& name );
	// ------------------------------------------------------------------//
	typedef enum
	{
		k_Align_Inside				= ( 1 << 0 ),
		k_Align_OutSide				= ( 1 << 1 ),
		k_Align_Left				= ( 1 << 2 ),
		k_Align_Right				= ( 1 << 3 ),
		k_Align_Top					= ( 1 << 4 ),
		k_Align_Bottom				= ( 1 << 5 ),
		k_Align_HorCenter			= ( 1 << 6 ),
		k_Align_VerCenter			= ( 1 << 7 ),
		k_Align_Center				= ( k_Align_HorCenter	| k_Align_VerCenter ),
		k_Align_Default				= ( k_Align_Inside	| k_Align_Left		| k_Align_Top ),

	} K_ALIGN;
	// ------------------------------------------------------------------//
	typedef enum
	{
		k_Transform_Rot90			= ( 1 << 0 ),
		k_Transform_Rot180			= ( 1 << 1 ),
		k_Transform_Rot270			= ( 1 << 2 ),

		k_Transform_FlipHor			= ( 1 << 3 ),
		k_Transform_FlipVer			= ( 1 << 4 ),
		k_Transform_Flip			= ( k_Transform_FlipHor | k_Transform_FlipVer ),

		k_Transform_Default			= (    0    ),

	} K_TRANSFORM;
	// ------------------------------------------------------------------//

	f32						    Left( void );
	f32						    Right( void );
	f32						    Top( void );
	f32						    Bottom( void );
	f32						    Width( void );
	f32						    Height( void );

	void						Move( TVec2 *d );
	void						Move( f32 x, f32 y );
	void						MoveTo( TVec2 *pos );
	void						MoveTo( f32 x, f32 y );

	void						Inflate( f32 dw, f32 dh );
	void						Zoom( f32 dw, f32 dh );
	void						Resize( f32 w, f32 h );
	void						Scale( f32 kw, f32 kh );

	void						SetCenter( TVec2 *pos );
	void						GetCenter( TVec2 *pos );

	void						Align( CRect2D *, u32 align );
	void						Transform( u32 trans );

	bool						IsInside( TVec2 *pos );
	bool						IsInside( f32 x, f32 y );

	void						Bound( f32, f32, f32, f32);
	void						Bound( CRect2D * );

	static void					Union( CRect2D *, CRect2D *, CRect2D * );
	static void					Intersect( CRect2D *, CRect2D *, CRect2D* );

	void						SetMaterial( CMaterial *material ) { m_material = material; }
	CMaterial*					GetMaterial() { return m_material; }

	virtual void				Render( CDriver *driver, const f32 *texCoords, TMatrix4 *transform );
	// ------------------------------------------------------------------//

protected:
	// ------------------------------------------------------------------//
	std::string					m_name;

	TVec2						m_position;
	TVec2						m_size;

	CMaterial*					m_material;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_RECT_2D_H
