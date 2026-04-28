#include "Game.h"

#include "SceneLoader.h"
#include "core/Config.h"
#include "core/Filesystem.h"
#include "core/Locator.h"
#include "core/Logger.h"
#include "core/Timer.h"
#include "renderer/Renderer2D.h"

#include <SDL3/SDL.h>
#include <filesystem>

namespace Engine
{
    Game& Game::Instance()
    {
        static Game instance{};
        return instance;
    }

    void Game::Init()
    {
        m_logger = std::make_unique<Logger>(Filesystem::GetLogsPath() / "log.txt");
        Locator::Provide(m_logger.get());
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#ifdef __APPLE__
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#else
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
#endif
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        Config::VideoConfig videoConfig{Config::ParseVideoConfig()};
        m_window = std::make_unique<Window>(videoConfig.windowConfig);
        m_window->Init();
        SDL_GL_SetSwapInterval(videoConfig.vsync);
        m_renderer = std::make_unique<Renderer2D>(m_window.get());
        m_renderer->Init();
        m_resourceManager = std::make_unique<ResourceManager>();
        Locator::Provide(m_resourceManager.get());
        const std::filesystem::path inputConfigFilepath{m_resourceManager->GetResourcePath("input") /
                                                        "game_input.yaml"}; // FIXME: hardcoded input file
        m_inputHandler = std::make_unique<InputHandler>(inputConfigFilepath);
        m_eventBus = std::make_unique<EventBus>();
        Locator::Provide(m_eventBus.get());
        if (m_hasDevMode) {
            m_devGui = std::make_unique<DevGuiImpl>(*m_window);
        } else {
            m_devGui = std::make_unique<NullDevGui>();
        }
        Locator::GetLogger()->Info("Game initialized");
    }

    void Game::Run()
    {
        Locator::GetLogger()->Info("Game started to run");
        m_isRunning = true;
        m_currentScene =
            SceneLoader::Load(m_resourceManager->GetResourcePath("scenes") / "scene1.yaml", *m_inputHandler,
                              *m_renderer); // FIXME: hardcoded scene file
        Timer::Ticks previousTicks{Timer::GetTicks()};
        float lag{0.0f};
        while (m_isRunning) {
            const Timer::Ticks currentTicks{Timer::GetTicks()};
            lag += static_cast<float>(currentTicks - previousTicks);
            previousTicks = currentTicks;
            ProcessEvents();
            while (lag >= s_timeStepInMs) {
                Update();
                lag -= s_timeStepInMs;
            }
            Render(lag / 1000.0f);
        }
        m_currentScene.reset();
    }

    void Game::ShutDown()
    {
        m_devGui.reset();
        m_eventBus.reset();
        m_resourceManager.reset();
        m_inputHandler.reset();
        m_renderer.reset();
        m_window.reset();
        Locator::GetLogger()->Info("Game resources destroyed");
    }

    void Game::ProcessEvents()
    {
        SDL_Event event{};
        while (SDL_PollEvent(&event)) {
            m_devGui->ProcessEvent(event);
            if (m_devGui->WantCaptureKeyboard() || m_devGui->WantCaptureMouse()) {
                m_inputHandler->SwitchScope(m_inputHandler->GetDevGuiScopeId());
            } else if (m_inputHandler->GetCurrentScopeId() == m_inputHandler->GetDevGuiScopeId()) {
                m_inputHandler->SwitchScope(m_inputHandler->GetPreviousScopeId());
            }
            switch (event.type) {
            case SDL_EVENT_QUIT:
                m_isRunning = false;
                break;
            case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
                m_renderer->SetViewportSize(event.window.data1, event.window.data2);
                break;
            case SDL_EVENT_KEY_DOWN:
                m_inputHandler->HandleKeyboardKeyDownEvent(event.key);
                break;
            case SDL_EVENT_KEY_UP:
                m_inputHandler->HandleKeyboardKeyUpEvent(event.key);
                break;
            default:
                break;
            }
        }
        m_inputHandler->ResolveInput();
    }

    void Game::Update() { m_currentScene->Update(s_timeStep); }

    void Game::Render(float frameExtrapolationTimeStep)
    {
        m_renderer->Clear();
        m_currentScene->Render(frameExtrapolationTimeStep);
        m_devGui->NewFrame();
        if (m_isDevModeEnabled) {
            m_devGui->Show();
        }
        m_devGui->Render();
        m_renderer->Present();
    }
} // namespace Engine
