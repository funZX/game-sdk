#include <core/io/sim_json_stream.h>

#include <render/sim_vertex_source.h>
#include <render/sim_vertex_group.h>

#include <render/scene/sim_light.h>
#include <render/scene/sim_camera.h>
#include <render/scene/sim_actor.h>
#include <render/scene/sim_scene.h>

#include <render/sim_material.h>
#include <render/sim_mesh.h>

#include "data.h"
#include "import.h"

// ----------------------------------------------------------------------//

CImport::CImport( const std::string& file,CData::K_DATA_TYPE type )
{
	m_type			= type;
	m_jsonStream	= SIM_NEW CJsonStream( file );
}

// ----------------------------------------------------------------------//

CImport::~CImport()
{
	SIM_SAFE_DELETE( m_jsonStream );
}

// ----------------------------------------------------------------------//

bool CImport::Parse( CData* data )
{
	switch ( m_type )
	{
	case CData::k_Data_Type_Material:
		return ParseMaterial( data );

	case CData::k_Data_Type_Mesh:
		return ParseMesh( data );

	case CData::k_Data_Type_MeshLOD:
		return ParseMeshLOD( data );

	case CData::k_Data_Type_Actor:
		return ParseActor( data );

	case CData::k_Data_Type_Light:
		return ParseLight( data );

	case CData::k_Data_Type_Camera:
		return ParseCamera( data );

	case CData::k_Data_Type_Scene:
		return ParseScene( data );
	}

	return false;
}

// ----------------------------------------------------------------------//
