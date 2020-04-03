#include <core/io/sim_file_system.h>

#include <render/sim_canvas.h>
#include <render/sim_driver.h>

#include "../Game.h"

#include "State_AppInit.h"
#include "State_Loading.h"
#include "State_MenuMain.h"
// ----------------------------------------------------------------------//
CState_AppInit::CState_AppInit()
{

}
// ----------------------------------------------------------------------//
CState_AppInit::~CState_AppInit()
{
	SIM_PRINT("\n~CState_AppInit");
}
// ----------------------------------------------------------------------//
void CState_AppInit::ShowGui( CCanvas* canvas )
{

}
// ----------------------------------------------------------------------//
void CState_AppInit::Update( f32 dt, void *userData )
{
	SIM_PRINT("\nCState_AppInit::Update");

    std::vector<std::string> fsList = {
        "ui.7z",
    };

    g.game->GoPop(SIM_NEW CState_Loading(fsList, SIM_NEW CState_MenuMain()));
}
// ----------------------------------------------------------------------//
void CState_AppInit::Render( CDriver *driver )
{
	SIM_PRINT("\nCState_AppInit::Render");
}
// ----------------------------------------------------------------------//
void CState_AppInit::OnEnter()
{
	SIM_PRINT("\nCState_AppInit::OnEnter");
}
// ----------------------------------------------------------------------//
void CState_AppInit::OnExit()
{
	SIM_PRINT("\nCState_AppInit::OnExit");
}
// ----------------------------------------------------------------------//