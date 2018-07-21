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

bool CImport::ParseScene( CData* data )
{
	json_t* jsonRoot = m_jsonStream->GetRoot();

	return true;
}

// ----------------------------------------------------------------------//