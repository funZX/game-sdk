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
#include <render/sim_material.h>
#include <render/sim_effect.h>
#include <render/sim_vertex_group.h>
#include <render/sim_vertex_source.h>

#include <core/sim_polygon.h>

namespace sim
{
using namespace rnr;

namespace core
{
// ----------------------------------------------------------------------//

CPolygon::CPolygon()
{

}
// ----------------------------------------------------------------------//
CPolygon::~CPolygon()
{

}
// ----------------------------------------------------------------------//
void CPolygon::AddVertex( const f32 x, const f32 y )
{
	TVec2 v;

	v.x = x;
	v.y = y;

	AddToEnd( v );
}
// ----------------------------------------------------------------------//
void CPolygon::AddVertex( const TVec2* v )
{
	AddVertex( v->x, v->y );
}
// ----------------------------------------------------------------------//
void CPolygon::Translate( const f32 x, const f32 y )
{
	TVec2* p, v;

	v.x = x;
	v.y = y;

	Begin();
	while ( p = Next() )
		Vec2Add( p, p, &v );
}

// ----------------------------------------------------------------------//
void CPolygon::Scale( const f32 kx, const f32 ky )
{
	TVec2* p = nullptr;

	Begin();

	while ( p = Next() )
	{
		p->x *= kx;
		p->y *= ky;
	}
}
// ----------------------------------------------------------------------//
const f32 CPolygon::GetMinX()
{
	f32 min = FLT_MAX;

	TVec2* p = nullptr;

	Begin();

	while ( p = Next() )
		if ( p->x < min )
			min = p->x;

	return min;
}
// ----------------------------------------------------------------------//
const f32 CPolygon::GetMinY()
{
	f32 min = FLT_MAX;

	TVec2* p = nullptr;

	Begin();

	while ( p = Next() )
		if ( p->y < min )
			min = p->y;

	return min;
}
// ----------------------------------------------------------------------//
const f32 CPolygon::GetMaxX()
{
	f32 max = -FLT_MAX;

	TVec2* p = nullptr;

	Begin();

	while ( p = Next() )
		if ( p->x > max )
			max = p->x;

	return max;
}
// ----------------------------------------------------------------------//
const f32 CPolygon::GetMaxY()
{
	f32 max = -FLT_MAX;

	TVec2* p = nullptr;

	Begin();

	while ( p = Next() )
		if ( p->y > max )
			max = p->y;

	return max;
}
// ----------------------------------------------------------------------//
bool CPolygon::IsInside( const TVec2* p )
{
	u32 wn = 0;

	TVec2 *v0 = nullptr, *v1 = nullptr;

	Begin();

	while ( ( v0 = Next() ) && ( v1 = Current() ) )
	{
		if ( v0->y <= p->y )
		{
			if ( v1->y > p->y )
				if ( 0 < Vec2Side( v0, v1, p ) )
					++wn;
		}
		else
		{
			if ( v1->y <= p->y )
				if ( 0 > Vec2Side( v0, v1, p ) )
					--wn;
		}
	}

	return wn != 0;
}
// ----------------------------------------------------------------------//
bool CPolygon::IsInside( const f32 x, const f32 y )
{
	TVec2 v;
	Vec2Set( &v, x, y );

	return IsInside( &v );
}
// ----------------------------------------------------------------------//
bool CPolygon::Intersects( CPolygon* polygon )
{
	TVec2* v = nullptr;

	polygon->Begin();

	while ( v = polygon->Next() )
		if ( IsInside( v ) )
			return true;

	return false;
}
// ----------------------------------------------------------------------//
CPolygon* CPolygon::Intersects( TPolygonList* polygonList )
{
	CPolygon** poly = nullptr;

	polygonList->Begin();

	while ( poly = polygonList->Next() )
		if ( (*poly)->Intersects( this ) )
			return (*poly);

	return nullptr;
}
// ----------------------------------------------------------------------//
void CPolygon::Render( rnr::CDriver *driver, rnr::CMaterial *material )
{
	CVertexGroup vg;
	CVertexSource vs;

	vg.SetVertexSource( &vs );
	
	s32 nVerts = Count();
	vg.m_vboData = SIM_NEW u16[ nVerts ];
	vg.m_vboSize = nVerts;

	vg.m_vertexSource->m_type			= CVertexSource::Type::LineStrip;
	vg.m_vertexSource->m_vertexFormat	= CVertexSource::AttributeFormat::Position;
	vg.m_vertexSource->m_vertexStride	= CVertexSource::AttributeStride::Position;
	vg.m_vertexSource->m_vboSize		= nVerts;

	s32 vtxSize		= Value(vg.m_vertexSource->m_vertexStride) / sizeof( f32 );
	s32 newFloats 	= nVerts * vtxSize;

	vg.m_vertexSource->m_vboData	= SIM_NEW f32[ newFloats ];

	TVec2* p	= nullptr;
	s32 k			= 0;
	s32  currIndex	= 0;
	f32* curVertex	= nullptr;

	Begin();
	while ( p = Next() )
	{
		currIndex	= k * vtxSize;
		
		curVertex	= &vg.m_vertexSource->m_vboData[ currIndex ];
		
		curVertex[ 0 ] = p->x;
		curVertex[ 1 ] = p->y;
		curVertex[ 2 ] = 0.0f;

		vg.m_vboData[ k ] = k++;
	};

	vg.SetMaterial( material );

	driver->Render( &vg );

	SIM_SAFE_DELETE_ARRAY( vg.m_vertexSource->m_vboData );
	SIM_SAFE_DELETE_ARRAY( vg.m_vboData );
}

// ----------------------------------------------------------------------//
}; // namespace sys
}; // namespace sim;
