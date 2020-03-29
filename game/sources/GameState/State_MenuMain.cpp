
#include <core/sim_state_machine.h>
#include <render/sim_canvas.h>
#include <render/sim_driver.h>
#include <render/sim_rect_2d.h>

#include "../Game.h"

#include "../World/World.h"

#include "State_Loading.h"
#include "State_Game.h"
#include "State_MenuMain.h"
// ----------------------------------------------------------------------//
CState_MenuMain::CState_MenuMain()
{
    CCanvas* canvas = g.canvas;
    canvas->OnMouseDown.Connect(this, &CState_MenuMain::MouseDown);
    canvas->OnMouseUp.Connect(this, &CState_MenuMain::MouseUp);
    canvas->OnMouseMove.Connect(this, &CState_MenuMain::MouseMove);
    canvas->OnKeyDown.Connect(this, &CState_MenuMain::KeyDown);
    canvas->OnKeyUp.Connect(this, &CState_MenuMain::KeyUp);
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

}

// ----------------------------------------------------------------------//
void CState_MenuMain::Render( CDriver *driver )
{

}
// ----------------------------------------------------------------------//
void CState_MenuMain::OnEnter( bool isPushed )
{

}
// ----------------------------------------------------------------------//
void CState_MenuMain::OnExit( bool isPoped )
{

}
// ----------------------------------------------------------------------//
void CState_MenuMain::MouseDown(CCanvas* canvas, int button, sigcxx::SLOT slot)
{
    //SIM_PRINT("\nMouseDown: %d", button);
}
// ----------------------------------------------------------------------//
void CState_MenuMain::MouseUp(CCanvas* canvas, int button, sigcxx::SLOT slot)
{
    //SIM_PRINT("\nMouseUp: %d", button);
}
// ----------------------------------------------------------------------//
void CState_MenuMain::MouseMove(CCanvas* canvas, f32 x, f32 y, sigcxx::SLOT slot)
{
    //SIM_PRINT("\nMouseMove: (%.2f, %.2f)", x, y);
}
// ----------------------------------------------------------------------//
void CState_MenuMain::KeyDown(CCanvas* canvas, int Key, bool KeyShift, bool KeyCtrl, bool KeyAlt, sigcxx::SLOT slot)
{
    //SIM_PRINT("\nKeyDown: (%d, %d, %d, %d)", Key, KeyShift, KeyCtrl, KeyAlt);
}
// ----------------------------------------------------------------------//
void CState_MenuMain::KeyUp(CCanvas* canvas, int Key, bool KeyShift, bool KeyCtrl, bool KeyAlt, sigcxx::SLOT slot)
{

    switch (Key)
    {
    case 78: // 'n'
    {
        std::vector<std::string> fsList = {
            "debug.7z",
            "world.7z",
            "ui.7z",
            "strawberry.7z"
        };

        g.game->GoNext(SIM_NEW CState_Loading( fsList, SIM_NEW CState_Game() ));
    }
    break;
    }
    //SIM_PRINT("\nKeyUp: (%d, %d, %d, %d)", Key, KeyShift, KeyCtrl, KeyAlt);
}
// ----------------------------------------------------------------------//