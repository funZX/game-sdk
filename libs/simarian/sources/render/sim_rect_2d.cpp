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

#include <render/sim_batch_2d.h>
#include <render/sim_rect_2d.h>
#include <render/sim_effect.h>
#include <render/sim_material.h>
#include <render/sim_vertex_source.h>
#include <render/sim_vertex_group.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//
CRect2D::CRect2D()
{
	Vec2ToZero(&m_position);
	Vec2ToZero(&m_size);

	m_material = NULL;
}
// ----------------------------------------------------------------------//

CRect2D::CRect2D( const std::string& name )
	: CRect2D()
{
	m_name		= name;
}
// ----------------------------------------------------------------------//

CRect2D::CRect2D(f32 x, f32 y, f32 width, f32 height)
{
	m_position.x = x;
	m_position.y = y;

	m_size.x = width;
	m_size.y = height;
}
// ----------------------------------------------------------------------//

f32	CRect2D::Left( void )
{
	return m_position.x;
}

// ----------------------------------------------------------------------//

f32 CRect2D::Right( void )
{
	return ( m_position.x + m_size.x );
}

// ----------------------------------------------------------------------//

f32 CRect2D::Top( void )
{
	return m_position.y;
}

// ----------------------------------------------------------------------//

f32 CRect2D::Bottom( void )
{
	return ( m_position.y + m_size.y );
}

// ----------------------------------------------------------------------//

f32 CRect2D::Width( void )
{
	return m_size.x;
}

// ----------------------------------------------------------------------//

f32 CRect2D::Height( void )
{
	return m_size.y;
}

// ----------------------------------------------------------------------//

void CRect2D::Bound( f32 x, f32 y, f32 width, f32 height )
{
	m_position.x	= x;
	m_position.y	= y;
	m_size.x		= width;
	m_size.y		= height;
}

// ----------------------------------------------------------------------//

void CRect2D::Bound( CRect2D *r )
{
	m_position.x		= r->m_position.x;
	m_position.y		= r->m_position.y;
	m_size.x			= r->m_size.x;
	m_size.y			= r->m_size.y;
}

// ----------------------------------------------------------------------//

void CRect2D::Move( TVec2 *d )
{
	m_position.x += d->x;
	m_position.y += d->y;
}

// ----------------------------------------------------------------------//

void CRect2D::Move( f32 dx, f32 dy )
{
	m_position.x += dx;
	m_position.y += dy;
}

// ----------------------------------------------------------------------//

void CRect2D::MoveTo( f32 x, f32 y )
{
	m_position.x = x;
	m_position.y = y;
}

// ----------------------------------------------------------------------//

void CRect2D::MoveTo( TVec2 *pos )
{
	m_position.x = pos->x;
	m_position.y = pos->y;
}

// ----------------------------------------------------------------------//

void CRect2D::Inflate( f32 dw, f32 dh )
{
	m_size.x += dw;
	m_size.y += dh;
}

// ----------------------------------------------------------------------//

void CRect2D::Zoom( f32 dw, f32 dh )
{
	m_size.x += dw;
	m_size.y += dh;
	m_position.x -= ( dw * 0.5f );
	m_position.y -= ( dh * 0.5f );
}

// ----------------------------------------------------------------------//

void CRect2D::Resize( f32 w, f32 h )
{
	m_size.x = w;
	m_size.y = h;
}

// ----------------------------------------------------------------------//

void CRect2D::Scale( f32 kw, f32 kh )
{
	m_size.x *= kw;
	m_size.y *= kh;
}

// ----------------------------------------------------------------------//

void CRect2D::Union( CRect2D *r1, CRect2D *r2, CRect2D *dst )
{

	f32 x = r1->m_position.x	> r2->m_position.x  ? r2->m_position.x	: r1->m_position.x;
	f32 y = r1->m_position.y	> r2->m_position.y  ? r2->m_position.y	: r1->m_position.y;
	f32 r = r1->Right()			< r2->Right()		? r2->Right()		: r1->Right();
	f32 b = r1->Bottom()		< r2->Bottom()		? r2->Bottom()		: r1->Bottom();

	dst->Bound( x, y, r - x, b - y );
}

// ----------------------------------------------------------------------//

void CRect2D::Intersect( CRect2D *r1, CRect2D *r2, CRect2D *dst )
{

	f32 x = r1->m_position.x	> r2->m_position.x  ? r1->m_position.x  : r2->m_position.x;
	f32 y = r1->m_position.y	> r2->m_position.y  ? r1->m_position.y  : r2->m_position.y;
	f32 r = r1->Right()			< r2->Right()		? r1->Right()		: r2->Right();
	f32 b = r1->Bottom()		< r2->Bottom()		? r1->Bottom()		: r2->Bottom();

	dst->Bound( x, y, r - x, b - y );
}

// ----------------------------------------------------------------------//

void CRect2D::SetCenter( TVec2 *loc )
{
	m_position.x = loc->x - ( m_size.x * 0.5f );
	m_position.y = loc->y - ( m_size.y * 0.5f );
}

// ----------------------------------------------------------------------//

void CRect2D::GetCenter( TVec2 *loc )
{
	loc->x = m_position.x + ( m_size.x * 0.5f );
	loc->y = m_position.y + ( m_size.y * 0.5f );
}

// ----------------------------------------------------------------------//

