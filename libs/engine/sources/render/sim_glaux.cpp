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

#include <core/sim_core.h>
#include <render/sim_driver.h>
#include <render/sim_material.h>
#include <render/sim_shader.h>
#include <render/sim_vertex_group.h>
#include <render/sim_vertex_source.h>
#include <render/sim_rect_2d.h>
#include <render/sim_glaux.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//

TSphere*	gluNewSphere( s32 numSlices, f32 radius )
{
	TSphere *sphere			= SIM_NEW TSphere;
	sphere->vertexGroup		= SIM_NEW CVertexGroup();
	sphere->vertexGroup->m_vertexSource = SIM_NEW CVertexSource();

	sphere->numSlices		= numSlices;
	sphere->radius			= radius;

	s32 numParallels = numSlices >> 1;
	f32 angleStep = ( 2.0f * SIM_PI ) / ( ( f32 ) numSlices );

	s32 numIndices	= 6 * numParallels * numSlices;
	s32 numVertices = ( numParallels + 1 ) * ( numSlices + 1 );

	sphere->vertexGroup->m_vboData = SIM_NEW u16[ numIndices ];
	sphere->vertexGroup->m_vboSize = numIndices;

	sphere->vertexGroup->m_vertexSource->m_type			= CVertexSource::k_Type_Triangle;
	sphere->vertexGroup->m_vertexSource->m_vertexFormat	= CVertexSource::k_Vertex_Attribute_Format_Position | CVertexSource::k_Vertex_Attribute_Format_TexCoord_0 | CVertexSource::k_Vertex_Attribute_Format_Normal;
	sphere->vertexGroup->m_vertexSource->m_vertexStride	= CVertexSource::k_Vertex_Attribute_Offset_Position + CVertexSource::k_Vertex_Attribute_Offset_TexCoord_0 + CVertexSource::k_Vertex_Attribute_Offset_Normal;
	sphere->vertexGroup->m_vertexSource->m_vboSize		= numVertices;

	s32 vtxSize							= sphere->vertexGroup->m_vertexSource->m_vertexStride / sizeof( f32 );
	s32 newFloats 						= numVertices * vtxSize;
	s32 vboOff							= 0;

	sphere->vertexGroup->m_vertexSource->m_vboData	= SIM_NEW f32[ newFloats ];

	for( s32 i = 0; i < numParallels + 1; i++ )
	{
		f32 sx = radius * sinf ( angleStep * (f32)i );
		f32 sy = radius * cosf ( angleStep * (f32)i );
		f32 sz = sx;

		for( s32 j = 0; j < numSlices + 1; j++ )
		{
			s32		currIndex = ( i * (numSlices + 1) + j ) * vtxSize;
			f32*	curVertex = &sphere->vertexGroup->m_vertexSource->m_vboData[ currIndex ];

			// position
			curVertex[ 0 ] = sx * sinf ( angleStep * (f32)j );
			curVertex[ 1 ] = sy;
			curVertex[ 2 ] = sz * cosf ( angleStep * (f32)j );

			// texCoords
			curVertex[ 3 ] = (f32) j / (f32) numSlices;
			curVertex[ 4 ] = ( 1.0f - (f32) i ) / (f32) (numParallels - 1 );

			// normals
			curVertex[ 5 ] = curVertex[ 0 ] / radius;
			curVertex[ 6 ] = curVertex[ 1 ] / radius;
			curVertex[ 7 ] = curVertex[ 2 ] / radius;
		}
	}

	u16 *indexBuf = sphere->vertexGroup->m_vboData;
	for( s32 i = 0; i < numParallels ; i++ )
	{
		s32 i0 = i * ( numSlices + 1 );
		s32 i1 = ( i + 1 ) * ( numSlices + 1 );

		for( s32 j = 0; j < numSlices; j++ )
		{
			s32 j0 = i0 + j;
			s32 j1 = i1 + j;

			*indexBuf++ = j0;
			*indexBuf++ = j1;
			*indexBuf++ = j1 + 1;

			*indexBuf++ = j0;
			*indexBuf++ = j1 + 1;
			*indexBuf++ = j0 + 1;
		}
	}

	return sphere;
}

// ----------------------------------------------------------------------//

TSphere* gluDelSphere( TSphere *sphere )
{
	SIM_ASSERT( sphere != NULL );

	SIM_SAFE_DELETE_ARRAY( sphere->vertexGroup->m_vertexSource->m_vboData );
	SIM_SAFE_DELETE( sphere->vertexGroup->m_vertexSource );

	SIM_SAFE_DELETE_ARRAY( sphere->vertexGroup->m_vboData );
	SIM_SAFE_DELETE( sphere->vertexGroup );
	
	SIM_SAFE_DELETE( sphere );

	return NULL;
}

