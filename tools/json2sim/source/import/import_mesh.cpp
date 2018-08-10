#include <core/io/sim_json_stream.h>

#include <render/sim_vertex_source.h>
#include <render/sim_vertex_group.h>

#include <render/sim_mesh.h>

#include "data.h"
#include "import.h"
#include "lodder.h"

// ----------------------------------------------------------------------//
CLodder lodder;
// ----------------------------------------------------------------------//

void GetBounds( mat::TVec3* box, mat::TVec3* center, f32* radius, const CVertexSource* vertexSource )
{
	mat::TVec3 a, b;
	mat::TVec3 ubound;
	mat::TVec3 lbound;
	
	Vec3Set( box, 0, 0, 0 );
	Vec3Set( &ubound, -99999.0f, -99999.0f, -99999.0f );
	Vec3Set( &lbound,  99999.0f,  99999.0f,  99999.0f );
	
	for( u16 i = 0; i < vertexSource->GetVboSize(); i++ )
	{
		int ii		= i * Value(vertexSource->GetVertexStride()) / sizeof( f32 );
		f32* buffer = vertexSource->m_vboData;

		// min
		if( buffer[ ii + 0 ] < lbound.x )
			lbound.x = buffer[ ii + 0 ];
			
		if( buffer[ ii + 1 ] < lbound.y )
			lbound.y = buffer[ ii + 1 ];
			
		if( buffer[ ii + 2 ] < lbound.z )
			lbound.z = buffer[ ii + 2 ];
					
		// max
		if( buffer[ ii + 0 ] > ubound.x )
			ubound.x = buffer[ ii + 0 ];
						
		if( buffer[ ii + 1 ] > ubound.y )
			ubound.y = buffer[ ii + 1 ];
							
		if( buffer[ ii + 2 ] > ubound.z )
			ubound.z = buffer[ ii + 2 ];
	}

	// center	
	mat::Vec3Add( center, &ubound, &lbound );
	mat::Vec3Scale( center, 0.5f );
	
	// DimX
	mat::Vec3Set( &a, ubound.x, ubound.y, ubound.z );
	mat::Vec3Set( &b, lbound.x, ubound.y, ubound.z );
	
	box->x = mat::Vec3Dist( &a, &b ) * 0.5f;
	
	
	// DimY
	mat::Vec3Set( &a, ubound.x, ubound.y, ubound.z );
	mat::Vec3Set( &b, ubound.x, lbound.y, ubound.z );
	
	box->y = mat::Vec3Dist( &a, &b ) * 0.5f;
	
	// DimZ
	mat::Vec3Set( &a, ubound.x, ubound.y, ubound.z );
	mat::Vec3Set( &b, ubound.x, ubound.y, lbound.z );
	
	box->z = mat::Vec3Dist( &a, &b ) * 0.5f;
	
	*radius = SIM_MAX( box->x, SIM_MAX( box->y, box->z ) );
}

// ----------------------------------------------------------------------//

struct TVertex
{
	f32 x, y, z;
	f32 u, v;
	f32 nx, ny, nz;
	f32 r, g, b, a;
};

struct TFace
{
	u16 i0, i1, i2;
};

// ----------------------------------------------------------------------//

u16 AddVertex( std::vector<TVertex>& vertices, const TVertex* v)
{
	for ( u16 k = 0; k < vertices.size(); k++ )
	{
		TVertex* crt = &vertices[ k ];

		if ( crt->x  == v->x  && crt->y  == v->y   && crt->z  == v->z)/* &&
			 crt->u  == v->u  && crt->v  == v->v   &&
			 crt->nx == v->nx && crt->ny == v->ny  && crt->nz == v->nz &&
			 crt->r  == v->r  && crt->g  == v->g   && crt->b  == v->b  && crt->a == v->a )*/

			return k;
	}

	vertices.push_back( *v );

	return vertices.size() - 1;
}

