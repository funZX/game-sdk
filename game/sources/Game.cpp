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

#include "Options.h"
#include "Game.h"

#include <engine.h>

// ----------------------------------------------------------------------//
TGlobal		O = {0};
// ----------------------------------------------------------------------//

CGame::CGame( const std::string& fsDir )
	: CEngine()
{
	SIM_MEMSET( &m_downKeys, 0, sizeof( m_downKeys ) );

	m_fsDir				= fsDir;

	O.game				= this;
	O.fsDir				= m_fsDir.c_str();
	O.driver			= m_driver;
	O.effect			= m_effect;
	O.canvas			= m_canvas;
	O.camera			= m_camera;
	
	O.font.engine		= m_font;

	m_world				= SIM_NEW CWorld();
	m_world->SetVisible( false );

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
