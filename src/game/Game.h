#ifndef GAME_H
#define GAME_H

#include <ECS.h>
#include <Events.h>
#include <ResourceManager.h>
#include <SDL.h>
#include <memory>

namespace Engine {

class Game {
private:
    bool debugCapability{false};
    bool debugModeActivated{false};
    bool isRunning{false};
    SDL_Window *window{};
    SDL_Renderer *renderer{};
    std::unique_ptr<Registry> registry{};
    std::unique_ptr<ResourceManager> resourceManager{};
    std::unique_ptr<EventBus> eventBus{};
    void loadMap(std::string_view tilesetFilename, std::string_view tilemapFilename, int tileWidth,
                 int tileHeight, int tilesetColumns, float scale);
    void loadEntities();
    void loadLevel(int level);
    void setup();
    void processInput();
    void update();
    void render(float frameExtrapolationFactor);
    const int windowWidth{1920};
    const int windowHeight{1080};
    static constexpr float updateRate{144.0f};
    static constexpr float timeStepInMs{1000.0f / updateRate};

public:
    Game(bool debugMode);
    Game(const Game &);
    ~Game() = default;
    Game &operator=(const Game &);
    void init();
    void run();
    void destroy();
};

} // namespace Engine

#endif
