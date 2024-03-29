#include <core/io/sim_file_system.h>
#include <core/io/sim_file_stream.h>
#include <core/sim_state_machine.h>

#include <render/scene/sim_camera.h>

#include <render/sim_canvas.h>
#include <render/sim_shader.h>
#include <render/sim_effect.h>
#include <render/sim_material.h>
#include <render/sim_font.h>

#include <vm/sim_squirrel.h>

#include "GameState/State_AppInit.h"

#include "World/World.h"

#include "Game.h"

#include <sim_engine.h>

// ----------------------------------------------------------------------//
TGlobal::~TGlobal()
{
    IState::TFsList fsList = {
        "debug.7z",
        "ui.7z",
        "world.7z",
        "strawberry.7z"
    };

    auto fnDtor = UNLOAD_FNDTOR(fsList);
    fnDtor();
}
// ----------------------------------------------------------------------//
TGlobal		g = { 0 };
// ----------------------------------------------------------------------//

CGame::CGame( const std::string& fsDir, IState* initState)
	: CEngine( initState )
{
	m_fsDir = fsDir;

	g.game				= this;

	g.driver			= m_driver;
	g.canvas			= m_canvas;
	g.camera			= &m_crtCamera;
	g.material			= m_material;

	g.font				= m_font;
	g.effect			= m_effect;
}

CGame::~CGame()
{

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
