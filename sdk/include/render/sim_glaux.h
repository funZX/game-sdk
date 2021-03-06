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
#include <core/sim_core.h>
#include <render/sim_render.h>

namespace sim
{
namespace ren
{
class CDriver;
class CVertexSource;
class CVertexGroup;
class CShader;
class CMaterial;
class CRect2D;

// ----------------------------------------------------------------------//
struct Sphere
{
	s32					numSlices;
	f32					radius;

	CVertexGroup*		vertexGroup;
};

struct Cube
{
	f32					sideSize;

	CVertexGroup*		vertexGroup;
};

Sphere*	    gluNewSphere( s32 numSlices, f32 radius );
Sphere*	    gluDelSphere( Sphere *sphere );
void		gluRenderSphere( CDriver *driver, Sphere *sphere );

Cube*		gluNewCube( f32 sideSize );
Cube*		gluDelCube( Cube *cube );
void		gluRenderCube( CDriver *driver, Cube *cube );

void		gluTBN(	Vec3* TAN, 
					Vec3* BIN,
					Vec3 NOR,
					Vec3 A, Vec3 B, Vec3 C,
					Vec2 H, Vec2 K, Vec2 L );

void		gluProject( Vec3 *obj, Mat4 *modelViewMatrix, Mat4 *projectionMatrix, CRect2D *viewPort, Vec3 screen );
// ----------------------------------------------------------------------//
}; // namespace ren
}; // namespace sim

#endif // __SIM_GLAUX_H
