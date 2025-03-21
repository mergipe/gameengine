#ifndef GAME_H
#define GAME_H

#include "core/Window.h"
#include "events/EventBus.h"
#include "game/Scene.h"
#include "renderer/Renderer2D.h"
#include "resources/ResourceManager.h"
#include "tools/DeveloperModeGui.h"
#include <memory>
#include <string_view>

namespace Engine
{
    class Game final
    {
    public:
        static Game& instance();
        Game() = default;
        Game(const Game&) = delete;
        Game(Game&&) = delete;
        Game& operator=(const Game&) = delete;
        Game& operator=(Game&&) = delete;
        ~Game() = default;
        void init();
        void run();
        void shutDown();
        EventBus& getEventBus() { return *m_eventBus; }
        DeveloperModeGui& getDeveloperModeGui() { return *m_developerModeGui; }
        bool hasDeveloperMode() { return m_hasDeveloperMode; }
        bool isDeveloperModeEnabled() { return m_isDeveloperModeEnabled; }

    private:
        void processInput();
        void update();
        void render(float frameExtrapolationFactor);
        static constexpr std::string_view s_windowTitle{"Game Engine"};
        static constexpr float s_updateRate{144.0f};
        static constexpr float s_timeStepInMs{1000.0f / s_updateRate};
        static constexpr int s_windowWidth{3200};
        static constexpr int s_windowHeight{1800};
        std::unique_ptr<ResourceManager> m_resourceManager{};
        std::unique_ptr<EventBus> m_eventBus{};
        std::unique_ptr<Window> m_window{};
        std::unique_ptr<Renderer2D> m_renderer{};
        std::unique_ptr<Scene> m_currentScene{};
        std::unique_ptr<DeveloperModeGui> m_developerModeGui{};
        bool m_hasDeveloperMode{true};
        bool m_isDeveloperModeEnabled{false};
        bool m_isRunning{false};
    };
} // namespace Engine

#endif
