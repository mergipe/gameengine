#include "game/Game.h"

int main(int, char*[])
{
    Engine::Game& game{Engine::Game::instance()};
    game.init();
    game.run();
    game.shutDown();
    return 0;
}
