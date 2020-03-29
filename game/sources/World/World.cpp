#include <core/io/sim_file_system.h>

#include <render/scene/sim_scene_node.h>
#include <render/scene/sim_actor.h>
#include <render/scene/sim_scene.h>
#include <render/scene/sim_sky_box.h>
#include <render/scene/sim_camera.h>

#include <render/sim_driver.h>
#include <render/sim_texture.h>
#include <render/sim_material.h>

#include <physic/sim_physic.h>

#include "../Debug/Debug.h"
#include "../Game.h"
#include "World.h"

CWorld::CWorld( io::CFileSystem* fs )
{
	m_physic			= SIM_NEW CPhysic();
    m_scene				= SIM_NEW CScene(fs);

    m_skybox = fs->GetSkybox("sunny");

	m_isEnabled			= true;
	m_isVisible			= true;
}

CWorld::~CWorld()
{
	SIM_SAFE_DELETE( m_scene );
	SIM_SAFE_DELETE( m_physic );
}

void CWorld::Update( f32 dt, void *userData )
{
	if (!m_isEnabled)
		return;

	m_physic->Update( dt, userData );
	m_scene->Update( dt, userData );
}

void CWorld::Render( CDriver *driver )
{
	if (!m_isEnabled)
		return;

	if( !m_isVisible )
		return;

	if ( m_skybox )
		m_skybox->Render( driver );

	m_scene->Render( driver );
}


void CWorld::AddActor( CActor* actor )
{
	m_scene->Add( actor );
}