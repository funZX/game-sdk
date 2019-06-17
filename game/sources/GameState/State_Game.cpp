
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
	m_mesh = m_fs->GetMesh("melonman");

	m_drawable = SIM_NEW CDrawable();
	m_drawable->MoveTo(300.0f, 100.0f);
	m_drawable->Resize(200, 200);
	m_drawable->SetColor(col::Blueish);
	m_drawable->OnDraw.Connect(this, &CState_Game::DrawToWidget);

	CScript* script = O.world->GetFs()->GetScript("deltatime");
	script->Run();
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
	O.world->Render( driver );
	m_drawable->Draw(driver);
}

// ----------------------------------------------------------------------//
void CState_Game::DrawToWidget(CDriver* driver, sigcxx::SLOT slot)
{
    CDebug d;

    d.Render( driver );
}
// ----------------------------------------------------------------------//
void CState_Game::OnEnter()
{

}
// ----------------------------------------------------------------------//
void CState_Game::OnGui(CDriver* driver)
{
    CEffect* fill = O.effect.color;

    CEffect::TTechnique techique;
    fill->CopyTechnique(&techique);
    fill->m_technique.depthtest = false;

    m_drawable->SetEffect(fill);
    m_drawable->Render(driver);

    fill->SetTechnique(&techique);

    O.canvas->DrawString(driver,
        10,
        (s32)(O.canvas->Height() - 2 * O.font.engine->GetHeight()),
        "The quick brown fox jumps over the lazy dog! :;.,'\"(?)+=*/=1234567890", col::Magenta);
}
// ----------------------------------------------------------------------//

void CState_Game::OnExit()
{

}
// ----------------------------------------------------------------------//