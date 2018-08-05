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

#include <core/sim_core.h>
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
	enum class AttributeIndex : u32
	{
		Position,
		TexCoord_0,
		TexCoord_1,
		TexCoord_2,
		TexCoord_3,
		Color,
		Normal,
		Tangent,
		Binormal,
		Bone,
		Weight,
	};

	// ------------------------------------------------------------------//
	enum class AttributeFormat: u32
	{
		None			= 0,

        Position		=   ( 1 <<  0 ),
        TexCoord_0		=   ( 1 <<  1 ),
        TexCoord_1		=   ( 1 <<  2 ),
        TexCoord_2		=   ( 1 <<  3 ),
        TexCoord_3		=   ( 1 <<  4 ),
        Color			=   ( 1 <<  5 ),
        Normal			=   ( 1 <<  6 ),
        Tangent			=   ( 1 <<  7 ),
        Binormal		=   ( 1 <<  8 ),
		Bone			=	( 1 <<  9),
		Weight			=	( 1 << 10),
	};
	// ------------------------------------------------------------------//
	enum class AttributeSize : u32
	{
        Position		=   ( 3 ),
        TexCoord_0		=   ( 2 ),
        TexCoord_1		=   ( 2 ),
        TexCoord_2		=   ( 2 ),
        TexCoord_3		=   ( 2 ),
        Color			=   ( 4 ),
        Normal			=   ( 3 ),
        Tangent			=   ( 3 ),
        Binormal		=   ( 3 ),
		Bone			=	( 2 ),
		Weight			=	( 2 ),

	};
	// ------------------------------------------------------------------//
	enum class AttributeStride : u32
	{
        Position		=   ( 3 * sizeof( float ) ),
        TexCoord_0		=   ( 2 * sizeof( float ) ),
        TexCoord_1		=   ( 2 * sizeof( float ) ),
        TexCoord_2		=   ( 2 * sizeof( float ) ),
        TexCoord_3		=   ( 2 * sizeof( float ) ),
        Color			=   ( 4 * sizeof( char ) ),
        Normal			=   ( 3 * sizeof( float ) ),
        Tangent			=   ( 3 * sizeof( float ) ),
        Binormal		=   ( 3 * sizeof( float ) ),
		Bone			=   ( 2 * sizeof( char ) ),
		Weight			=   ( 2 * sizeof( float ) ),
	};
	// ------------------------------------------------------------------//	
	enum class AttributeType : u32
	{
        Position		=   ( GL_FLOAT ),
        TexCoord_0		=   ( GL_FLOAT ),
        TexCoord_1		=   ( GL_FLOAT ),
        TexCoord_2		=   ( GL_FLOAT ),
        TexCoord_3		=   ( GL_FLOAT ),
        Color			=   ( GL_UNSIGNED_BYTE ),
        Normal			=   ( GL_FLOAT ),
        Tangent			=   ( GL_FLOAT ),
        Binormal		=   ( GL_FLOAT ),
        Bone			=   ( GL_UNSIGNED_BYTE ),
        Weight			=   ( GL_FLOAT ),
	};

	enum { k_Vertex_Attributes_Count = 12 };
	// ------------------------------------------------------------------//
	
	enum class Type : u32
	{
		Line,
		LineStrip,
		Points,
		Triangle,
		TriangleStrip,
	};

	// ------------------------------------------------------------------//

	inline f32*					GetVboData() const		{ return m_vboData; }
	inline u32					GetVboSize() const		{ return m_vboSize; }
	inline AttributeStride		GetVertexStride() const { return m_vertexStride; }
	inline AttributeFormat		GetVertexFormat() const	{ return m_vertexFormat; }
	inline Type					GetType() const			{ return m_type; }
	inline u32					GetID()					{ return m_iD; }

	u32							Generate();
	// ------------------------------------------------------------------//

protected:
	// ------------------------------------------------------------------//
	u32							m_iD;

public:
	// ------------------------------------------------------------------//
	AttributeFormat				m_vertexFormat;
	AttributeStride				m_vertexStride;

	u32							m_vboSize;
	f32*						m_vboData;

	Type						m_type;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
inline CVertexSource::AttributeFormat operator | (CVertexSource::AttributeFormat a1,
	CVertexSource::AttributeFormat a2)
{
	return static_cast<CVertexSource::AttributeFormat> (
		EnumValue(a1) | EnumValue(a2)
		);
}
// ----------------------------------------------------------------------//
inline CVertexSource::AttributeFormat operator & (CVertexSource::AttributeFormat a1, 
	CVertexSource::AttributeFormat a2)
{
	return static_cast<CVertexSource::AttributeFormat> (
		EnumValue(a1) & EnumValue(a2)
		);
}
// ----------------------------------------------------------------------//
inline CVertexSource::AttributeFormat operator ^ (CVertexSource::AttributeFormat a1, 
	CVertexSource::AttributeFormat a2)
{
	return static_cast<CVertexSource::AttributeFormat> (
		EnumValue(a1) ^ EnumValue(a2)
		);
}
// ----------------------------------------------------------------------//
inline CVertexSource::AttributeStride operator + (CVertexSource::AttributeStride a1,
	CVertexSource::AttributeStride a2)
{
	return static_cast<CVertexSource::AttributeStride> (
		EnumValue(a1) + EnumValue(a2)
		);
}
// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_VERTEX_SOURCE_H
