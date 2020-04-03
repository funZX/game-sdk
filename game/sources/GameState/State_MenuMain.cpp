
#include <core/sim_state_machine.h>
#include <render/sim_canvas.h>
#include <render/sim_driver.h>
#include <render/sim_rect_2d.h>

#include "../Game.h"

#include "../World/World.h"

#include "State_MenuMain.h"
#include "State_Loading.h"
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
    //SIM_PRINT("\nCState_MenuMain::Update");
}

// ----------------------------------------------------------------------//
void CState_MenuMain::Render( CDriver *driver )
{
    //SIM_PRINT("\nCState_MenuMain::Render");
}
// ----------------------------------------------------------------------//
void CState_MenuMain::OnEnter()
{
    SIM_PRINT("\nCState_MenuMain::OnEnter");

    CCanvas* canvas = g.canvas;
    canvas->OnEvent.Connect(this, &CState_MenuMain::OnEvent);
}
// ----------------------------------------------------------------------//
void CState_MenuMain::OnExit()
{
    SIM_PRINT("\nCState_MenuMain::OnExit");

    CCanvas* canvas = g.canvas;
    canvas->OnEvent.Disconnect(this, &CState_MenuMain::OnEvent);
}
// ----------------------------------------------------------------------//
void CState_MenuMain::OnEvent(CCanvas* canvas, CCanvas::TEvent* ev, sigcxx::SLOT slot)
{
    if (ev->evType == CCanvas::EvType::Key && !ev->evKey.down)
    {
        if (ev->evKey.code == 78) // Key_N
        {
            std::vector<std::string> fsList = {
                "debug.7z",
                "world.7z",
                "strawberry.7z"
            };

            g.game->GoPop(SIM_NEW CState_Loading(fsList, SIM_NEW CState_Game()));
        }
    }
}
// ----------------------------------------------------------------------//