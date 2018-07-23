#ifndef __SIM2OPT_VERTEX_H
#define __SIM2OPT_VERTEX_H

#include <engine.h>
#include <core/sim_list.h>

class Triangle;

class Vertex
{
public:
	Vertex( f32* src, u16 id );
	~Vertex();

	void						AddNeighbour( Vertex* v );
	void						RemoveNeighbour( Vertex* v );

	void						AddTriangle( Triangle* tri );
	void						RemoveTriangle( Triangle* tri );

public:
	f32*						src;
	u16							id;

	std::vector<Vertex*>		neighbors;
	std::vector<Triangle*>		triangles;

	f32							cost;
	Vertex*						collapse;
};

#endif // __SIM2OPT_VERTEX_H