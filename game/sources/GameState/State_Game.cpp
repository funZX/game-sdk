
#include <render/scene/sim_light.h>
#include <render/scene/sim_camera.h>

#include <render/sim_drawable.h>


#include <render/sim_font.h>
#include <render/sim_canvas.h>
#include <render/sim_texture.h>
#include <render/sim_material.h>
#include <render/sim_shader.h>
#include <render/sim_effect.h>
#include <render/sim_mesh.h>

#include <core/io/sim_file_system.h>
#include <core/sim_list.h>
#include <core/sim_binary_tree.h>
#include <core/sim_balance_tree.h>

#include <render/sim_material.h>

#include <render/sim_canvas.h>
#include <render/sim_driver.h>

#include <core/sim_state_machine.h>

#include <vm/sim_script.h>
#include <vm/sim_squirrel.h>

#include "../Game.h"
#include "../Debug/Debug.h"

#include "../World/World.h"

#include "State_Game.h"
#include "State_Loading.h"
#include "State_MenuMain.h"
// ----------------------------------------------------------------------//
CState_Game::CState_Game( IState::TFnDtor fnDtor ) : IState(fnDtor)
{
	m_world     = nullptr;
	m_debug     = nullptr;
    m_drawable  = nullptr;

	/*
	CBinaryTree<u32, CScript> bt;
	bt.Insert(0, *script);
	CBalanceTree<u32, CScript> br;
	br.Insert(0, *script);
	*/
}
// ----------------------------------------------------------------------//
CState_Game::~CState_Game()
{
    SIM_PRINT("\n~CState_Game");
}
// ----------------------------------------------------------------------//
void CState_Game::Update( f32 dt, void *userData )
{	
    //SIM_PRINT("\nCState_Game::Update");

    m_drawable->Update(dt, userData);
    m_world->Update( dt, userData );
    m_debug->Update(dt, userData);
}

// ----------------------------------------------------------------------//
void CState_Game::Render( CDriver *driver )
{
    //SIM_PRINT("\nCState_Game::Render");
    m_drawable->Draw(driver);

	m_world->Render(driver);
    m_debug->Render(driver);
}

// ----------------------------------------------------------------------//
void CState_Game::OnDrawableDraw(CDriver* driver, sigcxx::SLOT slot)
{
	m_world->Render(driver);
    m_debug->Render(driver);
}
// ----------------------------------------------------------------------//
void CState_Game::ShowGui(CCanvas* canvas)
{
    m_drawable->SetEffect( canvas->GetMaterial()->GetEffect() );
    m_drawable->Render( g.driver );
}
// ----------------------------------------------------------------------//
void CState_Game::OnEnter()
{
    SIM_PRINT("\nCState_Game::OnEnter");

    m_world = SIM_NEW CWorld(g.GetFs("world.7z"));
    m_debug = SIM_NEW CDebug(g.GetFs("debug.7z"));

    m_world->SetEnabled(true);
    m_world->SetVisible(true);

    CCanvas* canvas = g.canvas;
    canvas->OnEvent.Connect(this, &CState_Game::OnEvent);
    canvas->OnGui.Connect(m_debug, &CDebug::ShowGui);

    m_drawable = SIM_NEW CDrawable();
    m_drawable->MoveTo(300.0f, 100.0f);
    m_drawable->Resize(512, 512);
    m_drawable->SetColor(col::Orange);
    m_drawable->OnDraw.Connect(this, &CState_Game::OnDrawableDraw);
}
// ----------------------------------------------------------------------//

void CState_Game::OnExit()
{
    SIM_PRINT("\nCState_Game::OnExit");

    m_drawable->OnDraw.Disconnect(this, &CState_Game::OnDrawableDraw);
    SIM_SAFE_DELETE(m_drawable);
//  Drawable still in ImGui render queue.
//  Solutions:
//      1. drawable not class member for CState_Game
//      2. clear canvas/ImGui draw list
    g.canvas->Reset();

    CCanvas* canvas = g.canvas;
    canvas->OnEvent.Disconnect(this, &CState_Game::OnEvent);
    canvas->OnGui.Disconnect(m_debug, &CDebug::ShowGui);

    SIM_SAFE_DELETE( m_debug );
    SIM_SAFE_DELETE( m_world );
}
// ----------------------------------------------------------------------//
void CState_Game::OnEvent(CCanvas* canvas, CCanvas::TEvent* ev, sigcxx::SLOT slot)
{
    if (ev->evType == CCanvas::EvType::Key && !ev->evKey.down)
    {
        if (ev->evKey.code == 66) // Key_B
        {
            IState::TFsList fsList = {
                "ui.7z",
            };
            auto fnDtor = UNLOAD_FNDTOR(fsList);
            g.game->GoPop(SIM_NEW CState_Loading(fsList, SIM_NEW CState_MenuMain(fnDtor)));
        }
    }
}
// ----------------------------------------------------------------------//