// ----------------------------------------------------------------------//

void gluRenderSphere( CDriver *driver, TSphere *sphere )
{
	SIM_ASSERT( sphere != NULL );
	SIM_ASSERT( driver != NULL );

	driver->Render( sphere->vertexGroup );
}

// ----------------------------------------------------------------------//

TCube* gluNewCube( f32 sideSize )
{
	TCube *cube								= SIM_NEW TCube;

	cube->vertexGroup						= SIM_NEW CVertexGroup();
	cube->vertexGroup->m_vertexSource		= SIM_NEW CVertexSource();

	cube->sideSize							= sideSize;

	// A cube with the primary vertices (note: 4, 5, 6, 7 is the front face)
	//       1-----------2
	//      /|          /|
	//     /           / |
	//    /  |        /  |
	//   5-----------6   |
	//   |   0- - - -|- -3
	//   |  /        |  /
	//   | /         | /
	//   |/          |/
	//   4-----------7

	// Duplicate vertices are so that we can specify different normals for vertices depending one which face is using the vertex (parenthesis denote the "base" vertex for duplicates)
	static const f32 cubeVerts[] = {
		-0.5f, -0.5f, -0.5f,  //  0     Back
		-0.5f, -0.5f,  0.5f,  //  1
		 0.5f, -0.5f,  0.5f,  //  2
		 0.5f, -0.5f, -0.5f,  //  3

		-0.5f,  0.5f, -0.5f,  //  4     Front
		-0.5f,  0.5f,  0.5f,  //  5
		 0.5f,  0.5f,  0.5f,  //  6
		 0.5f,  0.5f, -0.5f,  //  7

		-0.5f,  0.5f, -0.5f,  //  8 (4) Left Side
		-0.5f,  0.5f,  0.5f,  //  9 (5)
		-0.5f, -0.5f,  0.5f,  // 10 (1)
		-0.5f, -0.5f, -0.5f,  // 11 (0)

		 0.5f,  0.5f, -0.5f,  // 12 (7) Right Side
		 0.5f,  0.5f,  0.5f,  // 13 (6)
		 0.5f, -0.5f,  0.5f,  // 14 (2)
		 0.5f, -0.5f, -0.5f,  // 15 (3)

		-0.5f,  0.5f, -0.5f,  // 16 (4) Bottom
		-0.5f, -0.5f, -0.5f,  // 17 (0)
		 0.5f, -0.5f, -0.5f,  // 18 (3)
		 0.5f,  0.5f, -0.5f,  // 19 (7)

		-0.5f,  0.5f,  0.5f,  // 20 (5) Top
		-0.5f, -0.5f,  0.5f,  // 21 (1)
		 0.5f, -0.5f,  0.5f,  // 22 (2)
		 0.5f,  0.5f,  0.5f,  // 23 (6)
	};

	// Normals depend upon which face is using the vertex
	static const GLfloat cubeNormals[] = {
		 0.0f, -1.0f,  0.0f,  // 0      Back
		 0.0f, -1.0f,  0.0f,  // 1
		 0.0f, -1.0f,  0.0f,  // 2
		 0.0f, -1.0f,  0.0f,  // 3
		 
		 0.0f,  1.0f,  0.0f,  // 4      Front
		 0.0f,  1.0f,  0.0f,  // 5
		 0.0f,  1.0f,  0.0f,  // 6
		 0.0f,  1.0f,  0.0f,  // 7

		-1.0f,  0.0f,  0.0f,  // 8  (4) Left Side
		-1.0f,  0.0f,  0.0f,  // 9  (5)
		-1.0f,  0.0f,  0.0f,  // 10 (1)
		-1.0f,  0.0f,  0.0f,  // 11 (0)

		 1.0f,  0.0f,  0.0f,  // 12 (7) Right Side
		 1.0f,  0.0f,  0.0f,  // 13 (6)
		 1.0f,  0.0f,  0.0f,  // 14 (2)
		 1.0f,  0.0f,  0.0f,  // 15 (3)
		 
		 0.0f,  0.0f, -1.0f,  // 16 (4) Bottom
		 0.0f,  0.0f, -1.0f,  // 17 (0)
		 0.0f,  0.0f, -1.0f,  // 18 (3)
		 0.0f,  0.0f, -1.0f,  // 19 (7)
		 
		 0.0f,  0.0f,  1.0f,  // 20 (5) Top
		 0.0f,  0.0f,  1.0f,  // 21 (1)
		 0.0f,  0.0f,  1.0f,  // 22 (2)
		 0.0f,  0.0f,  1.0f   // 23 (6)
	};

	static const f32 cubeTex[] = {
		0.0, 0.0,  // 0      Back
		0.0, 1.0,  // 1
		1.0, 1.0,  // 2
		1.0, 0.0,  // 3

		0.0, 0.0,  // 4      Front
		0.0, 1.0,  // 5
		1.0, 1.0,  // 6
		1.0, 0.0,  // 7

		0.0, 0.0,  // 8  (4) Left Side
		0.0, 1.0,  // 9  (5)
		1.0, 1.0,  // 10 (1)
		1.0, 0.0,  // 11 (0)

		0.0, 0.0,  // 12 (7) Right Side
		0.0, 1.0,  // 13 (6)
		1.0, 1.0,  // 14 (2)
		1.0, 0.0,  // 15 (3)

		0.0, 0.0,  // 16 (4) Bottom
		0.0, 1.0,  // 17 (0)
		1.0, 1.0,  // 18 (3)
		1.0, 0.0,  // 19 (7)

		0.0, 0.0,  // 20 (5) Top
		0.0, 1.0,  // 21 (1)
		1.0, 1.0,  // 22 (2)
		1.0, 0.0,  // 23 (6)
	};

	// Reuses vertices to specify triangles
	static const GLushort cubeIndices[] = {

		//     1------2
		//     |    / |
		//     |  /   |
		//     |/     |
		//     0------3
		0, 2, 1,     // Back 1
		0, 3, 2,     // Back 2

		//     5------6
		//     |    / |
		//     |  /   |
		//     |/     |
		//     4------7
		4, 5, 6,     // Front 1
		4, 6, 7,     // Front 2


		//  (5)9------8(1)
		//     |    / |
		//     |  /   |
		//     |/     |
		// (4)10------11(0)
		10, 9, 8,    // Left side 1
		10, 8, 11,   // Left side 2


		// (6)13------12(2)
		//     |    / |
		//     |  /   |
		//     |/     |
		// (7)14------15(3)
		14, 12, 13,  // Right side 1
		14, 15, 12,  // Right side 2

		// (0)17------18(3)
		//     |    / |
		//     |  /   |
		//     |/     |
		// (4)16------19(7)
		16, 18, 17,  // Bottom 1
		16, 19, 18,  // Bottom 2

		// (1)21------22(2)
		//     |    / |
		//     |  /   |
		//     |/     |
		// (5)20------23(6)
		20, 21, 22,  // Top 1
		20, 22, 23,  // Top 2

	};

	s32 numVertices			= 24;
	s32 numIndices			= 36;

	cube->vertexGroup->m_vboData		= SIM_NEW u16[ numIndices ];
	cube->vertexGroup->m_vboSize	= numIndices;

	cube->vertexGroup->m_vertexSource->m_type			= CVertexSource::k_Type_Triangle;
	cube->vertexGroup->m_vertexSource->m_vertexFormat	= CVertexSource::k_Vertex_Attribute_Format_Position | CVertexSource::k_Vertex_Attribute_Format_TexCoord_0 | CVertexSource::k_Vertex_Attribute_Format_Normal;
	cube->vertexGroup->m_vertexSource->m_vertexStride	= CVertexSource::k_Vertex_Attribute_Offset_Position + CVertexSource::k_Vertex_Attribute_Offset_TexCoord_0 + CVertexSource::k_Vertex_Attribute_Offset_Normal;
	cube->vertexGroup->m_vertexSource->m_vboSize	= numVertices;

	s32 vtxSize							= cube->vertexGroup->m_vertexSource->m_vertexStride / sizeof( f32 );
	s32 newFloats 						= numVertices * vtxSize;
	s32 vboOff							= 0;

	cube->vertexGroup->m_vertexSource->m_vboData		  = SIM_NEW f32[ newFloats ];

	for( s32 i = 0; i < numVertices; i++ )
	{
		s32		currIndex = i * vtxSize;
		f32*	curVertex = &cube->vertexGroup->m_vertexSource->m_vboData[ currIndex ];

		s32		i2	=  2 * i;
		s32		i20 = i2 + 0;
		s32		i21 = i2 + 1;
	
		s32		i3	= i2 + i;
		s32		i30 = i3 + 0;
		s32		i31 = i3 + 1; 
		s32		i32 = i3 + 2; 

		// position
		curVertex[ 0 ] = cubeVerts[ i30 ] * sideSize;
		curVertex[ 1 ] = cubeVerts[ i31 ] * sideSize;
		curVertex[ 2 ] = cubeVerts[ i32 ] * sideSize;

		// texCoords
		curVertex[ 3 ] = cubeTex[ i20 ];
		curVertex[ 4 ] = cubeTex[ i21 ];

		// normals
		curVertex[ 5 ] = cubeNormals[ i30 ];
		curVertex[ 6 ] = cubeNormals[ i31 ];
		curVertex[ 7 ] = cubeNormals[ i32 ];
	}

	SIM_MEMCPY( cube->vertexGroup->m_vboData, cubeIndices, numIndices * sizeof( u16 ) );

	return cube;
}

