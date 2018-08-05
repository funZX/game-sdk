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
CRect2D				OneSizeRectStatic( 0.0f, 0.0f, 1.0f, 1.0f );
CRect2D				OneSizeRectFlipStatic( 1.0f, 1.0f, -1.0f, -1.0f );
const CRect2D*		CRect2D::OneSizeRect		= &OneSizeRectStatic;
const CRect2D*		CRect2D::OneSizeRectFlip	= &OneSizeRectFlipStatic;
// ----------------------------------------------------------------------//
CRect2D::CRect2D()
{
	Vec2ToZero(&m_position);
	Vec2ToZero(&m_size);

	m_rotation	= 0.0f;
	m_material	= nullptr;

	m_texRect = &OneSizeRectStatic;
}
// ----------------------------------------------------------------------//

CRect2D::CRect2D( const std::string& name )
	: CRect2D()
{
	m_name = name;
}
// ----------------------------------------------------------------------//

CRect2D::CRect2D(f32 x, f32 y, f32 width, f32 height)
	: CRect2D()
{
	m_position.x = x;
	m_position.y = y;

	m_size.x = width;
	m_size.y = height;
}
// ----------------------------------------------------------------------//

f32	CRect2D::Left( void ) const
{
	return m_position.x;
}

// ----------------------------------------------------------------------//

f32 CRect2D::Right( void ) const
{
	return ( m_position.x + m_size.x );
}

// ----------------------------------------------------------------------//

f32 CRect2D::Top( void ) const
{
	return m_position.y;
}

// ----------------------------------------------------------------------//

f32 CRect2D::Bottom( void ) const
{
	return ( m_position.y + m_size.y );
}

// ----------------------------------------------------------------------//

f32 CRect2D::Width( void ) const
{
	return m_size.x;
}

// ----------------------------------------------------------------------//

f32 CRect2D::Height( void ) const
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

	OnResize();
}

// ----------------------------------------------------------------------//

void CRect2D::Bound( CRect2D *r )
{
	m_position.x		= r->m_position.x;
	m_position.y		= r->m_position.y;
	m_size.x			= r->m_size.x;
	m_size.y			= r->m_size.y;

	OnResize();
}

// ----------------------------------------------------------------------//

void CRect2D::Move( TVec2 *d )
{
	m_position.x += d->x;
	m_position.y += d->y;

	OnMove();
}

// ----------------------------------------------------------------------//

void CRect2D::Move( f32 dx, f32 dy )
{
	m_position.x += dx;
	m_position.y += dy;

	OnMove();
}

// ----------------------------------------------------------------------//

void CRect2D::MoveTo( f32 x, f32 y )
{
	m_position.x = x;
	m_position.y = y;

	OnMove();
}

// ----------------------------------------------------------------------//

void CRect2D::MoveTo( TVec2 *pos )
{
	m_position.x = pos->x;
	m_position.y = pos->y;

	OnMove();
}

// ----------------------------------------------------------------------//

void CRect2D::Inflate( f32 dw, f32 dh )
{
	m_size.x += dw;
	m_size.y += dh;

	OnResize();
}

// ----------------------------------------------------------------------//

void CRect2D::Zoom( f32 dw, f32 dh )
{
	m_size.x += dw;
	m_size.y += dh;
	m_position.x -= ( dw * 0.5f );
	m_position.y -= ( dh * 0.5f );

	OnResize();
}

// ----------------------------------------------------------------------//

void CRect2D::Resize( f32 w, f32 h )
{
	m_size.x = w;
	m_size.y = h;

	OnResize();
}

// ----------------------------------------------------------------------//

void CRect2D::Scale( f32 kw, f32 kh )
{
	m_size.x *= kw;
	m_size.y *= kh;

	OnResize();
}

// ----------------------------------------------------------------------//

void CRect2D::Rotate( f32 rotation)
{
	m_rotation = rotation;
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

	OnMove();
}

// ----------------------------------------------------------------------//

void CRect2D::GetCenter( TVec2 *loc ) const 
{
	loc->x = m_position.x + ( m_size.x * 0.5f );
	loc->y = m_position.y + ( m_size.y * 0.5f );
}

// ----------------------------------------------------------------------//

