#include <Game.h>
#include <Logger.h>

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    Logger::init();
    Game game{true};
    game.init();
    game.run();
    game.destroy();
    return 0;
}
