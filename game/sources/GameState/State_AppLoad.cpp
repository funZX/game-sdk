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
			(s32)(O.canvas->Height() - 2 * O.font->GetHeight()),
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

	O.world->SetEnabled( true );
	O.world->SetVisible( true );
}
// ----------------------------------------------------------------------//