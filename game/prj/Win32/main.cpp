#include <windows.h>
#include <Game.h>

CGame* game = 0;

void onStart()
{
    if (!game)
    {
        game = SIM_NEW CGame("../../blob/");
        game->Start();
    }
}

void onUpdate()
{
    if (game)
    {
        game->Run();
    }
}

void onResize(int w, int h)
{
    if (game)
    {
        game->Resize(w, h);
    }
}

void onClose()
{
    if (game)
    {
        game->Quit();
        delete game;
        game = 0;
    }
}

int main(int argc, char *argv[])
{
    zpl_platform p;

    zpl_memset(&p, 0, sizeof(p));

    p.callbacks.zpl_window_open = onStart;
    p.callbacks.zpl_window_resize = onResize;
    p.callbacks.zpl_window_update = onUpdate;
    p.callbacks.zpl_window_close = onClose;

    zpl_platform_init(&p, "game-sdk", 640, 480, ZPL_WINDOW_RESIZABLE);

    while (game)
    {
        zpl_platform_update(&p);
        zpl_platform_display(&p);

        Sleep(1);
    }

    zpl_platform_destroy(&p);
}