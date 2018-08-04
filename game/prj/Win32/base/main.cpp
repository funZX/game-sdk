#include <windows.h>

#include "../../../../Game/sources/Game.h"
#include "simulator.h"

CGame *game = NULL;

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
	_crtBreakAlloc = 2318;
#endif

	atexit( onQuit );
}

void resizeFromLayer( int width, int height )
{
	timeBeginPeriod( 1 );

	game = new CGame("../../blob/");
	game->Start( width, height );
}

void quit( void )
{
	game->Quit();
	delete game;

	timeEndPeriod( 1 );
}

void key( unsigned char key, bool isDown )
{
	game->KeyPress( key, isDown );
}

void touchesBegan( CGPoint *pos, int tapCount )
{
	game->PointerDown( (int)pos->x, (int)pos->y );
}

void touchesMoved( CGPoint *pos )
{
	game->PointerDrag( (int)pos->x, (int)pos->y );
}

void touchesEnded( CGPoint *pos )
{
	game->PointerUp( (int)pos->x, (int)pos->y );
}

void render ( void )
{
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