void CRect2D::Align( CRect2D *c, K_ALIGN align )
{
	switch (align)
	{
	case k_Align_RightIn:
		m_position.x = c->Right() - m_size.x;
		break;

	case k_Align_RightOut:
		m_position.x = c->Right();
		break;

	case k_Align_LeftIn:
		m_position.x = c->m_position.x;
		break;

	case k_Align_LeftOut:
		m_position.x = c->m_position.x - m_size.x;
		break;

	case k_Align_BottomIn:
		m_position.y = c->Bottom() - m_size.y;
		break;

	case k_Align_BottomOut:
		m_position.y = c->Bottom();
		break;

	case k_Align_TopIn:
		m_position.y = c->Top();
		break;

	case k_Align_TopOut:
		m_position.y = c->Top() - Height();
		break;

	case k_Align_HorCenter:
		m_position.x = c->m_position.x + ( ( c->m_size.x - m_size.x ) * 0.5f );
		break;

	case k_Align_VerCenter:
		m_position.y = c->m_position.y + ( ( c->m_size.y - m_size.y ) * 0.5f );
		break;

	case k_Align_Center:
		m_position.x = c->m_position.x + ( ( c->m_size.x - m_size.x ) * 0.5f );
		m_position.y = c->m_position.y + ( ( c->m_size.y - m_size.y ) * 0.5f );
		break;
	}

	OnMove();
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
void CRect2D::OnResize()
{

}

// ----------------------------------------------------------------------//
void CRect2D::OnMove()
{

}

// ----------------------------------------------------------------------//
void CRect2D::OnRotate()
{

}

// ----------------------------------------------------------------------//

void CRect2D::Render(CDriver* driver)
{
	Render( driver, CRect2D::OneSizeRect );
}

// ----------------------------------------------------------------------//

void CRect2D::Update( f32 dt, void *userData )
{

}

// ----------------------------------------------------------------------//

void CRect2D::Render( CDriver* driver, const CRect2D *texRect )
{
	static f32		v[ 20 ];
	static TVec2	r[ 4 ];

	Vec2Set( &r[0], m_position.x,				m_position.y );
	Vec2Set( &r[1], m_position.x + m_size.x,	m_position.y );
	Vec2Set( &r[2], m_position.x,				m_position.y + m_size.y);
	Vec2Set( &r[3], m_position.x + m_size.x,	m_position.y + m_size.y);

	if (m_rotation != 0.0f)
	{
		TVec2 center;

		GetCenter( &center );

		Vec2Rotate( &r[ 0 ], &center, m_rotation );
		Vec2Rotate( &r[ 1 ], &center, m_rotation );
		Vec2Rotate( &r[ 2 ], &center, m_rotation );
		Vec2Rotate( &r[ 3 ], &center, m_rotation );
	}

	v[  0 ] = r[ 0 ].x;
	v[  1 ] = r[ 0 ].y;
	v[  2 ] = 1.0f;
	v[  3 ] = texRect->Left();
	v[  4 ] = texRect->Top();

	v[  5 ] = r[ 1 ].x;
	v[  6 ] = r[ 1 ].y;
	v[  7 ] = 1.0f;
	v[  8 ] = texRect->Right();
	v[  9 ] = texRect->Top();

	v[ 10 ] = r[ 2 ].x;
	v[ 11 ] = r[ 2 ].y;
	v[ 12 ] = 1.0f;
	v[ 13 ] = texRect->Left();
	v[ 14 ] = texRect->Bottom();

	v[ 15 ] = r[ 3 ].x;
	v[ 16 ] = r[ 3 ].y;
	v[ 17 ] = 1.0f;
	v[ 18 ] = texRect->Right();
	v[ 19 ] = texRect->Bottom();

	CVertexSource	vs;
	vs.m_type			= CVertexSource::Type::Triangle;
	vs.m_vertexFormat	= CVertexSource::k_Vertex_Attribute_Format_Position | CVertexSource::k_Vertex_Attribute_Format_TexCoord_0;
	vs.m_vertexStride	= CVertexSource::k_Vertex_Attribute_Offset_Position + CVertexSource::k_Vertex_Attribute_Offset_TexCoord_0;
	vs.m_vboSize		= 4;

	vs.m_vboData		= &v[0];

	static u16 i[6];

	i[ 0 ] = 2; i[ 1 ] = 1; i[ 2 ] = 0;
	i[ 3 ] = 2; i[ 4 ] = 3; i[ 5 ] = 1;

	CVertexGroup	vg;
	vg.m_vboData	= &i[0];
	vg.m_vboSize = 6;
	vg.SetMaterial( m_material );
	vg.SetVertexSource( &vs );

	driver->Render(&vg);

	vs.m_vboData = nullptr;
	vg.m_vboData = nullptr;
}

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
