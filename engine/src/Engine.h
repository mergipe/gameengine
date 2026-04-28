#ifndef ENGINE_H
#define ENGINE_H

#include "core/Logger.h"
#include "core/Window.h"
#include "debug/DevGui.h"
#include "events/EventBus.h"
#include "input/InputHandler.h"
#include "renderer/Renderer2D.h"
#include "resources/ResourceManager.h"
#include "scene/Scene.h"

#include <memory>

namespace Engine
{
    class Engine final
    {
    public:
        static Engine& Instance();
        Engine() = default;
        Engine(const Engine&) = delete;
        Engine(Engine&&) = delete;
        Engine& operator=(const Engine&) = delete;
        Engine& operator=(Engine&&) = delete;
        ~Engine() = default;
        void Start();
        const Config::WindowConfig& GetWindowConfig() const { return m_window->GetConfig(); }
        bool HasDevMode() const { return m_hasDevMode; }
        bool IsDevModeEnabled() const { return m_isDevModeEnabled; }
        void ToggleDevMode() { m_isDevModeEnabled = !m_isDevModeEnabled; }
        static constexpr SDL_Scancode s_toggleDevModeKey{SDL_SCANCODE_F12};

    private:
        void Init();
        void InitSDL();
        void Run();
        void ShutDown();
        void ProcessEvents();
        void Update();
        void Render(float frameExtrapolationTimeStep);
        static constexpr float s_updateRate{60.0f};
        static constexpr float s_timeStepInMs{1000.0f / s_updateRate};
        static constexpr float s_timeStep{1.0f / s_updateRate};
        std::unique_ptr<Logger> m_logger{};
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
