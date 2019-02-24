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
