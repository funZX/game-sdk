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

#ifndef __SIM_VERTEX_SOURCE_H
#define __SIM_VERTEX_SOURCE_H

#include <core/sim_core.h>
#include <core/sim_interfaces.h>

#include <render/sim_render.h>

namespace sim
{
namespace rnr
{
// ----------------------------------------------------------------------//
struct Sphere;
struct Cube;

class CEffect;
class CMaterial;
class CDriver;
class CRect2D;
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
        ScreenPos,
		WorldPos,
		TexCoord_0,
		TexCoord_1,
		TexCoord_2,
		TexCoord_3,
		Color,
		Normal,
		Tangent,
		Binormal,
		WeightArraySize,
		WeightArray,
	};

	// ------------------------------------------------------------------//
	enum class AttributeFormat: u32
	{
		None			= 0,

        ScreenPos       =   ( 1 <<  0 ),
        WorldPos		=   ( 1 <<  1 ),
        TexCoord_0		=   ( 1 <<  2 ),
        TexCoord_1		=   ( 1 <<  3 ),
        TexCoord_2		=   ( 1 <<  4 ),
        TexCoord_3		=   ( 1 <<  5 ),
        Color			=   ( 1 <<  6 ),
        Normal			=   ( 1 <<  7 ),
        Tangent			=   ( 1 <<  8 ),
        Binormal		=   ( 1 <<  9 ),
		WeightArraySize =	( 1 << 10 ),
		WeightArray		=	( 1 << 11 ),
	};
	// ------------------------------------------------------------------//
	enum { k_Vertex_Bones_Influences_Max = 16 };

	enum class AttributeSize : u32
	{
        ScreenPos       =   ( 2 ),
        WorldPos		=   ( 3 ),
        TexCoord_0		=   ( 2 ),
		TexCoord_1		=	( 2 ),
		TexCoord_2		=	( 2 ),
		TexCoord_3		=	( 2 ),
        Color			=   ( 4 ),
        Normal			=   ( 3 ),
        Tangent			=   ( 3 ),
        Binormal		=   ( 3 ),
		WeightArraySize =   ( 1 ),
		WeightArray		=	( k_Vertex_Bones_Influences_Max ),
	};
	// ------------------------------------------------------------------//
	enum class AttributeStride : u32
	{
        ScreenPos       =   ( 2 * sizeof( float ) ),
        WorldPos		=   ( 3 * sizeof( float ) ),
        TexCoord_0		=   ( 2 * sizeof( float ) ),
        TexCoord_1		=   ( 2 * sizeof( float ) ),
        TexCoord_2		=   ( 2 * sizeof( float ) ),
        TexCoord_3		=   ( 2 * sizeof( float ) ),
        Color			=   ( 4 * sizeof( char ) ),
        Normal			=   ( 3 * sizeof( float ) ),
        Tangent			=   ( 3 * sizeof( float ) ),
        Binormal		=   ( 3 * sizeof( float ) ),
		WeightArraySize =	( 1 * sizeof( GL_UNSIGNED_BYTE ) ),
		WeightArray		=   ( k_Vertex_Bones_Influences_Max * sizeof( float ) ),
	};
	// ------------------------------------------------------------------//	
	enum class AttributeType : u32
	{
        ScreenPos       =   ( GL_FLOAT ),
        WorldPos		=   ( GL_FLOAT ),
        TexCoord_0		=   ( GL_FLOAT ),
        TexCoord_1		=   ( GL_FLOAT ),
        TexCoord_2		=   ( GL_FLOAT ),
        TexCoord_3		=   ( GL_FLOAT ),
        Color			=   ( GL_UNSIGNED_BYTE ),
        Normal			=   ( GL_FLOAT ),
        Tangent			=   ( GL_FLOAT ),
        Binormal		=   ( GL_FLOAT ),
		WeightArraySize =	( GL_UNSIGNED_BYTE ),
		WeightArray		=   ( GL_FLOAT ),
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

    virtual bool				Load(io::CMemStream* ms);
    virtual bool				Save(io::CMemStream* ms);
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
		Value(a1) | Value(a2)
		);
}
// ----------------------------------------------------------------------//
inline CVertexSource::AttributeFormat operator & (CVertexSource::AttributeFormat a1, 
	CVertexSource::AttributeFormat a2)
{
	return static_cast<CVertexSource::AttributeFormat> (
		Value(a1) & Value(a2)
		);
}
// ----------------------------------------------------------------------//
inline CVertexSource::AttributeFormat operator ^ (CVertexSource::AttributeFormat a1, 
	CVertexSource::AttributeFormat a2)
{
	return static_cast<CVertexSource::AttributeFormat> (
		Value(a1) ^ Value(a2)
		);
}
// ----------------------------------------------------------------------//
inline CVertexSource::AttributeStride operator + (CVertexSource::AttributeStride a1,
	CVertexSource::AttributeStride a2)
{
	return static_cast<CVertexSource::AttributeStride> (
		Value(a1) + Value(a2)
		);
}
// ----------------------------------------------------------------------//
}; // namespace rnr
}; // namespace sim
#endif // __SIM_VERTEX_SOURCE_H
