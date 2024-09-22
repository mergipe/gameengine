#ifndef GAME_H
#define GAME_H

#include <SDL.h>

class Game {
private:
    bool isRunning{false};
    SDL_Window *window{NULL};
    SDL_Renderer *renderer{NULL};

public:
    Game() = default;
    Game(const Game &) = default;
    ~Game() = default;
    Game &operator=(const Game &);
    void initialize();
    void run();
    void processInput();
    void update(int timeStep);
    void render(float frameExtrapolationFactor);
    void destroy();
    const int windowWidth{1280};
    const int windowHeight{720};
    static constexpr int updateRate{144};
    static constexpr int timeStepInMs{1000 / updateRate};
};

#endif