// ----------------------------------------------------------------------//

TCube* gluDelCube( TCube *cube )
{
	SIM_ASSERT( cube != NULL );

	SIM_SAFE_DELETE_ARRAY( cube->vertexGroup->m_vertexSource->m_vboData );
	SIM_SAFE_DELETE( cube->vertexGroup->m_vertexSource );

	SIM_SAFE_DELETE_ARRAY( cube->vertexGroup->m_vboData );
	SIM_SAFE_DELETE( cube->vertexGroup );
	
	SIM_SAFE_DELETE( cube );

	return NULL;	
}

// ----------------------------------------------------------------------//

void gluRenderCube( CDriver *driver, TCube *cube )
{
	SIM_ASSERT( cube != NULL );
	SIM_ASSERT( driver != NULL );

	driver->Render( cube->vertexGroup );
}

// ----------------------------------------------------------------------//

void gluProject( TVec3 *obj, TMatrix4 *modelViewMatrix, TMatrix4 *projectionMatrix, CRect2D *viewPort, TVec3 *screen )
{
	TVec3 in, out;
	
	in.x = obj->x;
	in.y = obj->y;
	in.x = obj->z;

	Matrix4Transform( modelViewMatrix, &in, &out );
	Matrix4TransformHomogen( projectionMatrix, &out, &in );

	screen->x = viewPort->Left() + ( 1.0f + in.x ) * viewPort->Right()  * 0.5f;
	screen->y = viewPort->Top()  + ( 1.0f + in.y ) * viewPort->Bottom() * 0.5f;

	screen->z = ( 1.0f + in.z ) * 0.5f;
}

