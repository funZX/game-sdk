#ifndef __SIM_POLYGON_H
#define __SIM_POLYGON_H

#include <core/sim_list.h>
#include <math/sim_vec2.h>

// ----------------------------------------------------------------------//
namespace sim
{
// ----------------------------------------------------------------------//
	namespace rnr { class CDriver; class CEffect; class CMaterial; };
// ----------------------------------------------------------------------//

namespace core
{
// ----------------------------------------------------------------------//

class CPolygon;
typedef sim::stl::CList<CPolygon*>	TPolygonList;

class CPolygon : public sim::stl::CList<mat::TVec2>
{
public:
	CPolygon();
	virtual ~CPolygon();

public:
	bool			Intersects( CPolygon* polygon );
	CPolygon*		Intersects( TPolygonList* polygonList );

	bool			IsInside( const f32 x, const f32 y );
	bool			IsInside( const mat::TVec2* v );

	void			Translate( const sim::f32 x, const f32 y );
	void			Scale( const f32 kx, const f32 ky );

	void			AddVertex( const f32 x, const f32 y );
	void			AddVertex( const mat::TVec2* v );

	const f32		GetMinX();
	const f32		GetMinY();

	const f32		GetMaxX();
	const f32		GetMaxY();

	void			Render( rnr::CDriver *driver, rnr::CMaterial *mtl );
};

// ----------------------------------------------------------------------//
} // namespace stl;
} // namespace sim;

#endif // __SIM_POLYGON_H

