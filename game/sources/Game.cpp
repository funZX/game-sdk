#include <core/io/sim_file_system.h>
#include <core/io/sim_file_stream.h>

#include <render/scene/sim_camera.h>

#include <render/sim_batch_2d.h>
#include <render/sim_canvas.h>
#include <render/sim_shader.h>
#include <render/sim_effect.h>
#include <render/sim_material.h>

#include <render/font/sim_font.h>
#include <render/font/sim_font_atlas.h>

#include <vm/sim_squirrel.h>

#include "GameState/State_AppInit.h"

#include "World/World.h"

#include "Game.h"

#include <sim_engine.h>

// ----------------------------------------------------------------------//
TGlobal		O = {0};
// ----------------------------------------------------------------------//

CGame::CGame( const std::string& fsDir )
	: CEngine()
{
	m_fsDir = fsDir;

	SIM_MEMSET( &m_downKeys, 0, sizeof( m_downKeys ) );

	O.game				= this;

	O.driver			= m_driver;
	O.canvas			= m_canvas;
	O.camera			= m_camera;
	O.material			= m_material;

	O.font.engine		= m_font;
	O.effect.engine		= m_effect;

	m_world				= SIM_NEW CWorld();
	m_world->SetVisible( false );
	m_world->SetEnabled( false );

	O.world				= m_world;

	GoNext( SIM_NEW CState_AppInit() );
}

CGame::~CGame()
{
	SIM_SAFE_DELETE( m_world );
}

// ----------------------------------------------------------------------//

const std::string CGame::GetFsPath( const std::string& fs)
{
	std::string location;

	location.append( m_fsDir );
	location.append( fs );

	return location;
}

// ----------------------------------------------------------------------//


void CGame::Update( f32 dt, void *userData )
{
	CEngine::Update( dt, userData );
}

// ----------------------------------------------------------------------//

void CGame::KeyPress( u8 key, bool isDown )
{
	m_downKeys[ key ] = isDown;

	std::cout << "KeyPress: " << key << ", " << isDown << std::endl;
}

// ----------------------------------------------------------------------//
