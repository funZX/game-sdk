#include <core/io/sim_file_system.h>

#include <core/sim_state_machine.h>
#include <render/sim_canvas.h>
#include <render/sim_driver.h>
#include <render/sim_rect_2d.h>

#include "../Game.h"

#include "../World/World.h"

#include "State_Loading.h"

// ----------------------------------------------------------------------//
CState_Loading::CState_Loading( const IState::TFsList& fsLoad, IState* nextState )
{
    for (auto& fsName : fsLoad)
    {
        TFsListIterator it = g.fsList.find(fsName);

        if (it == g.fsList.end())
            continue;

        if (it->second != nullptr)
            continue;

        it->second = SIM_NEW CFileSystem( g.game->GetFsPath( fsName ) );
    }

    m_childState    = nextState;
}
// ----------------------------------------------------------------------//
CState_Loading::~CState_Loading()
{
    SIM_PRINT("\n~CState_Loading");
}
// ----------------------------------------------------------------------//
void CState_Loading::ShowGui( CCanvas* canvas )
{
}
// ----------------------------------------------------------------------//
void CState_Loading::Update( f32 dt, void *userData )
{
    SIM_PRINT("\nCState_Loading::Update");

    for ( auto& fs : g.fsList )
    {
        if ( fs.second )
            if ( fs.second->LoadNext() )
                return;
    }

    g.game->GoPop( m_childState );
}
// ----------------------------------------------------------------------//
void CState_Loading::Render( CDriver *driver )
{
    SIM_PRINT("\nCState_Loading::Render");
}
// ----------------------------------------------------------------------//
void CState_Loading::OnEnter()
{
    SIM_PRINT("\nCState_Loading::OnEnter");

    for ( auto& fs : g.fsList )
        if ( fs.second )
            fs.second->Open();
}
// ----------------------------------------------------------------------//
void CState_Loading::OnExit()
{
    SIM_PRINT("\nCState_Loading::OnExit");

    for ( auto& fs : g.fsList )
        if ( fs.second )
            fs.second->Close();
}
// ----------------------------------------------------------------------//