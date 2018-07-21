#include <windows.h>

#include "../../../../Game/sources/Game.h"
#include "simulator.h"

void onQuit()
{
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
}

void onStart()
{
#ifdef _DEBUG
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	_crtBreakAlloc = 139;
#endif

	atexit( onQuit );
}

void resizeFromLayer( int width, int height )
{
	timeBeginPeriod( 1 );

	CGame *game = new CGame("../../blob/");
	game->Start( width, height );
}

void quit( void )
{
	CGame *game = CGame::GetSingletonPtr();

	game->Quit();
	delete game;

	timeEndPeriod( 1 );
}

void key( unsigned char key, bool isDown )
{
	static CGame *game = CGame::GetSingletonPtr();
	game->KeyPress( key, isDown );
}

void touchesBegan( CGPoint *pos, int tapCount )
{
	static CGame *game = CGame::GetSingletonPtr();
	game->PointerDown( (int)pos->x, (int)pos->y );
}

void touchesMoved( CGPoint *pos )
{
	static CGame *game = CGame::GetSingletonPtr();
	game->PointerDrag( (int)pos->x, (int)pos->y );
}

void touchesEnded( CGPoint *pos )
{
	static CGame *game = CGame::GetSingletonPtr();
	game->PointerUp( (int)pos->x, (int)pos->y );
}

void render ( void )
{
	static CGame *game = CGame::GetSingletonPtr();
	game->Run();

#if SIM_DEBUG
	_CrtCheckMemory();
#endif
}

int main(int argc, char *argv[])
{
	onStart();

	SimulatorSetRenderCallback( render );
	SimulatorSetResizeFromLayerCallback( resizeFromLayer );
	SimulatorSetTouchesBeganCallback( touchesBegan );
	SimulatorSetTouchesMovedCallback( touchesMoved );
	SimulatorSetTouchesEndedCallback( touchesEnded );
	SimulatorSetKeyCallback( key );
	SimulatorSetQuitCallback( quit );

	SimulatorRun( 800, 600 );
}