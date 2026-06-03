#include "Engine.h"

#include "core/ConfigManager.h"
#include "core/Locator.h"
#include "core/Logger.h"
#include "core/Timer.h"
#include "renderer/RenderManager.h"
#include "scene/SceneManager.h"

#include <SDL3/SDL.h>

namespace Engine
{
    Engine& Engine::Instance()
    {
        static Engine instance{};
        return instance;
    }

    void Engine::Start()
    {
        Init();
        Run();
        ShutDown();
    }

    void Engine::Init()
    {
        m_logger = std::make_unique<Logger>();
        m_logger->Init();
        Locator::Provide(m_logger.get());
        InitSDL();
        ConfigManager::Init();
        const VideoConfig& videoConfig{ConfigManager::GetVideoConfig()};
        m_window = std::make_unique<Window>();
        m_window->Create(videoConfig.windowConfig);
        SDL_GL_SetSwapInterval(videoConfig.vsync);
        m_renderManager = std::make_unique<RenderManager>();
        m_renderManager->Init();
        Locator::Provide(m_renderManager.get());
        m_resourceManager = std::make_unique<ResourceManager>();
        m_resourceManager->Init();
        Locator::Provide(m_resourceManager.get());
        m_inputManager = std::make_unique<InputManager>();
        m_inputManager->Init();
        Locator::Provide(m_inputManager.get());
        m_eventBus = std::make_unique<EventBus>();
        Locator::Provide(m_eventBus.get());
        if (m_hasDevMode) {
            m_devGui = std::make_unique<DevGuiImpl>(m_window.get());
        } else {
            m_devGui = std::make_unique<NullDevGui>();
        }
        m_devGui->Init();
        m_sceneManager = std::make_unique<SceneManager>();
        m_sceneManager->Init();
        m_physicsEngine2D = std::make_unique<PhysicsEngine2D>();
        m_physicsEngine2D->Init();
        Locator::Provide(m_physicsEngine2D.get());
    }

    void Engine::InitSDL()
    {
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            Locator::GetLogger()->Critical("Failed to initialize SDL: {}", SDL_GetError());
            std::abort();
        }
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#ifdef __APPLE__
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#else
#ifdef DEBUG
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#else
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
#endif
#endif
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    }

    void Engine::Run()
    {
        Locator::GetLogger()->Info("Engine started running");
        m_isRunning = true;
        m_sceneManager->LoadScene(ConfigManager::GetGameConfig().initialScene);
        Timer::Ticks previousTicks{Timer::GetTicks()};
        float lagInNs{0.0f};
        while (m_isRunning) {
            const Timer::Ticks currentTicks{Timer::GetTicks()};
            lagInNs += static_cast<float>(currentTicks - previousTicks);
            previousTicks = currentTicks;
            ProcessEvents();
            while (lagInNs >= s_timeStepInNs) {
                Update();
                lagInNs -= s_timeStepInNs;
            }
            Render(lagInNs / 1'000'000'000.0f);
        }
    }

    void Engine::ShutDown()
    {
        m_sceneManager->ShutDown();
        m_devGui->ShutDown();
        m_eventBus.reset();
        m_resourceManager->ShutDown();
        m_inputManager->ShutDown();
        m_renderManager->ShutDown();
        m_window->Destroy();
        SDL_Quit();
        m_logger->ShutDown();
    }

    void Engine::ProcessEvents()
    {
        SDL_Event event{};
        while (SDL_PollEvent(&event)) {
            m_devGui->ProcessEvent(event);
            if (m_devGui->WantCaptureKeyboard() || m_devGui->WantCaptureMouse()) {
                m_inputManager->SwitchScope(m_inputManager->GetDevGuiScopeId());
            } else if (m_inputManager->GetCurrentScopeId() == m_inputManager->GetDevGuiScopeId()) {
                m_inputManager->SwitchScope(m_inputManager->GetPreviousScopeId());
            }
            switch (event.type) {
            case SDL_EVENT_QUIT:
                m_isRunning = false;
                break;
            case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
                m_renderManager->OnViewportResize(event.window.data1, event.window.data2);
                m_sceneManager->OnViewportResize(event.window.data1, event.window.data2);
                break;
            case SDL_EVENT_KEY_DOWN:
                m_inputManager->HandleKeyboardKeyDownEvent(event.key);
                break;
            case SDL_EVENT_KEY_UP:
                m_inputManager->HandleKeyboardKeyUpEvent(event.key);
                break;
            default:
                break;
            }
        }
        m_inputManager->ResolveInput();
    }

    void Engine::Update() { m_sceneManager->GetCurrentScene()->Update(s_timeStep); }

    void Engine::Render(float frameExtrapolationTimeStep)
    {
        m_renderManager->Clear();
        m_sceneManager->GetCurrentScene()->Render(frameExtrapolationTimeStep);
        m_devGui->NewFrame();
        if (m_isDevModeEnabled) {
            m_renderManager->RenderDevGui();
            m_physicsEngine2D->DebugDraw();
            m_sceneManager->RenderDevGui();
            m_devGui->Show();
        }
        m_renderManager->Flush();
        m_devGui->Render();
        m_renderManager->Present(*m_window);
    }
} // namespace Engine
