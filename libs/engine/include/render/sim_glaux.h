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

#ifndef __SIM_GLAUX_H
#define __SIM_GLAUX_H

#include <core/sim_interfaces.h>
#include <math/sim_vec2.h>
#include <math/sim_vec3.h>
#include <math/sim_matrix4.h>
#include <render/sim_render.h>

namespace sim
{
namespace rnr
{
class CDriver;
class CVertexSource;
class CVertexGroup;
class CShader;
class CMaterial;
class CRect2D;

// ----------------------------------------------------------------------//
struct TSphere
{
	s32					numSlices;
	f32					radius;

	CVertexGroup*		vertexGroup;
};

struct TCube
{
	f32					sideSize;

	CVertexGroup*		vertexGroup;
};

TSphere*	gluNewSphere( s32 numSlices, f32 radius );
TSphere*	gluDelSphere( TSphere *sphere );
void		gluRenderSphere( CDriver *driver, TSphere *sphere );

TCube*		gluNewCube( f32 sideSize );
TCube*		gluDelCube( TCube *cube );
void		gluRenderCube( CDriver *driver, TCube *cube );

void		gluTBN(	mat::TVec3* TAN, 
					mat::TVec3* BIN,
					const mat::TVec3* NOR,
					const mat::TVec3* A, const mat::TVec3* B, const mat::TVec3* C,
					const mat::TVec2* H, const mat::TVec2* K, const mat::TVec2* L );

void		gluProject( mat::TVec3 *obj, mat::TMatrix4 *modelViewMatrix, mat::TMatrix4 *projectionMatrix, CRect2D *viewPort, mat::TVec3 *screen );
// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim

#endif // __SIM_GLAUX_H
