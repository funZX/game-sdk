
#include <render/scene/sim_light.h>
#include <render/scene/sim_camera.h>

#include <render/gui/sim_widget_drawable.h>

#include <render/sim_canvas.h>
#include <render/sim_texture.h>
#include <render/sim_material.h>
#include <render/sim_shader.h>
#include <render/sim_effect.h>
#include <render/sim_mesh.h>

#include <core/io/sim_file_system.h>
#include <render/sim_material.h>

#include <render/sim_canvas.h>
#include <render/sim_driver.h>

#include <core/sim_state_machine.h>

#include <vm/sim_script.h>
#include <vm/sim_squirrel.h>

#include "../Game.h"

#include "../World/World.h"

#include "State_Game.h"
// ----------------------------------------------------------------------//
CState_Game::CState_Game()
{
	m_fs = O.world->GetFs();
	m_mesh = m_fs->GetMesh("melonman/melonman");

	m_drawable = SIM_NEW CWidgetDrawable();
	m_drawable->MoveTo(100.0f, 100.0f);
	m_drawable->Resize(200, 200);
	m_drawable->SetColor(&col::Blueish);
	m_drawable->OnDraw.Connect(this, &CState_Game::DrawToWidget);

	CScript* script = O.world->GetFs()->GetScript("dt/deltatime");
	script->Run();
}
// ----------------------------------------------------------------------//
CState_Game::~CState_Game()
{
	SIM_SAFE_DELETE( m_drawable );
}
// ----------------------------------------------------------------------//
void CState_Game::Update( f32 dt, void *userData )
{
	O.world->Update( dt, userData );
}
// ----------------------------------------------------------------------//
void CState_Game::Render2D( CDriver *driver )
{
	O.world->Render2D(driver);

	static CEffect* fill	= m_fs->GetEffect("color/fill_color");
	static CEffect* filltex = m_fs->GetEffect("color/fill_color_texture_color");
	f32 dr = 20.0f * driver->GetTimerRot();

	CMaterial m;
	CRect2D r;

	r.SetMaterial(&m);

	m.SetEffect(fill);
	m.SetTexture(0, 0);

	r.Bound(m_drawable);
	r.Zoom(6.0f, 6.0f);
	r.Rotate(dr);
	r.Render(driver, CRect2D::OneSizeRect);

	bool isBatchEnabled =
	driver->EnableBatch2D(false);

	CEffect::TTechnique techique;
	filltex->CopyTechnique(&techique);
	filltex->m_technique.depthtest = false;

	m_drawable->SetMaterial(&m);
	m.SetEffect(filltex);
	m.SetTexture(m_drawable->GetTexture(), 0);
	m_drawable->Rotate(dr);
	m_drawable->Render(driver);

	filltex->SetTechnique(&techique);
	driver->EnableBatch2D(isBatchEnabled);
}
// ----------------------------------------------------------------------//
void CState_Game::Render3D( CDriver *driver )
{
	O.world->Render( driver );

	f32 dr = driver->GetTimerRot();

	driver->MatrixPush();
	{

	}
	driver->MatrixPop();

	m_drawable->Draw(driver);
}

// ----------------------------------------------------------------------//
void CState_Game::DrawToWidget(CDriver* driver, sigcxx::SLOT slot)
{
	f32 dr = driver->GetTimerRot();

	driver->MatrixPush();
	driver->MatrixTranslateY(-0.5f);
	driver->MatrixTranslateZ(-2.0f);
	driver->MatrixRotateY(dr * 60.0f);
	m_mesh->Render(driver);
	driver->MatrixPop();

	driver->MatrixPush();
	driver->MatrixTranslate(-0.4f, -0.2f, -1.5f);
	driver->MatrixRotateY(dr * 20.0f);
	m_mesh->Render(driver);
	driver->MatrixPop();

	driver->MatrixPush();
	driver->MatrixTranslate(0.4f, -0.2f, -1.5f);
	driver->MatrixScale(0.5f, 0.5f, 0.5f);
	driver->MatrixRotateY(dr * 40.0f);
	m_mesh->Render(driver);
	driver->MatrixPop();
}
// ----------------------------------------------------------------------//