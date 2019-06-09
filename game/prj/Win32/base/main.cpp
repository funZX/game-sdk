#include <windows.h>
#include <core/sim_core.h>

#include "../../../../Game/sources/Game.h"

zpl_platform p;

CGame *game = NULL;

void onQuit()
{
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif

    zpl_platform_destroy( &p );
}

void onStart()
{
#ifdef _DEBUG
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_crtBreakAlloc = 4469;
#endif

    int width   = 640;
    int height  = 480;

    zpl_platform_init(&p, "game-sdk", width, height, ZPL_WINDOW_RESIZABLE);

    game = new CGame("../../blob/");
    game->Start(width, height);

    atexit( onQuit );
}

void quit( void )
{
	game->Quit();
	delete game;
}

int main(int argc, char *argv[])
{
	onStart();


    while (!p.quit_requested)
    {
        game->Run();

        zpl_platform_update(&p);
        zpl_platform_display(&p);

        Sleep(1);
    }
}