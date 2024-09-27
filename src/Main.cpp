#include <Game.h>
#include <Logger.h>

int main() {
    Logger::init();
    Game game{};
    game.init();
    game.run();
    game.destroy();
    return 0;
}
