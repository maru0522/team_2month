#include <Windows.h>
#include "GameSystem.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    BasicFrame* game = new GameSystem();

    game->Run();

    delete game;

    return 0;
}