#ifndef GAME_H
#define GAME_H

#include <ECS.h>
#include <SDL.h>
#include <memory>

class Game {
private:
    bool isRunning{false};
    SDL_Window *window{};
    SDL_Renderer *renderer{};
    std::unique_ptr<Registry> registry{};
    void setup();
    void processInput();
    void update(float timeStep);
    void render(float frameExtrapolationFactor);
    const int windowWidth{1920};
    const int windowHeight{1080};
    static constexpr float updateRate{144.0f};
    static constexpr float timeStepInMs{1000.0f / updateRate};

public:
    Game() = default;
    Game(const Game &);
    ~Game() = default;
    Game &operator=(const Game &);
    void init();
    void run();
    void destroy();
};

#endif
