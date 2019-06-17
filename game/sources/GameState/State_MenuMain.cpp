
#include <core/sim_state_machine.h>
#include <render/sim_canvas.h>
#include <render/sim_driver.h>
#include <render/sim_rect_2d.h>

#include "../Game.h"

#include "../World/World.h"

#include "State_MenuMain.h"
#include "State_Game.h"
// ----------------------------------------------------------------------//
CState_MenuMain::CState_MenuMain()
{

}
// ----------------------------------------------------------------------//
CState_MenuMain::~CState_MenuMain()
{
    SIM_PRINT("\n~CState_MenuMain");
}
// ----------------------------------------------------------------------//
void CState_MenuMain::ShowGui( CCanvas* canvas )
{

}
// ----------------------------------------------------------------------//
void CState_MenuMain::Update( f32 dt, void *userData )
{
	O.world->Update( dt, userData );
	O.game->GoNext( SIM_NEW CState_Game() );
}

// ----------------------------------------------------------------------//
void CState_MenuMain::Render( CDriver *driver )
{
	O.world->Render(driver);
}
// ----------------------------------------------------------------------//
void CState_MenuMain::OnEnter()
{

}
// ----------------------------------------------------------------------//
void CState_MenuMain::OnExit()
{

}
// ----------------------------------------------------------------------//