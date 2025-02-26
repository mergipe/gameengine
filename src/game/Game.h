#ifndef GAME_H
#define GAME_H

#include "ECS.h"
#include "core/Window.h"
#include "events/EventBus.h"
#include "renderer/Camera.h"
#include "renderer/Renderer.h"
#include "resources/ResourceManager.h"
#include <filesystem>
#include <memory>
#include <string_view>

namespace Engine
{
    class Game final
    {
    public:
        explicit Game(bool debugCapability)
            : m_debugCapability{debugCapability}
        {
        }
        Game(const Game&) = delete;
        Game(Game&&) = delete;
        Game& operator=(const Game&) = delete;
        Game& operator=(Game&&) = delete;
        ~Game() = default;
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
        static constexpr std::string_view s_logFilepath{"logs/log.txt"};
        static constexpr std::string_view s_resourcesFolder{"resources"};
        static constexpr std::string_view s_tilemapsFolder{"tilemaps"};
        static constexpr std::string_view s_texturesFolder{"textures"};
        static constexpr std::string_view s_windowTitle{"Game Engine"};
        static constexpr float s_updateRate{144.0f};
        static constexpr float s_timeStepInMs{1000.0f / s_updateRate};
        static constexpr int s_windowWidth{1920};
        static constexpr int s_windowHeight{1080};
        std::filesystem::path m_basePath{};
        Camera m_camera{};
        std::unique_ptr<Registry> m_registry{};
        std::unique_ptr<ResourceManager> m_resourceManager{};
        std::unique_ptr<EventBus> m_eventBus{};
        std::unique_ptr<Window> m_window{};
        std::unique_ptr<Renderer> m_renderer{};
        bool m_debugCapability{false};
        bool m_debugModeActivated{false};
        bool m_isRunning{false};
    };
} // namespace Engine

#endif