// ----------------------------------------------------------------------//

void GetBucket( json_t* jsonRoot, std::vector<TVertex>&	vertices )
{
	json_t* jsonSource		= json_object_get( jsonRoot, "source" );
	json_t* jsonVertices	= json_object_get( jsonSource, "vertices" );
	json_t* jsonTexcoord	= json_object_get( jsonSource, "texcoord" );
	json_t* jsonNormals		= json_object_get( jsonSource, "normals" );
	json_t* jsonColors		= json_object_get( jsonSource, "vcolors" );

	json_t* jsonTriangles	= json_object_get( jsonRoot, "triangles" );

	u16 vboSize			= json_array_size( jsonTriangles );

	for ( u16 tri = 0; tri < vboSize / 3; tri++ )
	{
		TVertex v0, v1, v2;

		u16 i0 = 3 * tri, i1 = i0 + 1, i2 = i1 + 1;

		u16 u0 = (u16) json_integer_value( json_array_get( jsonTriangles, i0 ) );
		u16 u1 = (u16) json_integer_value( json_array_get( jsonTriangles, i1 ) );
		u16 u2 = (u16) json_integer_value( json_array_get( jsonTriangles, i2 ) );




		v0.x = (f32) json_real_value( json_array_get( jsonVertices, ( u0 * 3 ) + 0 ) );
		v0.y = (f32) json_real_value( json_array_get( jsonVertices, ( u0 * 3 ) + 1 ) );
		v0.z = (f32) json_real_value( json_array_get( jsonVertices, ( u0 * 3 ) + 2 ) );

		v0.u = (f32) json_real_value( json_array_get( jsonTexcoord, ( i0 * 2 ) + 0  ) );
		v0.v = (f32) json_real_value( json_array_get( jsonTexcoord, ( i0 * 2 ) + 1  ) );

		v0.nx = (f32) json_real_value( json_array_get( jsonNormals, ( i0 * 3 ) + 0 ) );
		v0.ny = (f32) json_real_value( json_array_get( jsonNormals, ( i0 * 3 ) + 1 ) );
		v0.nz = (f32) json_real_value( json_array_get( jsonNormals, ( i0 * 3 ) + 2 ) );

		v0.r = (f32) json_real_value( json_array_get( jsonColors, ( i0 * 4 ) + 0 ) );
		v0.g = (f32) json_real_value( json_array_get( jsonColors, ( i0 * 4 ) + 1 ) );
		v0.b = (f32) json_real_value( json_array_get( jsonColors, ( i0 * 4 ) + 2 ) );
		v0.a = (f32) json_real_value( json_array_get( jsonColors, ( i0 * 4 ) + 3 ) );




		v1.x = (f32) json_real_value( json_array_get( jsonVertices, ( u1 * 3 ) + 0 ) );
		v1.y = (f32) json_real_value( json_array_get( jsonVertices, ( u1 * 3 ) + 1 ) );
		v1.z = (f32) json_real_value( json_array_get( jsonVertices, ( u1 * 3 ) + 2 ) );

		v1.u = (f32) json_real_value( json_array_get( jsonTexcoord, ( i1 * 2 ) + 0 ) );
		v1.v = (f32) json_real_value( json_array_get( jsonTexcoord, ( i1 * 2 ) + 1 ) );

		v1.nx = (f32) json_real_value( json_array_get( jsonNormals, ( i1 * 3 ) + 0 ) );
		v1.ny = (f32) json_real_value( json_array_get( jsonNormals, ( i1 * 3 ) + 1 ) );
		v1.nz = (f32) json_real_value( json_array_get( jsonNormals, ( i1 * 3 ) + 2 ) );

		v1.r = (f32) json_real_value( json_array_get( jsonColors, ( i1 * 4 ) + 0 ) );
		v1.g = (f32) json_real_value( json_array_get( jsonColors, ( i1 * 4 ) + 1 ) );
		v1.b = (f32) json_real_value( json_array_get( jsonColors, ( i1 * 4 ) + 2 ) );
		v1.a = (f32) json_real_value( json_array_get( jsonColors, ( i1 * 4 ) + 3 ) );




		v2.x = (f32) json_real_value( json_array_get( jsonVertices, ( u2 * 3 ) + 0 ) );
		v2.y = (f32) json_real_value( json_array_get( jsonVertices, ( u2 * 3 ) + 1 ) );
		v2.z = (f32) json_real_value( json_array_get( jsonVertices, ( u2 * 3 ) + 2 ) );

		v2.u = (f32) json_real_value( json_array_get( jsonTexcoord, ( i2 * 2 ) + 0 ) );
		v2.v = (f32) json_real_value( json_array_get( jsonTexcoord, ( i2 * 2 ) + 1 ) );

		v2.nx = (f32) json_real_value( json_array_get( jsonNormals, ( i2 * 3 ) + 0 ) );
		v2.ny = (f32) json_real_value( json_array_get( jsonNormals, ( i2 * 3 ) + 1 ) );
		v2.nz = (f32) json_real_value( json_array_get( jsonNormals, ( i2 * 3 ) + 2 ) );

		v2.r = (f32) json_real_value( json_array_get( jsonColors, ( i2 * 4 ) + 0 ) );
		v2.g = (f32) json_real_value( json_array_get( jsonColors, ( i2 * 4 ) + 1 ) );
		v2.b = (f32) json_real_value( json_array_get( jsonColors, ( i2 * 4 ) + 2 ) );
		v2.a = (f32) json_real_value( json_array_get( jsonColors, ( i2 * 4 ) + 3 ) );




		TFace face;

		face.i0 = AddVertex( vertices, &v0 );
		face.i1 = AddVertex( vertices, &v1 );
		face.i2 = AddVertex( vertices, &v2 );

		json_array_set( jsonTriangles, i0, json_integer( face.i0 ) );
		json_array_set( jsonTriangles, i1, json_integer( face.i1 ) );
		json_array_set( jsonTriangles, i2, json_integer( face.i2 ) );
	}
}

