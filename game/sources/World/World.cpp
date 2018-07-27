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

#include <vm/sim_script.h>
#include <vm/sim_squirrel.h>

#include "../Debug/Debug.h"
#include "../Options.h"
#include "../Game.h"
#include "World.h"

CWorld::CWorld()
{
	m_fs				= SIM_NEW CFileSystem( O.game->GetFsPath("world.7z") );
	m_physic			= SIM_NEW CPhysic();
	m_scene				= SIM_NEW CScene( "main scene" );

	m_isFocused			= false;
	m_isEnabled			= true;
	m_isVisible			= true;

	m_debug				= SIM_NEW CDebug( m_fs );
}

CWorld::~CWorld()
{
	SIM_SAFE_DELETE( m_debug );

	SIM_SAFE_DELETE( m_scene );
	SIM_SAFE_DELETE( m_physic );
	SIM_SAFE_DELETE( m_fs );
}

void CWorld::Update( f32 dt, void *userData )
{
	m_physic->Update( dt, userData );
	m_scene->Update( dt, userData );

	static CScript* script = m_fs->GetScript("dt/deltatime");

	script->Run();

	//O.game->GetVM()->DebuggerStart();
}

void CWorld::Render( CDriver *driver )
{
	if( !m_isVisible )
		return;

	static CSkyBox* skybox = m_fs->GetSkybox("sunny");
	skybox->Render( driver );

	m_scene->Render( driver );

	#if SIM_DEBUG
	m_debug->Render( driver );
	#endif
}

void CWorld::Render2D(CDriver *driver)
{
	#if SIM_DEBUG
	m_debug->Render2D(driver);
	#endif
}

void CWorld::AddActor( CActor* actor )
{
	m_scene->Add( actor );
}