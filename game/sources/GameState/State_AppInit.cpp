#include <core/io/sim_file_system.h>

#include <render/sim_canvas.h>
#include <render/sim_driver.h>

#include "../Game.h"

#include "State_AppLoad.h"
#include "State_AppInit.h"
// ----------------------------------------------------------------------//
CState_AppInit::CState_AppInit()
{

}
// ----------------------------------------------------------------------//
CState_AppInit::~CState_AppInit()
{
	SIM_PRINT("~CState_AppInit");
}
// ----------------------------------------------------------------------//
void CState_AppInit::Update( f32 dt, void *userData )
{
	O.game->GoNext(SIM_NEW CState_AppLoad());
}
// ----------------------------------------------------------------------//
void CState_AppInit::Render2D( CDriver *driver )
{

}
// ----------------------------------------------------------------------//
void CState_AppInit::Render3D( CDriver *driver )
{

}
// ----------------------------------------------------------------------//
void CState_AppInit::OnEnter()
{
}
// ----------------------------------------------------------------------//
void CState_AppInit::OnExit()
{

}
// ----------------------------------------------------------------------//