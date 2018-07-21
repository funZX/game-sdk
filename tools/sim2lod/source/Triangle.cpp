#include <Vertex.h>
#include <Triangle.h>

Triangle::Triangle( Vertex* v0, Vertex* v1, Vertex* v2, u32 id )
{
	this->id = id;

	this->v0 = v0;
	this->v1 = v1;
	this->v2 = v2;

	Vec3ToDirZ(&this->normal);
}

bool Triangle::HasVertex( Vertex* v )
{
	return v0 == v || v1 ==v || v2 == v;
}

void Triangle::ReplaceVertex( Vertex* v, Vertex* u )
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

void Triangle::ComputeNormal()
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