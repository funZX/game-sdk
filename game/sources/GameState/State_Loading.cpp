#include <core/io/sim_file_system.h>

#include <core/sim_state_machine.h>
#include <render/sim_canvas.h>
#include <render/sim_driver.h>
#include <render/sim_rect_2d.h>

#include "../Game.h"

#include "../World/World.h"

#include "State_Loading.h"
#include "State_Game.h"
// ----------------------------------------------------------------------//

std::map<std::string, CFileSystem*> CState_Loading::m_fsList = {
    { "debug.7z",       nullptr },
    { "ui.7z",          nullptr },
    { "world.7z",       nullptr },
    { "strawberry.7z",  nullptr },
};

// ----------------------------------------------------------------------//
CFileSystem* CState_Loading::GetFs( const std::string& fsName )
{
    std::map<std::string, CFileSystem*>::iterator it = m_fsList.find( fsName );
    return it == m_fsList.end() ? nullptr : m_fsList[fsName];
}

// ----------------------------------------------------------------------//
CState_Loading::CState_Loading( const std::vector<std::string>& fsLoad, IState* nextState )
{
    for (auto& fsName : fsLoad)
    {
        std::map<std::string, CFileSystem*>::iterator it = m_fsList.find(fsName);

        if (it == m_fsList.end())
            continue;

        it->second = SIM_NEW CFileSystem( g.game->GetFsPath( fsName ) );
    }

    m_childState    = nextState;
    m_dealloc       = false;
}
// ----------------------------------------------------------------------//
CState_Loading::~CState_Loading()
{
    for ( auto& fs : m_fsList )
        if ( fs.second )
            SIM_SAFE_DELETE( fs.second );

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

    if ( m_childState && m_dealloc )
    {
        SIM_SAFE_DELETE( m_childState ); // dealloc 
        g.game->GoBack(); // pop to CState_MenuMain
        return;
    }

    for ( auto& fs : m_fsList )
    {
        if ( fs.second )
            if ( fs.second->LoadNext() )
                return;
    }

    m_dealloc = true;
    g.game->GoNext( m_childState );
}
// ----------------------------------------------------------------------//
void CState_Loading::Render( CDriver *driver )
{
    SIM_PRINT("\nCState_Loading::Render");

    if ( !m_childState )
        return;
}
// ----------------------------------------------------------------------//
void CState_Loading::OnEnter( bool isPushed )
{
    SIM_PRINT("\nCState_Loading::OnEnter");

    if ( !isPushed )
        return;

    for ( auto& fs : m_fsList )
        if ( fs.second )
            fs.second->Open();
}
// ----------------------------------------------------------------------//
void CState_Loading::OnExit( bool isPoped )
{
    SIM_PRINT("\nCState_Loading::OnExit");

    if ( isPoped )
        return;

    for ( auto& fs : m_fsList )
        if ( fs.second )
            fs.second->Close();
}
// ----------------------------------------------------------------------//