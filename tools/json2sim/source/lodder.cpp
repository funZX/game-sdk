#include "data.h"

#include "lodder.h"

#include <core/io/sim_file_stream.h>
#include <core/io/sim_file_system.h>
#include <core/io/sim_mem_stream.h>
#include <render/sim_mesh.h>
#include <render/sim_vertex_source.h>
#include <render/sim_vertex_group.h>

// ----------------------------------------------------------------------//
CLodder::Vertex::Vertex( f32* src, u16 id )
{
	this->src	= src;
	this->id	= id;

	cost		= 0.0f;
	collapse	= NULL;
}

CLodder::Vertex::~Vertex()
{
	for (u32 k = 0; k < neighbors.size(); k++)
		neighbors[k]->RemoveNeighbour(this);
	neighbors.clear();

	triangles.clear();
}

void CLodder::Vertex::AddNeighbour( Vertex* v )
{
	if (std::find(neighbors.begin(), neighbors.end(), v) == neighbors.end())
		neighbors.push_back(v);
}

void CLodder::Vertex::RemoveNeighbour( Vertex* v )
{
	std::vector<Vertex*>::iterator it = std::find(neighbors.begin(), neighbors.end(), v);

	if ( it != neighbors.end() )
		neighbors.erase( it );
}

void CLodder::Vertex::AddTriangle( Triangle* tri )
{
	if (std::find(triangles.begin(), triangles.end(), tri) == triangles.end())
		triangles.push_back(tri);
}

void CLodder::Vertex::RemoveTriangle( Triangle* tri )
{
	std::vector<Triangle*>::iterator it = std::find(triangles.begin(), triangles.end(), tri);

	if ( it != triangles.end() )
		triangles.erase( it );
}

// ----------------------------------------------------------------------//

CLodder::Triangle::Triangle( Vertex* v0, Vertex* v1, Vertex* v2, u32 id )
{
	this->id = id;

	this->v0 = v0;
	this->v1 = v1;
	this->v2 = v2;

	Vec3ToDirZ(&this->normal);
}

bool CLodder::Triangle::HasVertex( Vertex* v )
{
	return v0 == v || v1 ==v || v2 == v;
}

void CLodder::Triangle::ReplaceVertex( Vertex* v, Vertex* u )
{
	if ( v0 == v )
	{
		v0 = u;

		v1->AddNeighbour(u);
		v2->AddNeighbour(u);
		
		u->AddNeighbour(v1);
		u->AddNeighbour(v2);

		SIM_ASSERT(v1 != u);
		SIM_ASSERT(v2 != u);
	}

	if ( v1 == v ) 
	{
		v1 = u;

		v0->AddNeighbour(u);
		v2->AddNeighbour(u);

		u->AddNeighbour(v0);
		u->AddNeighbour(v2);

		SIM_ASSERT(v0 != u);
		SIM_ASSERT(v2 != u);
	}

	if ( v2 == v )
	{
		v2 = u;

		v0->AddNeighbour(u);
		v1->AddNeighbour(u);

		u->AddNeighbour(v0);
		u->AddNeighbour(v1);

		SIM_ASSERT(v0 != u);
		SIM_ASSERT(v1 != u);
	}
}

void CLodder::Triangle::ComputeNormal()
{
	TVec3* u = (TVec3*) v0->src;
	TVec3* v = (TVec3*) v1->src;
	TVec3* w = (TVec3*) v2->src;

	TVec3 p, q;

	Vec3Diff( &p, w, u );
	Vec3Diff( &q, v, u );

	Vec3Cross( &normal, &p, &q );
	Vec3Normalize( &normal );
}

// ----------------------------------------------------------------------//

CLodder :: CLodder()
{
}

CLodder :: ~CLodder()
{
	for (u32 k = 0; k < m_triangleList.size(); k++)
		SIM_SAFE_DELETE( m_triangleList[ k ] );
	m_triangleList.clear();

	for (u32 k = 0; k < m_vertexList.size(); k++)
		SIM_SAFE_DELETE( m_vertexList[ k ] );
	m_vertexList.clear();
}

CLodder::Vertex* CLodder::AddVertex( f32* src, s32 id )
{
	for ( u32 k = 0; k < m_vertexList.size(); k++ )
		if (id == m_vertexList[ k ]->id)
			return m_vertexList[ k ];

	Vertex* vertex = SIM_NEW CLodder::Vertex( src, id );
	m_vertexList.push_back( vertex );

	return vertex;
}

// ----------------------------------------------------------------------//

void CLodder::DeleteVertex( CLodder::Vertex* v )
{
	TVertexListIter it = std::find( m_vertexList.begin(), m_vertexList.end(), v );

	if ( it != m_vertexList.end() )
	{
		SIM_SAFE_DELETE(*it);
		m_vertexList.erase(it);
	}
}

// ----------------------------------------------------------------------//

void CLodder::DeleteTriangle( CLodder::Triangle* tri )
{
	TTriangleListIter it = std::find( m_triangleList.begin(), m_triangleList.end(), tri );

	if ( it != m_triangleList.end() )
	{
		SIM_SAFE_DELETE(*it);
		m_triangleList.erase(it);
	}
}

// ----------------------------------------------------------------------//

