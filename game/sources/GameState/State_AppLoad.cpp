#include <core/io/sim_file_system.h>

#include <render/scene/sim_actor.h>
#include <render/font/sim_font.h>
#include <render/sim_canvas.h>
#include <render/sim_driver.h>

#include "../Game.h"

#include "../World/World.h"

#include "State_MenuMain.h"
#include "State_AppLoad.h"

CState_AppLoad::CState_AppLoad()
{
}

CState_AppLoad::~CState_AppLoad()
{
	SIM_PRINT("~CState_AppLoad");
}

void CState_AppLoad::Update( f32 dt, void *userData )
{
	if ( !O.world->Load() )
		O.game->GoNext(SIM_NEW CState_MenuMain());
}

void CState_AppLoad::Render2D( CDriver *driver )
{
	O.game->Print( driver,
		0, 
		O.canvas->GetHeight() - 2 * O.font.simarian->GetHeight(), 
		O.world->GetFs()->GetLoadMessage() );
}

void CState_AppLoad::Render3D( CDriver *driver )
{

}

void CState_AppLoad::OnEnter()
{
	O.world->GetFs()->Open();
};

void CState_AppLoad::OnExit()
{
	O.world->GetFs()->Close();
};