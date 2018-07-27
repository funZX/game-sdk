#ifndef __SIM2OPT_TRIANGLE_H
#define __SIM2OPT_TRIANGLE_H

#include <sim_engine.h>

class Vertex;

class Triangle
{
public:
	Triangle( Vertex* v0, Vertex* v1, Vertex* v2, u32 id );

	void			ComputeNormal();
	void			ReplaceVertex( Vertex* v, Vertex* u );
	bool			HasVertex( Vertex* v );

public:
	u32				id;

	Vertex*			v0;
	Vertex*			v1;
	Vertex*			v2;

	TVec3			normal;
};

#endif // __SIM2OPT_TRIANGLE_H