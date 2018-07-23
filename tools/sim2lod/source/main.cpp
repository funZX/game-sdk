#include <engine.h>

#include <core/io/sim_file_stream.h>
#include <core/io/sim_file_system.h>
#include <core/io/sim_mem_stream.h>
#include <render/sim_mesh.h>
#include <render/sim_vertex_source.h>
#include <render/sim_vertex_group.h>

#include <Triangle.h>
#include <Vertex.h>

void			onStart();

Vertex*			AddVertex( std::vector<Vertex*>& vertices, f32* src, s32 id );

void			DeleteVertex( std::vector<Vertex*>& vertices, Vertex* v );
void			DeleteTriangle( std::vector<Triangle*>& triangles, Triangle* tri );

Vertex*			MinimumCostEdge( std::vector<Vertex*>& vertices );
f32				ComputeEdgeCollapseCost( Vertex* u, Vertex* v);
void			ComputeEdgeCostAtVertex( Vertex* v);

void			Collapse( Vertex* u, Vertex* v, std::vector<Vertex*>& vertices, std::vector<Triangle*>& triangles );
CVertexGroup*	CreateLOD( std::vector<Vertex*>& vertices, std::vector<Triangle*>& triangles, u32 depth );


int main(int argc, char* argv[])
{
	if( argc != 3 )
	{
		printf( "\nsim2opt\tinput.sim\toutput.sim" );
		exit( EXIT_SUCCESS );
	}

	onStart();

	std::string input(argv[ 1 ] );
	std::string output(argv[ 2 ] );

	sim::rnr::CMesh* mesh = SIM_NEW sim::rnr::CMesh( "" );
	sim::io::CFileStream fs(input);

	fs.Open();

	sim::io::CMemStream ms( fs.Read(0), fs.GetSize() );
	mesh->Load( &ms );

	fs.Close();

	if (mesh->m_vertexSource->GetType() != CVertexSource::k_Type_Triangle)
	{
		printf( "\nmodel vertex source is not formed of triangles" );
		exit( EXIT_SUCCESS );	
	}

	CVertexSource* vertexSource = mesh->m_vertexSource;

	std::vector<Triangle*>	triangles;
	std::vector<Vertex*>	vertices;

	CVertexGroup* vertexGroup = mesh->m_vertexGroup;

	for (u32 i = 0; i < vertexGroup->GetVboSize(); i += 3)
	{
		u32 vOffset = vertexSource->m_vertexStride / sizeof(f32);

		u16 i0  = vertexGroup->m_vboData[i + 0]; 
		u16 i1  = vertexGroup->m_vboData[i + 1];
		u16 i2  = vertexGroup->m_vboData[i + 2];

		f32* v0 = &vertexSource->m_vboData[ i0 * vOffset ];
		f32* v1 = &vertexSource->m_vboData[ i1 * vOffset ];
		f32* v2 = &vertexSource->m_vboData[ i2 * vOffset ];

		Vertex* x0 = AddVertex(vertices, v0, i0 );
		Vertex* x1 = AddVertex(vertices, v1, i1 );
		Vertex* x2 = AddVertex(vertices, v2, i2 );

		x0->AddNeighbour(x1);
		x0->AddNeighbour(x2);
		x1->AddNeighbour(x2);
		x1->AddNeighbour(x0);
		x2->AddNeighbour(x0);
		x2->AddNeighbour(x1);

		Triangle* tri = SIM_NEW Triangle( x0, x1, x2, i / 3 );

		x0->AddTriangle(tri);
		x1->AddTriangle(tri);
		x2->AddTriangle(tri);

		tri->ComputeNormal();

		triangles.push_back( tri );
	}

	const u32 nLODS = 4;
	const u32 LOD_DETAIL[] = { 70, 50, 30, 10 }; 

	CVertexGroup* lod = vertexGroup;

	for ( u32 k = 0; k < nLODS; k++ )
	{
		CVertexGroup* nextlod = CreateLOD( vertices, triangles, LOD_DETAIL[k] );

		nextlod->m_vertexSource = vertexSource;

		lod->m_lod	= nextlod;
		lod			= nextlod;
	}

	for (u32 k = 0; k < triangles.size(); k++)
		SIM_SAFE_DELETE( triangles[ k ] );
	triangles.clear();

	for (u32 k = 0; k < vertices.size(); k++)
		SIM_SAFE_DELETE( vertices[ k ] );
	vertices.clear();

	CMemStream writems(1024*1024);

	mesh->Save( &writems );

	u32 memsize = 
	writems.GetCursor();
	writems.Rewind();

	FILE* file = fopen( output.c_str(), "wb" );
	fwrite( writems.Read(0), memsize, 1, file );	
	fclose( file );

	SIM_SAFE_DELETE( mesh );

	return EXIT_SUCCESS;
}

