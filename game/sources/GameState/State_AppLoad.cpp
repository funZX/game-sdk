#include <render/scene/sim_actor.h>
#include <render/sim_font.h>
#include <render/sim_canvas.h>
#include <render/sim_driver.h>

#include "../Game.h"

#include "../World/World.h"

#include "State_MenuMain.h"
#include "State_AppLoad.h"

// ----------------------------------------------------------------------//
CState_AppLoad::CState_AppLoad()
{

}
// ----------------------------------------------------------------------//
CState_AppLoad::~CState_AppLoad()
{
	SIM_PRINT("\n~CState_AppLoad");
}
// ----------------------------------------------------------------------//
void CState_AppLoad::ShowGui( CCanvas* canvas )
{
}
// ----------------------------------------------------------------------//
void CState_AppLoad::Update( f32 dt, void *userData )
{
	g.game->GoNext( SIM_NEW CState_MenuMain() );
}

// ----------------------------------------------------------------------//
void CState_AppLoad::Render( CDriver *driver )
{

}
// ----------------------------------------------------------------------//
void CState_AppLoad::OnEnter( bool isPushed )
{
}

// ----------------------------------------------------------------------//
void CState_AppLoad::OnExit( bool isPoped )
{
}
// ----------------------------------------------------------------------//