// ----------------------------------------------------------------------//

void gluTBN(	TVec3* TAN, 
			TVec3* BIN,
			const TVec3* NOR,
			const TVec3* A, const TVec3* B, const TVec3* C,
			const TVec2* H, const TVec2* K, const TVec2* L )
{
	TVec3 D, E, T, U;
	TVec2 F, G;

	Vec3Diff( &D, B, A );													// D = B-A
	Vec3Diff( &E, C, A );													// E = C-A

	Vec2Diff( &F, K, H );													// F = K-H
	Vec2Diff( &G, L, H );													// G = L-H

	// Expressed as:
	//			D = F.s * T + F.t * U
	//			E = G.s * T + G.t * U
	// 
	// <=>
	//			| D.x D.y D.z |								|  F.s   F.t | | T.x T.y T.z |
	//			|             |			=					|            | |             |
	//			| E.x E.y E.z |								|  G.s   G.t | | U.x U.y U.z |
	// <=>
	//			| T.x T.y T.z |			          1         |  G.t  -F.t | | D.x D.y D.z |
	//			|             |			= ----------------- |            | |             |
	//			| U.x U.y U.z |			  F.s G.t - F.t G.s | -G.s   F.s | | E.x E.y E.z |

	f32 c = 1.0f / ( F.s * G.t - F.t * G.s );

	T.x = c * (  G.t  * D.x  - F.t * E.x );
	T.y = c * (  G.t  * D.y  - F.t * E.y );
	T.z = c * (  G.t  * D.z  - F.t * E.z );

	U.x = c * ( -G.s  * D.x  + F.s * E.x );
	U.y = c * ( -G.s  * D.y  + F.s * E.y );
	U.z = c * ( -G.s  * D.z  + F.s * E.z );

	// transforming from object space to tangent space:
	//			| T.x U.x NOR.x |
	//			| T.y U.y NOR.y |
	//			| T.z U.z NOR.z |
	
	//			TAN = T - (NOR�T) * NOR
	//			BIN = U - (NOR�U) * NOR - (TAN�U) * TAN
	
	TVec3 n, t;

	Vec3Copy( &n, NOR );
	Vec3Scale( &n, &n, Vec3Dot( NOR, &T ) );
	Vec3Diff( TAN, &T, &n );
	Vec3Normalize( TAN );

	Vec3Copy( &n, NOR );
	Vec3Scale( &n, &n, Vec3Dot( NOR, &U ) );
	Vec3Copy( &t, TAN );
	Vec3Scale( &t, &t, Vec3Dot( TAN, &U ) );
	Vec3Diff( BIN, &U, &n );
	Vec3Diff( BIN, BIN, &t );
	Vec3Normalize( BIN );
}
// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim