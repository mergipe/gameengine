#include "game/Game.h"

int main(int, char*[])
{
    Engine::Game game{true};
    game.init();
    game.run();
    game.destroy();
    return 0;
}
