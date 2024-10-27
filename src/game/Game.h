#ifndef GAME_H
#define GAME_H

#include "ECS.h"
#include "Events.h"
#include "resources/ResourceManager.h"
#include <SDL.h>
#include <memory>
#include <string_view>

namespace Engine
{
    class Game
    {
    public:
        Game(bool debugMode);
        Game(const Game&);
        Game& operator=(const Game&);
        void init();
        void run();
        void destroy();

    private:
        void loadMap(std::string_view tilesetFilename, std::string_view tilemapFilename, int tileWidth,
                     int tileHeight, int tilesetColumns, float scale);
        void loadEntities();
        void loadLevel(int level);
        void setup();
        void processInput();
        void update();
        void render(float frameExtrapolationFactor);
        std::filesystem::path m_basePath{};
        std::unique_ptr<Registry> m_registry{};
        std::unique_ptr<ResourceManager> m_resourceManager{};
        std::unique_ptr<EventBus> m_eventBus{};
        SDL_Window* m_window{};
        SDL_Renderer* m_renderer{};
        static constexpr float s_updateRate{144.0f};
        static constexpr float s_timeStepInMs{1000.0f / s_updateRate};
        const int m_windowWidth{1920};
        const int m_windowHeight{1080};
        bool m_debugCapability{false};
        bool m_debugModeActivated{false};
        bool m_isRunning{false};
    };
} // namespace Engine

#endif