void CRect2D::Align( CRect2D *c, u32 align )
{
	if( !align ) align = k_Align_Default;

	if( BitGet( align, k_Align_Right ) )
	{
		if( BitGet( align, k_Align_OutSide ) ) {
			m_position.x = c->Right();
		}
		else {
			m_position.x = c->Right() - m_size.x;
		}
	}

	if( BitGet( align, k_Align_Left ) )
	{
		if( BitGet( align, k_Align_OutSide ) ) {
			m_position.x = c->m_position.x - m_size.x;
		}
		else {
			m_position.x = c->m_position.x;
		}
	}

	if( BitGet( align, k_Align_Bottom ) )
	{
		if( BitGet( align, k_Align_OutSide ) ) {
			m_position.y = c->Bottom();
		}
		else {
			m_position.y = c->Bottom() - m_size.y;
		}
	}

	if( BitGet( align, k_Align_Top ) )
	{
		if( BitGet( align, k_Align_OutSide ) ) {
			m_position.y = c->Top() - Height();
		}
		else {
			m_position.y = c->Top();
		}
	}

	if( BitGet( align, k_Align_HorCenter ) ) {
		m_position.x = c->m_position.x + ( ( c->m_size.x - m_size.x )* 0.5f );
	}

	if( BitGet( align, k_Align_VerCenter ) ) {
		m_position.y = c->m_position.y + ( ( c->m_size.y - m_size.y )* 0.5f );
	}
}

// ----------------------------------------------------------------------//

void CRect2D::Transform( u32 trans )
{
	if( trans == k_Transform_Default ) return;

	if( BitGet( trans, k_Transform_FlipHor ) )
	{
		m_position.x = Right();
		m_size.x	 = -m_size.x;
	}

	if( BitGet( trans, k_Transform_FlipVer ) )
	{
		m_position.y = Bottom();
		m_size.y	 = -m_size.y;
	}

	if( BitGet( trans, k_Transform_Rot90 ) )
	{
		f32 f;
		TVec2 center;

		GetCenter( &center );

		m_position.x = center.x + Height() * 0.5f;
		m_position.y = center.y + Width() * 0.5f;

		f = m_size.x;
		m_size.x = -m_size.y;
		m_size.y = -f;
	}

	if( BitGet( trans, k_Transform_Rot180 ) )
	{
		m_position.x = Right();
		m_position.y = Bottom();

		m_size.x = -m_size.x;
		m_size.y = -m_size.y;
	}

	if( BitGet( trans, k_Transform_Rot270 ) )
	{
		f32 f;
		TVec2 center;

		GetCenter( &center );

		m_position.x = center.x + Height() * 0.5f;
		m_position.y = center.y - Width() * 0.5f;

		f = m_size.x;
		m_size.x = -m_size.y;
		m_size.y = +f;
	}
}


// ----------------------------------------------------------------------//
bool CRect2D::IsInside( f32 x, f32 y )
{
	return ( x > m_position.x && x < this->Right() && y > m_position.y && y < this->Top() );
}

// ----------------------------------------------------------------------//

bool CRect2D::IsInside( TVec2 *v )
{
	return ( v->x >= m_position.x && v->x < this->Right() && v->y >= m_position.y && v->y < this->Top() );
}

// ----------------------------------------------------------------------//

void CRect2D::Render( CDriver* driver, CRect2D *texRect, TMatrix4 *transform )
{
	static f32 v[ 20 ];

	v[  0 ] = m_position.x;
	v[  1 ] = m_position.y;
	v[  2 ] = 1.0f;
	v[  3 ] = texRect->Left();
	v[  4 ] = texRect->Top();

	v[  5 ] = m_position.x + m_size.x;
	v[  6 ] = m_position.y;
	v[  7 ] = 1.0f;
	v[  8 ] = texRect->Right();
	v[  9 ] = texRect->Top();

	v[ 10 ] = m_position.x;
	v[ 11 ] = m_position.y + m_size.y;
	v[ 12 ] = 0.0f;
	v[ 13 ] = texRect->Left();
	v[ 14 ] = texRect->Bottom();

	v[ 15 ] = m_position.x + m_size.x;
	v[ 16 ] = m_position.y + m_size.y;
	v[ 17 ] = 1.0f;
	v[ 18 ] = texRect->Right();
	v[ 19 ] = texRect->Bottom();

	if( transform != NULL )
	{
		const f32 *vertices[] = { &v[ 0 ], &v[ 5 ] , &v[ 10 ], &v[ 15 ] };
		TVec3 w;

		for( s32 k = 0; k < 4; k++ )
		{
			Matrix4Transform( transform , (TVec3*) vertices[ k ], &w );

			v[ 5 * k + 0 ] = w.x;
			v[ 5 * k + 1 ] = w.y;
			v[ 5 * k + 2 ] = w.z;
		}
	}

	CVertexSource	vs;
	vs.m_type			= CVertexSource::k_Type_Triangle;
	vs.m_vertexFormat	= CVertexSource::k_Vertex_Attribute_Format_Position | CVertexSource::k_Vertex_Attribute_Format_TexCoord_0;
	vs.m_vertexStride	= CVertexSource::k_Vertex_Attribute_Offset_Position + CVertexSource::k_Vertex_Attribute_Offset_TexCoord_0;
	vs.m_vboSize		= 4;

	vs.m_vboData	= &v[0];

	static u16 i[6];

	i[ 0 ] = 2; i[ 1 ] = 1; i[ 2 ] = 0;
	i[ 3 ] = 2; i[ 4 ] = 3; i[ 5 ] = 1;

	CVertexGroup	vg;
	vg.m_vboData	= &i[0];
	vg.m_vboSize = 6;
	vg.SetMaterial( m_material );
	vg.SetVertexSource( &vs );

	driver->Render(&vg);

	vs.m_vboData = NULL;
	vg.m_vboData = NULL;
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
