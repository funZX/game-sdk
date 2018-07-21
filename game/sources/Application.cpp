#include <core/sys/sim_thread.h>
#include <render/sim_canvas.h>
#include <render/sim_driver.h>

#include "Application.h"
#include "Game.h"

CApplication::CApplication()
{
	m_simarian			= SIM_NEW CSimarian("simarian");
	O.simarian			= m_simarian;
	O.driver			= m_simarian->GetDriver();
	O.effect			= m_simarian->GetEffect();

	m_ALDevice			= NULL;
	m_ALContext			= NULL;

	m_currentTime		= 0;
	m_updateTime		= 0;
	m_frameTime			= 0;
	m_deltaTime			= 0.0f;
}

CApplication::~CApplication()
{
	SIM_SAFE_DELETE( m_simarian );
}

void CApplication::InitOpenGL( void )
{
	O.driver->Initialize();
}

void CApplication::InitOpenAL()
{
	m_ALDevice = alcOpenDevice( NULL );

	if( m_ALDevice != NULL )
	{	
		m_ALContext = alcCreateContext( m_ALDevice, NULL );
		alcMakeContextCurrent( m_ALContext );
	}
}

void CApplication::Start( int width, int height )
{
	m_Game = SIM_NEW CGame( "simarian", width, height, "../../../blob/" );

	InitOpenGL();
	InitOpenAL();

	m_Game->Start();
}

void CApplication::Run( void )
{
	u32 begin		= GetTime();

	m_currentTime	= begin;
	m_frameTime		= m_currentTime - m_updateTime;
	m_deltaTime		= m_frameTime / 1000.0f;
	m_updateTime	= m_currentTime;

	m_Game->Update( m_deltaTime, this );
	m_Game->Render( O.driver );

	u32 end			= GetTime();
	if( m_frameTime > MinDt )
		end += m_frameTime - MinDt;

	s32 dtMili		= end - begin;
	s32 wtMili		= min( MinDt, max( 1, MinDt - dtMili ) );
	
	CThread::Wait( wtMili );
}

void CApplication::Quit( void )
{
	m_Game->Stop();

	SIM_SAFE_DELETE( m_Game );

	alcMakeContextCurrent( NULL );
	
	if( m_ALContext != NULL )
	{
		alcDestroyContext( m_ALContext );
		
		alcCloseDevice( m_ALDevice );
	}
}

void CApplication::PointerDown( u32 x, u32 y )
{
	m_Game->PointerDown( x, y );
}

void CApplication::PointerDrag( u32 x, u32 y )
{
	m_Game->PointerDrag( x, y );
}

void CApplication::PointerUp( u32 x, u32 y )
{
	m_Game->PointerUp( x, y );
}

void CApplication::KeyPress( u8 key, bool isDown )
{
	m_Game->KeyPress( key, isDown );
}
