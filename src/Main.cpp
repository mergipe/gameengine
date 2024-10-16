#include "game/Game.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    Engine::Game game{true};
    game.init();
    game.run();
    game.destroy();
    return 0;
}
