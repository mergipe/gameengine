#include <Game.h>
#include <Logger.h>

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    Engine::Logger::init();
    Engine::Game game{true};
    game.init();
    game.run();
    game.destroy();
    return 0;
}
