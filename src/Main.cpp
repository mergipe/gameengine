#include "game/Game.h"

#include <iostream>

int main(int, char*[])
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << '\n';
        std::abort();
    }
    Engine::Game& game{Engine::Game::Instance()};
    game.Init();
    game.Run();
    game.ShutDown();
    SDL_Quit();
    return 0;
}
