#include <core/io/sim_file_system.h>
#include <render/sim_material.h>

#include <render/sim_canvas.h>
#include <render/sim_driver.h>

#include <core/sim_state_machine.h>

#include <vm/sim_script.h>
#include <vm/sim_squirrel.h>

#include "../Game.h"

#include "../World/World.h"

#include "State_Game.h"

CState_Game::CState_Game()
{
	//O.game->GetVM()->DebuggerStart();

	static CScript* script = O.world->GetFs()->GetScript("dt/deltatime");
	script->Run();
}

CState_Game::~CState_Game()
{

}

void CState_Game::Update( f32 dt, void *userData )
{
	O.world->Update( dt, userData );
}

void CState_Game::Render2D( CDriver *driver )
{
	O.world->Render2D(driver);
}

void CState_Game::Render3D( CDriver *driver )
{
	O.world->Render( driver );
}