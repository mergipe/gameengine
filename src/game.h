#ifndef GAME_H
#define GAME_H

#include <SDL.h>

constexpr int updateRate{144};
constexpr int timeStepInMs{1000 / updateRate};

class Game {
private:
    bool isRunning{false};
    SDL_Window *window{NULL};
    SDL_Renderer *renderer{NULL};

public:
    Game();
    Game(const Game &);
    ~Game();
    Game &operator=(const Game &);
    void initialize();
    void run();
    void processInput();
    void update();
    void render(float frameExtrapolationFactor);
    void destroy();
    const int windowWidth{1280};
    const int windowHeight{720};
};

#endif