// ----------------------------------------------------------------------//

Vertex* AddVertex( std::vector<Vertex*>& vertices, f32* src, s32 id )
{
	for ( u32 k = 0; k < vertices.size(); k++ )
		if (id == vertices[ k ]->id)
			return vertices[ k ];

	Vertex* vertex = SIM_NEW Vertex( src, id );
	vertices.push_back( vertex );

	return vertex;
}

// ----------------------------------------------------------------------//

void DeleteVertex( std::vector<Vertex*>& vertices, Vertex* v )
{
	std::vector<Vertex*>::iterator it = std::find(vertices.begin(), vertices.end(), v);

	if ( it != vertices.end() )
	{
		SIM_SAFE_DELETE(*it);
		vertices.erase(it);
	}
}

// ----------------------------------------------------------------------//

void DeleteTriangle( std::vector<Triangle*>& triangles, Triangle* tri )
{
	std::vector<Triangle*>::iterator it = std::find(triangles.begin(), triangles.end(), tri);

	if ( it != triangles.end() )
	{
		SIM_SAFE_DELETE(*it);
		triangles.erase(it);
	}
}

// ----------------------------------------------------------------------//

Vertex* MinimumCostEdge( std::vector<Vertex*>& vertices )
{
	Vertex* min = NULL;
	f32 mincost = FLT_MAX;

	for ( u32 k = 0; k < vertices.size(); k++ )
	{
		Vertex* v = vertices[ k ];

		if ( v->cost < mincost )
		{
			min		= v;
			mincost = v->cost;
		}
	}

	return min;
}

// ----------------------------------------------------------------------//

f32	ComputeEdgeCollapseCost( Vertex* u, Vertex* v )
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

void	ComputeEdgeCostAtVertex( Vertex* v )
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

void	Collapse( Vertex* u, Vertex* v, std::vector<Vertex*>& vertices, std::vector<Triangle*>& triangles )
{
	if ( !v )
	{
		std::vector<Vertex*>::iterator it = std::find(vertices.begin(), vertices.end(), u);
		SIM_SAFE_DELETE(*it);
		vertices.erase(it);

		return;
	}

	std::vector<Vertex*> xn		= u->neighbors;
	std::vector<Triangle*> xt	= u->triangles;

	for ( u32 k = 0; k < xt.size(); k++ )
	{
		Triangle* tri = xt[ k ];

		if ( tri->HasVertex( v ) )
		{
			tri->v0->RemoveTriangle( tri );
			tri->v1->RemoveTriangle( tri );
			tri->v2->RemoveTriangle( tri );

			DeleteTriangle( triangles, tri );
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
	DeleteVertex( vertices, u );

	for ( u32 n = 0; n < xn.size(); n++ )
	{
		Vertex* xv = xn[ n ];

		SIM_ASSERT( xv->neighbors.size() > 1);
		SIM_ASSERT( xv->triangles.size() > 0);

		ComputeEdgeCostAtVertex( xv );
	}
}

// ----------------------------------------------------------------------//

CVertexGroup*	CreateLOD( std::vector<Vertex*>& vertices, std::vector<Triangle*>& triangles, u32 depth )
{
	for (u32 k = 0; k < vertices.size(); k++)
		ComputeEdgeCostAtVertex( vertices[ k ] );

	u32 maxVertices = vertices.size() * depth / 100;
	u32 nVertices	= vertices.size();

	while( nVertices > maxVertices )
	{
		Vertex* v = MinimumCostEdge( vertices );
		Collapse( v, v->collapse, vertices, triangles );

		nVertices	= vertices.size();
	}

	CVertexGroup* vertexGroup = SIM_NEW CVertexGroup();

	vertexGroup->m_vboSize = 3 * triangles.size();
	vertexGroup->m_vboData = SIM_NEW u16[vertexGroup->m_vboSize];

	for ( u32 k = 0, i = 0; i < triangles.size(); k += 3, i++ )
	{
		Triangle* tri = triangles[ i ];

		vertexGroup->m_vboData[ 3 * i + 0 ] = tri->v0->id;
		vertexGroup->m_vboData[ 3 * i + 1 ] = tri->v1->id;
		vertexGroup->m_vboData[ 3 * i + 2 ] = tri->v2->id;
	}

	return vertexGroup;
}

// ----------------------------------------------------------------------//

void onQuit()
{
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
}

void onStart()
{
#ifdef _DEBUG
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_crtBreakAlloc = 138;
#endif

	atexit(onQuit);
}

