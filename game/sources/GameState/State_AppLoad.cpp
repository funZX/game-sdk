#include <core/io/sim_file_system.h>

#include <render/scene/sim_actor.h>
#include <render/font/sim_font.h>
#include <render/sim_canvas.h>
#include <render/sim_driver.h>

#include "../Game.h"

#include "../World/World.h"

#include "State_MenuMain.h"
#include "State_AppLoad.h"

// ----------------------------------------------------------------------//
CState_AppLoad::CState_AppLoad()
{
	m_fsui = SIM_NEW CFileSystem( O.game->GetFsPath("ui.7z") );
	m_fsworld = O.world->GetFs();
	m_fsCrt = m_fsui;
}
// ----------------------------------------------------------------------//
CState_AppLoad::~CState_AppLoad()
{
	SIM_SAFE_DELETE( m_fsui );
	SIM_PRINT("~CState_AppLoad");
}
// ----------------------------------------------------------------------//
void CState_AppLoad::Update( f32 dt, void *userData )
{
	if (!m_fsCrt->LoadNext())
		m_fsCrt = (m_fsCrt == m_fsworld ? NULL : m_fsworld);

	if (m_fsCrt == NULL)
		O.game->GoNext(SIM_NEW CState_MenuMain());
}
// ----------------------------------------------------------------------//
void CState_AppLoad::Render2D( CDriver *driver )
{
	if (m_fsCrt != NULL)
	{
		O.game->Print(driver,
			0,
			(s32)(O.canvas->Height() - 2 * O.font.engine->GetHeight()),
			m_fsCrt->GetLoadMessage());
	}
}
// ----------------------------------------------------------------------//
void CState_AppLoad::Render3D( CDriver *driver )
{

}
// ----------------------------------------------------------------------//
void CState_AppLoad::OnEnter()
{
	m_fsui->Open();
	m_fsworld->Open();
}
// ----------------------------------------------------------------------//
void CState_AppLoad::OnExit()
{
	m_fsworld->Close();
	m_fsui->Close();

	// FONTS
	O.font.roboto.bold12 = m_fsui->GetFont("roboto/Roboto-Bold12");
	O.font.roboto.bold14 = m_fsui->GetFont("roboto/Roboto-Bold14");
	O.font.roboto.bold16 = m_fsui->GetFont("roboto/Roboto-Bold16");

	O.font.roboto.italic12 = m_fsui->GetFont("roboto/Roboto-Italic12");
	O.font.roboto.italic14 = m_fsui->GetFont("roboto/Roboto-Italic14");
	O.font.roboto.italic16 = m_fsui->GetFont("roboto/Roboto-Italic16");

	O.font.roboto.light12 = m_fsui->GetFont("roboto/Roboto-Light12");
	O.font.roboto.light14 = m_fsui->GetFont("roboto/Roboto-Light14");
	O.font.roboto.light16 = m_fsui->GetFont("roboto/Roboto-Light16");

	O.font.roboto.medium12 = m_fsui->GetFont("roboto/Roboto-Medium12");
	O.font.roboto.medium14 = m_fsui->GetFont("roboto/Roboto-Medium14");
	O.font.roboto.medium16 = m_fsui->GetFont("roboto/Roboto-Medium16");

	O.font.roboto.regular12 = m_fsui->GetFont("roboto/Roboto-Regular12");
	O.font.roboto.regular14 = m_fsui->GetFont("roboto/Roboto-Regular14");
	O.font.roboto.regular16 = m_fsui->GetFont("roboto/Roboto-Regular16");

	O.font.roboto.thin12 = m_fsui->GetFont("roboto/Roboto-Thin12");
	O.font.roboto.thin14 = m_fsui->GetFont("roboto/Roboto-Thin14");
	O.font.roboto.thin16 = m_fsui->GetFont("roboto/Roboto-Thin16");

	// EFFECTS
	O.effect.color = m_fsworld->GetEffect("default");
	O.effect.vertex = m_fsworld->GetEffect("vertex");
	O.effect.texture = m_fsworld->GetEffect("texture");

	O.effect.debug.normals = m_fsworld->GetEffect("debug.normals");

	O.effect.fill.texture = m_fsworld->GetEffect("fill.texture");

	O.effect.fill.vertex.color = m_fsworld->GetEffect("fill.vertex");
	O.effect.fill.vertex.texture = m_fsworld->GetEffect("fill.vertex.texture");

	O.effect.diffuse.color = m_fsworld->GetEffect("fill.diffuse");
	O.effect.diffuse.texture = m_fsworld->GetEffect("fill.texture.diffuse");

	O.effect.diffuse.vertex.color = m_fsworld->GetEffect("fill.vertex.diffuse");
	O.effect.diffuse.vertex.texture = m_fsworld->GetEffect("fill.vertex.texture.diffuse");

	O.effect.lighting.bumpmap = m_fsworld->GetEffect("lighting.bumpmap");
	O.effect.lighting.parallax = m_fsworld->GetEffect("lighting.parallax");
	O.effect.lighting.phong = m_fsworld->GetEffect("lighting.phong");
	O.effect.lighting.reflection = m_fsworld->GetEffect("lighting.reflection");

	O.effect.particle.sprite = m_fsworld->GetEffect("particle.sprite");

	O.world->SetEnabled( true );
	O.world->SetVisible( true );
}
// ----------------------------------------------------------------------//