#ifndef GAME_H
#define GAME_H

#include "core/Window.h"
#include "debug/DevGui.h"
#include "events/EventBus.h"
#include "game/Scene.h"
#include "input/InputHandler.h"
#include "renderer/Renderer2D.h"
#include "resources/ResourceManager.h"
#include <memory>

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
        EventBus& getEventBus() const { return *m_eventBus; }
        DevGui& getDevGui() const { return *m_devGui; }
        const WindowConfig& getWindowConfig() const { return m_window->getConfig(); }
        bool hasDevMode() const { return m_hasDevMode; }
        bool isDevModeEnabled() const { return m_isDevModeEnabled; }
        void toggleDevMode() { m_isDevModeEnabled = !m_isDevModeEnabled; }
        static constexpr SDL_Scancode s_toggleDevModeKey{SDL_SCANCODE_F12};

    private:
        void processEvents();
        void update();
        void render(float frameExtrapolationFactor);
        static constexpr float s_updateRate{144.0f};
        static constexpr float s_timeStepInMs{1000.0f / s_updateRate};
        std::unique_ptr<Window> m_window{};
        std::unique_ptr<Renderer2D> m_renderer{};
        std::unique_ptr<InputHandler> m_inputHandler{};
        std::unique_ptr<ResourceManager> m_resourceManager{};
        std::unique_ptr<EventBus> m_eventBus{};
        std::unique_ptr<Scene> m_currentScene{};
        std::unique_ptr<DevGui> m_devGui{};
        bool m_hasDevMode{true};
        bool m_isDevModeEnabled{false};
        bool m_isRunning{false};
    };
} // namespace Engine

#endif
