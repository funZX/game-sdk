#include <render/sim_vertex_source.h>
#include <render/sim_vertex_group.h>

#include <render/scene/sim_light.h>
#include <render/scene/sim_camera.h>
#include <render/scene/sim_actor.h>
#include <render/scene/sim_scene.h>

#include <render/sim_material.h>
#include <render/sim_mesh.h>

#include "data.h"
#include "lodder.h"
#include "import.h"
#include "export.h"

CData::CData()
{
	m_material	= NULL;
	m_mesh		= NULL;
	m_actor		= NULL;
	m_light		= NULL;
	m_camera	= NULL;
	m_scene		= NULL;

	m_info		= NULL;
	m_lodder	= NULL;
}

CData::~CData()
{
	SIM_SAFE_DELETE( m_lodder );
	SIM_SAFE_DELETE( m_info );

	SIM_SAFE_DELETE( m_material );
	SIM_SAFE_DELETE( m_mesh );
	SIM_SAFE_DELETE( m_actor );
	SIM_SAFE_DELETE( m_light );
	SIM_SAFE_DELETE( m_camera );
	SIM_SAFE_DELETE( m_scene );
}

// ----------------------------------------------------------------------//

void CData::Load( const CImport* i )
{

}

// ----------------------------------------------------------------------//

void CData::Save( const CExport* e )
{

}

// ----------------------------------------------------------------------//

