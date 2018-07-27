#ifndef __SIM_LODDER_H
#define __SIM_LODDER_H

#include <sim_engine.h>

class CLodder
{
	// ----------------------------------------------------------------------//
	class Triangle;
	// ----------------------------------------------------------------------//

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

	// ----------------------------------------------------------------------//

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

	// ----------------------------------------------------------------------//

public:
	CLodder();
	virtual ~CLodder();

	// ----------------------------------------------------------------------//

	typedef std::vector<Vertex*>				TVertexList;
	typedef std::vector<Vertex*>::iterator		TVertexListIter;

	typedef std::vector<Triangle*>				TTriangleList;
	typedef std::vector<Triangle*>::iterator	TTriangleListIter;

protected:

	Vertex*				AddVertex( f32* src, s32 id );

	void				DeleteVertex( Vertex* v );
	void				DeleteTriangle( Triangle* tri );

	Vertex*				MinimumCostEdge();
	f32					ComputeEdgeCollapseCost( Vertex* u, Vertex* v);
	void				ComputeEdgeCostAtVertex( Vertex* v);

	void				Collapse( Vertex* u, Vertex* v );
	CVertexGroup*		CreateLOD( u32 depth );

protected:

	TVertexList			m_vertexList;
	TTriangleList		m_triangleList;
};

#endif // __SIM_LODDER_H
