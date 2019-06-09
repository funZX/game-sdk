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

#ifndef __SIM_RECT_2D_H
#define __SIM_RECT_2D_H

#include <core/sim_core.h>
#include <core/sim_interfaces.h>

#include <render/sim_render.h>

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
	enum class Align : u32
	{
		InLeft,
		InRight,
		InTop,
		InBottom,

		OutLeft,
		OutRight,
		OutTop,
		OutBottom,

		HorCenter,
		VerCenter,
		Center
	};
	// ------------------------------------------------------------------//

	f32						    Left( void ) const;
	f32						    Right( void ) const;
	f32						    Top( void ) const;
	f32						    Bottom( void ) const;
	f32						    Width( void ) const;
	f32						    Height( void ) const;

	void						Move( Vec2 *d );
	void						Move( f32 x, f32 y );
	void						MoveTo( Vec2 *pos );
	void						MoveTo( f32 x, f32 y );

	void						Inflate( f32 dw, f32 dh );
	void						Zoom( f32 dw, f32 dh );
	void						Resize( f32 w, f32 h );
	void						Scale( f32 kw, f32 kh );
	void						Rotate( f32 rotation);

	void						SetCenter( Vec2 *pos );
	void						GetCenter( Vec2 *pos ) const;

	void						AlignTo( CRect2D *, Align align );

	bool						IsInside( Vec2 *pos );
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
	Vec2						m_position;
	Vec2						m_size;
	f32							m_rotation;

	CMaterial*					m_material;
	CRect2D*					m_texRect;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_RECT_2D_H
