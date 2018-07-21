#include <Vertex.h>
#include <Triangle.h>

Vertex::Vertex( f32* src, u16 id )
{
	this->src	= src;
	this->id	= id;

	cost		= 0.0f;
	collapse	= NULL;
}

Vertex::~Vertex()
{
	for (u32 k = 0; k < neighbors.size(); k++)
		neighbors[k]->RemoveNeighbour(this);
	neighbors.clear();

	triangles.clear();
}

void Vertex::AddNeighbour( Vertex* v )
{
	if (std::find(neighbors.begin(), neighbors.end(), v) == neighbors.end())
		neighbors.push_back(v);
}

void Vertex::RemoveNeighbour( Vertex* v )
{
	std::vector<Vertex*>::iterator it = std::find(neighbors.begin(), neighbors.end(), v);

	if ( it != neighbors.end() )
		neighbors.erase( it );
}

void Vertex::AddTriangle( Triangle* tri )
{
	if (std::find(triangles.begin(), triangles.end(), tri) == triangles.end())
		triangles.push_back(tri);
}

void Vertex::RemoveTriangle( Triangle* tri )
{
	std::vector<Triangle*>::iterator it = std::find(triangles.begin(), triangles.end(), tri);

	if ( it != triangles.end() )
		triangles.erase( it );
}

