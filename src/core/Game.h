#ifndef GAME_H
#define GAME_H

#include <ECS.h>
#include <SDL.h>

class Game {
private:
    bool isRunning{false};
    SDL_Window *window{nullptr};
    SDL_Renderer *renderer{nullptr};
    Registry *registry{nullptr};

public:
    Game();
    Game(const Game &) = default;
    ~Game() = default;
    Game &operator=(const Game &);
    void init();
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