// ----------------------------------------------------------------------//

bool CImport::ParseMesh( CData* data )
{
	data->m_info			= SIM_NEW CData::TInfo();

	json_t* jsonRoot		= m_jsonStream->GetRoot();
	json_t* jsonName		= json_object_get( jsonRoot, "name" );
	json_t* jsonMaterial	= json_object_get( jsonRoot, "material" );

	if ( !jsonRoot || !jsonName || !jsonMaterial )
		return false;

	data->m_info->material = json_string_value( jsonMaterial );

	std::vector<TVertex>	vertices ;
	GetBucket( jsonRoot, vertices );

	bool export_tbn			= ( 1 == json_integer_value( json_object_get( jsonRoot, "tbn" ) ) );
	bool tbn_texcoord		= false;
	bool tbn_normals		= false;

	//source
	json_t* jsonSource		= json_object_get( jsonRoot,   "source" );
	json_t* jsonTexcoord	= json_object_get( jsonSource, "texcoord" );
	json_t* jsonNormals		= json_object_get( jsonSource, "normals" );
	json_t* jsonColors		= json_object_get( jsonSource, "vcolors" );
	json_t* jsonBones		= json_object_get( jsonSource, "bones" );
	json_t* jsonWeights		= json_object_get( jsonSource, "weights" );

	CVertexSource::AttributeFormat format	= CVertexSource::AttributeFormat::Position;
	CVertexSource::AttributeStride stride	= CVertexSource::AttributeStride::Position;

	if ( json_array_size( jsonTexcoord ) > 0 )
	{
		format	= format | CVertexSource::AttributeFormat::TexCoord_0;
		stride	= stride + CVertexSource::AttributeStride::TexCoord_0;

		tbn_texcoord = true;
	}

	if ( json_array_size( jsonNormals ) > 0 )
	{
		format = format | CVertexSource::AttributeFormat::Normal;
		stride = stride + CVertexSource::AttributeStride::Normal;

		tbn_normals = true;
	}

	export_tbn = ( export_tbn && tbn_texcoord && tbn_normals );

	if ( export_tbn )
	{
		format = format | CVertexSource::AttributeFormat::Tangent;
		stride = stride + CVertexSource::AttributeStride::Tangent;

		format = format | CVertexSource::AttributeFormat::Binormal;
		stride = stride + CVertexSource::AttributeStride::Binormal;
	}

	if ( json_array_size( jsonColors ) > 0 )
	{
		format  = format | CVertexSource::AttributeFormat::Color;
		stride	= stride + CVertexSource::AttributeStride::Color;
	}

	if ( json_array_size( jsonBones ) > 0 && json_array_size( jsonWeights ) > 0 )
	{
		format = format | CVertexSource::AttributeFormat::Bone;
		stride = stride + CVertexSource::AttributeStride::Bone;

		format = format | CVertexSource::AttributeFormat::Weight;
		stride = stride + CVertexSource::AttributeStride::Weight;
	}

	data->m_mesh = SIM_NEW CMesh( json_string_value( jsonName ) );

	data->m_mesh->m_material = data->m_info->material;

	CVertexSource* vertexSource = SIM_NEW CVertexSource();
	data->m_mesh->m_vertexSource = vertexSource;

	vertexSource->m_vertexStride = stride;
	vertexSource->m_vertexFormat = format;

	u32 vertexSize = Value(stride) / sizeof( f32 );
	vertexSource->m_vboSize = vertices.size();
	vertexSource->m_vboData = SIM_NEW f32[ vertexSize * vertexSource->m_vboSize ];

	for ( u32 k = 0; k < vertexSource->m_vboSize; k++ )
	{
		u32 vOff = 0, vi = 3 * k;

		TVertex* vertex = &vertices[ k ];
		f32* curVertex = &vertexSource->m_vboData[ k * vertexSize ];

		curVertex[ vOff + 0 ] = vertex->x;
		curVertex[ vOff + 1 ] = vertex->y;
		curVertex[ vOff + 2 ] = vertex->z;

		vOff += Value(CVertexSource::AttributeSize::Position);

		if (CVertexSource::AttributeFormat::None != ( format & CVertexSource::AttributeFormat::TexCoord_0 ) )
		{
			curVertex[ vOff + 0 ] = vertex->u;
			curVertex[ vOff + 1 ] = vertex->v;

			vOff += Value(CVertexSource::AttributeSize::TexCoord_0);
		}

		if (CVertexSource::AttributeFormat::None != ( format & CVertexSource::AttributeFormat::TexCoord_1 ) )
			vOff += Value(CVertexSource::AttributeSize::TexCoord_1);

		if (CVertexSource::AttributeFormat::None != ( format & CVertexSource::AttributeFormat::TexCoord_2 ) )
			vOff += Value(CVertexSource::AttributeSize::TexCoord_2);

		if (CVertexSource::AttributeFormat::None != ( format & CVertexSource::AttributeFormat::TexCoord_3 ) )
			vOff += Value(CVertexSource::AttributeSize::TexCoord_3);

		if (CVertexSource::AttributeFormat::None != ( format & CVertexSource::AttributeFormat::Normal ) )
		{
			curVertex[ vOff + 0 ] = vertex->nx;
			curVertex[ vOff + 1 ] = vertex->ny;
			curVertex[ vOff + 2 ] = vertex->nz;
		
			vOff += Value(CVertexSource::AttributeSize::Normal);
		}

		if (CVertexSource::AttributeFormat::None != ( format & CVertexSource::AttributeFormat::Tangent ) )
			vOff += Value(CVertexSource::AttributeSize::Tangent);

		if (CVertexSource::AttributeFormat::None != ( format & CVertexSource::AttributeFormat::Binormal ) )
			vOff += Value(CVertexSource::AttributeSize::Binormal);

		if (CVertexSource::AttributeFormat::None != ( format & CVertexSource::AttributeFormat::Color ) )
		{
			mat::TVec4  v4;
			col::TColor col;
			mat::Vec4Set( &v4, vertex->r, vertex->g, vertex->b, vertex->a );
			mat::Vec4Scale( &v4, &v4, 1.0f / 255.0f );
			col::Vec4ToColor( &v4, &col );

			curVertex[ vOff + 0 ] = *((f32*)&col);

			vOff = vOff + Value(CVertexSource::AttributeSize::Color);
		}

		if (CVertexSource::AttributeFormat::None != ( format & CVertexSource::AttributeFormat::Bone ) )
			vOff += Value(CVertexSource::AttributeSize::Bone);

		if (CVertexSource::AttributeFormat::None != ( format & CVertexSource::AttributeFormat::Weight ) )
			vOff += Value(CVertexSource::AttributeSize::Bone);
	}

	// group
	json_t* jsonTriangles		= json_object_get( jsonRoot, "triangles" );

	CVertexGroup* group			= SIM_NEW CVertexGroup();

	data->m_mesh->m_vertexGroup = group;
	
	group->m_vboSize	= json_array_size( jsonTriangles );
	group->m_vboData	= SIM_NEW u16[ group->m_vboSize ];

	for ( u16 tri = 0; tri < group->m_vboSize / 3; tri++ )
	{
		mat::TVec3		A, B, C, NA, NB, NC;
		mat::TVec2		H, K, L;

		u16 i0 = 3 * tri, 
			i1 = i0 + 1, 
			i2 = i1 + 1;

		group->m_vboData[ i0 ] = (u16) json_integer_value( json_array_get( jsonTriangles, i0 ) );
		group->m_vboData[ i1 ] = (u16) json_integer_value( json_array_get( jsonTriangles, i1 ) );
		group->m_vboData[ i2 ] = (u16) json_integer_value( json_array_get( jsonTriangles, i2 ) );

		u16 vi0 = group->m_vboData[ i0 ] * vertexSize;
		f32* v0	= &vertexSource->m_vboData[ vi0 ];
		A.x = v0[ 0 ]; 
		A.y = v0[ 1 ]; 
		A.z = v0[ 2 ];

		u16 vi1 = group->m_vboData[ i1 ] * vertexSize;
		f32* v1	= &vertexSource->m_vboData[ vi1 ];
		B.x = v1[ 0 ]; 
		B.y = v1[ 1 ];
		B.z = v1[ 2 ];

		u16 vi2 = group->m_vboData[ i2 ] * vertexSize;
		f32* v2	= &vertexSource->m_vboData[ vi2 ];
		C.x = v2[ 0 ];
		C.y = v2[ 1 ]; 
		C.z = v2[ 2 ];

		u32 vOff = Value(CVertexSource::AttributeSize::Position);

		if (CVertexSource::AttributeFormat::None != ( format & CVertexSource::AttributeFormat::TexCoord_0 ) )
		{
			H.x = v0[ vOff + 0 ];
			H.y = v0[ vOff + 1 ];

			K.x = v1[ vOff + 0 ];
			K.y = v1[ vOff + 1 ];

			L.x = v2[ vOff + 0 ];
			L.y = v2[ vOff + 1 ];

			vOff = vOff + Value(CVertexSource::AttributeSize::TexCoord_0);
		}

		if (CVertexSource::AttributeFormat::None != ( format & CVertexSource::AttributeFormat::TexCoord_1 ) )
			vOff = vOff + Value(CVertexSource::AttributeSize::TexCoord_1);

		if (CVertexSource::AttributeFormat::None != ( format & CVertexSource::AttributeFormat::TexCoord_2 ) )
			vOff = vOff + Value(CVertexSource::AttributeSize::TexCoord_2);

		if (CVertexSource::AttributeFormat::None != ( format & CVertexSource::AttributeFormat::TexCoord_3 ) )
			vOff = vOff + Value(CVertexSource::AttributeSize::TexCoord_2);

		if (CVertexSource::AttributeFormat::None != ( format & CVertexSource::AttributeFormat::Normal ) )
		{
			f32* n0	= &vertexSource->m_vboData[ vi0 + vOff ];
			NA.x = n0[ 0 ]; 
			NA.y = n0[ 1 ]; 
			NA.z = n0[ 2 ];

			f32* n1	= &vertexSource->m_vboData[ vi1 + vOff ];
			NB.x = n1[ 0 ]; 
			NB.y = n1[ 1 ]; 
			NB.z = n1[ 2 ];

			f32* n2	= &vertexSource->m_vboData[ vi2 + vOff ];
			NC.x = n2[ 0 ]; 
			NC.y = n2[ 1 ]; 
			NC.z = n2[ 2 ];

			vOff = vOff + Value(CVertexSource::AttributeSize::Normal);
		}

		if ( export_tbn )
		{
			mat::TVec3 TA, BA;
			gluTBN( &TA, &BA, &NA, &A, &B, &C, &H, &K, &L );

			mat::TVec3 TB, BB;
			gluTBN( &TB, &BB, &NB, &B, &C, &A, &K, &L, &H );

			mat::TVec3 TC, BC;
			gluTBN( &TC, &BC, &NC, &C, &A, &B, &L, &H, &K );


			// write tangents
			u16 ti0 = vi0 + vOff;
			vertexSource->m_vboData[ ti0 + 0 ] = TA.x;
			vertexSource->m_vboData[ ti0 + 1 ] = TA.y;
			vertexSource->m_vboData[ ti0 + 2 ] = TA.z;

			u16 ti1 = vi1 + vOff;
			vertexSource->m_vboData[ ti1 + 0 ] = TB.x;
			vertexSource->m_vboData[ ti1 + 1 ] = TB.y;
			vertexSource->m_vboData[ ti1 + 2 ] = TB.z;

			u16 ti2 = vi2 + vOff;
			vertexSource->m_vboData[ ti2 + 0 ] = TC.x;
			vertexSource->m_vboData[ ti2 + 1 ] = TC.y;
			vertexSource->m_vboData[ ti2 + 2 ] = TC.z;

			vOff = vOff + Value(CVertexSource::AttributeSize::Tangent);
			// write binormals
			u16 bi0 = vi0 + vOff;
			vertexSource->m_vboData[ bi0 + 0 ] = BA.x;
			vertexSource->m_vboData[ bi0 + 1 ] = BA.y;
			vertexSource->m_vboData[ bi0 + 2 ] = BA.z;

			u16 bi1 = vi1 + vOff;
			vertexSource->m_vboData[ bi1 + 0 ] = BB.x;
			vertexSource->m_vboData[ bi1 + 1 ] = BB.y;
			vertexSource->m_vboData[ bi1 + 2 ] = BB.z;

			u16 bi2 = vi2 + vOff;
			vertexSource->m_vboData[ bi2 + 0 ] = BC.x;
			vertexSource->m_vboData[ bi2 + 1 ] = BC.y;
			vertexSource->m_vboData[ bi2 + 2 ] = BC.z;

			vOff = vOff + Value(CVertexSource::AttributeSize::Binormal);
		}
	}

	mat::TVec3 box, center;
	f32 radius;
	GetBounds( &box, &center, &radius, vertexSource );

	data->m_mesh->SetBox( &box );
	data->m_mesh->SetCenter( &center );
	data->m_mesh->SetRadius( radius );

	return true;
}

// ----------------------------------------------------------------------//