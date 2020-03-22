
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
// ----------------------------------------------------------------------//
CState_Game::CState_Game()
{
	m_fs = O.world->GetFs();

	m_drawable = SIM_NEW CDrawable();
	m_drawable->MoveTo(300.0f, 100.0f);
	m_drawable->Resize(200, 200);
	m_drawable->SetColor(col::Orange);
	m_drawable->OnDraw.Connect(this, &CState_Game::DrawToWidget);

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
	SIM_SAFE_DELETE( m_drawable );
    SIM_PRINT("\n~CState_Game");
}
// ----------------------------------------------------------------------//
void CState_Game::Update( f32 dt, void *userData )
{
    O.world->Update( dt, userData );
}

// ----------------------------------------------------------------------//
void CState_Game::Render( CDriver *driver )
{
    m_drawable->Draw(driver);
    O.world->Render(driver);

    CDebug d;
    d.Render(driver);
}

// ----------------------------------------------------------------------//
void CState_Game::DrawToWidget(CDriver* driver, sigcxx::SLOT slot)
{
    O.world->Render(driver);

    CDebug d;
    d.Render(driver);
}
// ----------------------------------------------------------------------//
void CState_Game::OnEnter()
{

}
// ----------------------------------------------------------------------//
void CState_Game::ShowGui( CCanvas* canvas )
{
    ImGui::ShowDemoWindow();

    CEffect* fill = O.effect.color;
    static CMaterial m;

    m.SetEffect(fill);

    CEffect::TTechnique techique;
    fill->CopyTechnique(&techique);
    fill->m_technique.depthtest = false;

    m_drawable->SetMaterial(&m);
    m_drawable->Render(O.driver);

    fill->SetTechnique(&techique);
}
// ----------------------------------------------------------------------//

void CState_Game::OnExit()
{

}
// ----------------------------------------------------------------------//