CLodder::Vertex* CLodder::MinimumCostEdge()
{
	Vertex* min = NULL;
	f32 mincost = FLT_MAX;

	for ( u32 k = 0; k < m_vertexList.size(); k++ )
	{
		Vertex* v = m_vertexList[ k ];

		if ( v->cost < mincost )
		{
			min		= v;
			mincost = v->cost;
		}
	}

	return min;
}

// ----------------------------------------------------------------------//

f32	CLodder::ComputeEdgeCollapseCost( Vertex* u, Vertex* v )
{
	TVec3 dir;
	Vec3Set( &dir, v->src[0], v->src[1], v->src[2] );
	Vec3Diff(&dir, (TVec3*)u->src);

	f32 edgelength	= Vec3Magnitude( &dir );
	f32 curvature	= 0;
	
	std::vector<Triangle*> sides;
	
	for ( u32 k = 0; k < u->triangles.size(); k++ )
		if( u->triangles[ k ]->HasVertex( v ) )
			sides.push_back( u->triangles[ k ] );

	
	for ( u32 k = 0; k < u->triangles.size(); k++ )
	{
		Triangle* tri0	= u->triangles[ k ];
		f32 mincurv		= 1.0f;

		for ( u32 i = 0; i < sides.size(); i++ )
		{
			Triangle* tri1	= sides[ i ];

			f32 dot = Vec3Dot( &tri0->normal, &tri1->normal);

			mincurv = SIM_MIN( mincurv, ( 1 - dot ) / 2.0f );
		}

		curvature = SIM_MAX( curvature, mincurv );
	}

	return edgelength * curvature;
}

// ----------------------------------------------------------------------//

void CLodder::ComputeEdgeCostAtVertex( Vertex* v )
{
	if ( v->neighbors.size() == 0 )
	{
		v->collapse = NULL;
		v->cost		= -0.01f;

		return;
	}

	v->cost			= FLT_MAX;
	v->collapse		= NULL;

	for ( u32 k = 0; k < v->neighbors.size(); k++ )
	{
		Vertex* u	= v->neighbors[ k ];
		f32 c		= ComputeEdgeCollapseCost( v, u );

		if (c < v->cost )
		{
			v->collapse = u;
			v->cost = c;
		}
	}
}

// ----------------------------------------------------------------------//

void CLodder::Collapse( Vertex* u, Vertex* v )
{
	if ( !v )
	{
		TVertexListIter it = std::find( m_vertexList.begin(), m_vertexList.end(), u);
		
		SIM_SAFE_DELETE(*it);
		m_vertexList.erase(it);

		return;
	}

	TVertexList xn		= u->neighbors;
	TTriangleList xt	= u->triangles;

	for ( u32 k = 0; k < xt.size(); k++ )
	{
		Triangle* tri = xt[ k ];

		if ( tri->HasVertex( v ) )
		{
			tri->v0->RemoveTriangle( tri );
			tri->v1->RemoveTriangle( tri );
			tri->v2->RemoveTriangle( tri );

			DeleteTriangle( tri );
		}
	}

	for ( u32 k = 0; k < u->triangles.size(); k++ )
	{
		Triangle* tri = u->triangles[ k ];

		tri->ReplaceVertex( u, v );
		v->AddTriangle(tri);

		SIM_ASSERT( tri->v0->neighbors.size() > 1);
		SIM_ASSERT( tri->v1->neighbors.size() > 1);
		SIM_ASSERT( tri->v2->neighbors.size() > 1);

		SIM_ASSERT( tri->v0->triangles.size() > 0);
		SIM_ASSERT( tri->v1->triangles.size() > 0);
		SIM_ASSERT( tri->v2->triangles.size() > 0);
	}

	Vertex sv = (*u);
	DeleteVertex( u );

	for ( u32 n = 0; n < xn.size(); n++ )
	{
		Vertex* xv = xn[ n ];

		SIM_ASSERT( xv->neighbors.size() > 1);
		SIM_ASSERT( xv->triangles.size() > 0);

		ComputeEdgeCostAtVertex( xv );
	}
}

// ----------------------------------------------------------------------//

CVertexGroup* CLodder::CreateLOD( u32 depth )
{
	for (u32 k = 0; k < m_vertexList.size(); k++)
		ComputeEdgeCostAtVertex( m_vertexList[ k ] );

	u32 maxVertices = m_vertexList.size() * depth / 100;
	u32 nVertices	= m_vertexList.size();

	while( nVertices > maxVertices )
	{
		Vertex* v = MinimumCostEdge();
		Collapse( v, v->collapse );

		nVertices	= m_vertexList.size();
	}

	CVertexGroup* vertexGroup = SIM_NEW CVertexGroup();

	vertexGroup->m_vboSize = 3 * m_triangleList.size();
	vertexGroup->m_vboData = SIM_NEW u16[vertexGroup->m_vboSize];

	for ( u32 k = 0, i = 0; i < m_triangleList.size(); k += 3, i++ )
	{
		Triangle* tri = m_triangleList[ i ];

		vertexGroup->m_vboData[ 3 * i + 0 ] = tri->v0->id;
		vertexGroup->m_vboData[ 3 * i + 1 ] = tri->v1->id;
		vertexGroup->m_vboData[ 3 * i + 2 ] = tri->v2->id;
	}

	return vertexGroup;
}