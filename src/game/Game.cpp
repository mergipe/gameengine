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
    Game& Game::instance()
    {
        static Game instance{};
        return instance;
    }

    void Game::init()
    {
        m_logger = std::make_unique<Logger>(Filesystem::getLogsPath() / "log.txt");
        Locator::provide(m_logger.get());
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
        Config::VideoConfig videoConfig{Config::loadVideoConfig()};
        m_window = std::make_unique<Window>(videoConfig.windowConfig);
        m_window->init();
        SDL_GL_SetSwapInterval(videoConfig.vsync);
        m_renderer = std::make_unique<Renderer2D>(m_window.get());
        m_renderer->init();
        m_resourceManager = std::make_unique<ResourceManager>();
        Locator::provide(m_resourceManager.get());
        const std::filesystem::path inputConfigFilepath{m_resourceManager->getResourcePath("input") /
                                                        "game_input.yaml"}; // FIXME: hardcoded input file
        m_inputHandler = std::make_unique<InputHandler>(inputConfigFilepath);
        m_eventBus = std::make_unique<EventBus>();
        Locator::provide(m_eventBus.get());
        if (m_hasDevMode) {
            m_devGui = std::make_unique<DevGuiImpl>(*m_window);
        } else {
            m_devGui = std::make_unique<NullDevGui>();
        }
        Locator::getLogger()->info("Game initialized");
    }

    void Game::run()
    {
        Locator::getLogger()->info("Game started to run");
        m_isRunning = true;
        m_currentScene =
            SceneLoader::load(m_resourceManager->getResourcePath("scenes") / "scene1.yaml", *m_inputHandler,
                              *m_renderer); // FIXME: hardcoded scene file
        Timer::Ticks previousTicks{Timer::getTicks()};
        float lag{0.0f};
        while (m_isRunning) {
            const Timer::Ticks currentTicks{Timer::getTicks()};
            lag += static_cast<float>(currentTicks - previousTicks);
            previousTicks = currentTicks;
            processEvents();
            while (lag >= s_timeStepInMs) {
                update();
                lag -= s_timeStepInMs;
            }
            render(lag / 1000.0f);
        }
        m_currentScene.reset();
    }

    void Game::shutDown()
    {
        m_devGui.reset();
        m_eventBus.reset();
        m_resourceManager.reset();
        m_inputHandler.reset();
        m_renderer.reset();
        m_window.reset();
        Locator::getLogger()->info("Game resources destroyed");
    }

    void Game::processEvents()
    {
        SDL_Event event{};
        while (SDL_PollEvent(&event)) {
            m_devGui->processEvent(event);
            if (m_devGui->wantCaptureKeyboard() || m_devGui->wantCaptureMouse()) {
                m_inputHandler->switchScope(m_inputHandler->getDevGuiScopeId());
            } else if (m_inputHandler->getCurrentScopeId() == m_inputHandler->getDevGuiScopeId()) {
                m_inputHandler->switchScope(m_inputHandler->getPreviousScopeId());
            }
            switch (event.type) {
            case SDL_EVENT_QUIT:
                m_isRunning = false;
                break;
            case SDL_EVENT_WINDOW_RESIZED:
                m_renderer->setViewportSize(event.window.data1, event.window.data2);
                break;
            case SDL_EVENT_KEY_DOWN:
                m_inputHandler->handleKeyboardKeyDownEvent(event.key);
                break;
            case SDL_EVENT_KEY_UP:
                m_inputHandler->handleKeyboardKeyUpEvent(event.key);
                break;
            default:
                break;
            }
        }
        m_inputHandler->resolveInput();
    }

    void Game::update() { m_currentScene->update(s_timeStep); }

    void Game::render(float frameExtrapolationTimeStep)
    {
        m_renderer->clear();
        m_currentScene->render(frameExtrapolationTimeStep);
        m_devGui->newFrame();
        if (m_isDevModeEnabled) {
            m_devGui->show();
        }
        m_devGui->render();
        m_renderer->present();
    }
} // namespace Engine
