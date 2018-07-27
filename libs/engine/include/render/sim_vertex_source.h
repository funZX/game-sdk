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

#ifndef __SIM_VERTEX_SOURCE_H
#define __SIM_VERTEX_SOURCE_H

#include <core/sim_interfaces.h>

#include <math/sim_matrix4.h>
#include <math/sim_vec2.h>
#include <math/sim_vec3.h>

#include <render/sim_render.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//
struct TSphere;
struct TCube;

class CEffect;
class CMaterial;
class CDriver;
class CRect2D;
class CBatch2D;
class CBatch3D;
class CFontChar;

class CVertexSource : public IEngineItem
{
public:
	CVertexSource();
	CVertexSource( const std::string& name );
	virtual ~CVertexSource();

	// ------------------------------------------------------------------//
	typedef enum
	{
        k_Vertex_Attribute_Format_Position			=   ( 1 <<  0 ),
        k_Vertex_Attribute_Format_TexCoord_0		=   ( 1 <<  1 ),
        k_Vertex_Attribute_Format_TexCoord_1		=   ( 1 <<  2 ),
        k_Vertex_Attribute_Format_TexCoord_2		=   ( 1 <<  3 ),
        k_Vertex_Attribute_Format_TexCoord_3		=   ( 1 <<  4 ),
        k_Vertex_Attribute_Format_Color				=   ( 1 <<  5 ),
        k_Vertex_Attribute_Format_Normal			=   ( 1 <<  6 ),
        k_Vertex_Attribute_Format_Tangent			=   ( 1 <<  7 ),
        k_Vertex_Attribute_Format_Binormal			=   ( 1 <<  8 ),
		k_Vertex_Attribute_Format_Bone				=	( 1 <<  9),
		k_Vertex_Attribute_Format_Weight			=	( 1 << 10),

        k_Vertex_Attribute_Format_Count				=   12,

	} K_VERTEX_ATTRIBUTE_FORMAT;
	// ------------------------------------------------------------------//
	typedef enum
	{
        k_Vertex_Attribute_Size_Position		=   ( 3 ),
        k_Vertex_Attribute_Size_TexCoord_0		=   ( 2 ),
        k_Vertex_Attribute_Size_TexCoord_1		=   ( 2 ),
        k_Vertex_Attribute_Size_TexCoord_2		=   ( 2 ),
        k_Vertex_Attribute_Size_TexCoord_3		=   ( 2 ),
        k_Vertex_Attribute_Size_Color			=   ( 4 ),
        k_Vertex_Attribute_Size_Normal			=   ( 3 ),
        k_Vertex_Attribute_Size_Tangent			=   ( 3 ),
        k_Vertex_Attribute_Size_Binormal		=   ( 3 ),
		k_Vertex_Attribute_Size_Bone			=	( 2 ),
		k_Vertex_Attribute_Size_Weight			=	( 2 ),

	} K_VERTEX_ATTRIBUTE_SIZE;
	// ------------------------------------------------------------------//
	typedef enum
	{
        k_Vertex_Attribute_Offset_Position		=   ( 3 * sizeof( float ) ),
        k_Vertex_Attribute_Offset_TexCoord_0    =   ( 2 * sizeof( float ) ),
        k_Vertex_Attribute_Offset_TexCoord_1    =   ( 2 * sizeof( float ) ),
        k_Vertex_Attribute_Offset_TexCoord_2    =   ( 2 * sizeof( float ) ),
        k_Vertex_Attribute_Offset_TexCoord_3    =   ( 2 * sizeof( float ) ),
        k_Vertex_Attribute_Offset_Color			=   ( 4 * sizeof( char ) ),
        k_Vertex_Attribute_Offset_Normal		=   ( 3 * sizeof( float ) ),
        k_Vertex_Attribute_Offset_Tangent		=   ( 3 * sizeof( float ) ),
        k_Vertex_Attribute_Offset_Binormal		=   ( 3 * sizeof( float ) ),
		k_Vertex_Attribute_Offset_Bone			=   ( 2 * sizeof( char ) ),
		k_Vertex_Attribute_Offset_Weight		=   ( 2 * sizeof( float ) ),

	} K_VERTEX_ATTRIBUTE_OFFSET;
	// ------------------------------------------------------------------//
	
	typedef enum
	{
        k_Vertex_Attribute_Type_Position		=   ( GL_FLOAT ),
        k_Vertex_Attribute_Type_TexCoord_0		=   ( GL_FLOAT ),
        k_Vertex_Attribute_Type_TexCoord_1		=   ( GL_FLOAT ),
        k_Vertex_Attribute_Type_TexCoord_2		=   ( GL_FLOAT ),
        k_Vertex_Attribute_Type_TexCoord_3		=   ( GL_FLOAT ),
        k_Vertex_Attribute_Type_Color			=   ( GL_UNSIGNED_BYTE ),
        k_Vertex_Attribute_Type_Normal			=   ( GL_FLOAT ),
        k_Vertex_Attribute_Type_Tangent			=   ( GL_FLOAT ),
        k_Vertex_Attribute_Type_Binormal		=   ( GL_FLOAT ),
        k_Vertex_Attribute_Type_Bone			=   ( GL_UNSIGNED_BYTE ),
        k_Vertex_Attribute_Type_Weight			=   ( GL_FLOAT ),


	} K_VERTEX_ATTRIBUTE_TYPE;

	// ------------------------------------------------------------------//
	
	typedef enum
	{
		k_Type_Line,
		k_Type_LineStrip,
		k_Type_Points,
		k_Type_Triangle,
		k_Type_TriangleStrip,

	} K_TYPE;

	// ------------------------------------------------------------------//

	inline f32*							GetVboData() const		{ return m_vboData; }
	inline u32							GetVboSize() const		{ return m_vboSize; }
	inline u32							GetVertexStride() const	{ return m_vertexStride; }
	inline u32							GetVertexFormat() const	{ return m_vertexFormat; }
	inline K_TYPE						GetType() const			{ return m_type; }
	inline u32							GetID()					{ return m_iD; }

	u32									Generate();
	// ------------------------------------------------------------------//

protected:
	// ------------------------------------------------------------------//
	u32									m_iD;

public:
	// ------------------------------------------------------------------//
	u32									m_vertexStride;
	u32									m_vertexFormat;

	u32									m_vboSize;
	f32*								m_vboData;

	K_TYPE								m_type;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_VERTEX_SOURCE_H
