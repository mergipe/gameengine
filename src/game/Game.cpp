#include "Game.h"
#include "core/Config.h"
#include "core/Filesystem.h"
#include "core/Logger.h"
#include "core/Timer.h"
#include "renderer/Renderer2D.h"
#include <SDL3/SDL.h>

namespace Engine
{
    Game& Game::instance()
    {
        static Game instance{};
        return instance;
    }

    void Game::init()
    {
        Logger::init(Filesystem::getLogsPath() / "log.txt");
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
        m_window = std::make_unique<Window>(Config::loadWindowConfig());
        m_window->init();
        m_renderer = std::make_unique<Renderer2D>(m_window.get());
        m_renderer->init();
        m_inputHandler = std::make_unique<InputHandler>();
        m_resourceManager = std::make_unique<ResourceManager>();
        m_eventBus = std::make_unique<EventBus>();
        if (m_hasDevMode) {
            m_devGui = std::make_unique<DevGui>(*m_window);
            m_inputHandler->bindKeyEventCommand(s_toggleDevModeKey, [this]() { toggleDevMode(); }, false);
        }
        Logger::info("Game initialized");
    }

    void Game::run()
    {
        Logger::info("Game started to run");
        m_isRunning = true;
        m_currentScene = std::make_unique<Scene>(m_renderer.get(), m_resourceManager.get());
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
            render(lag);
        }
        m_currentScene.reset();
    }

    void Game::shutDown()
    {
        if (m_devGui) {
            m_devGui.reset();
        }
        m_eventBus.reset();
        m_resourceManager.reset();
        m_inputHandler.reset();
        m_renderer.reset();
        m_window.reset();
        Logger::info("Game resources destroyed");
    }

    void Game::processEvents()
    {
        SDL_Event event{};
        while (SDL_PollEvent(&event)) {
            if (m_devGui) {
                m_devGui->processEvent(event);
            }
            switch (event.type) {
            case SDL_EVENT_QUIT:
                m_isRunning = false;
                break;
            case SDL_EVENT_KEY_DOWN:
                if (m_devGui && m_devGui->wantCaptureKeyboard())
                    break;
                m_inputHandler->handleKeyEvent(event.key);
                break;
            }
        }
        if (!m_devGui || !!m_devGui->wantCaptureKeyboard())
            m_inputHandler->handleInputState();
    }

    void Game::update() { m_currentScene->update(s_timeStepInMs); }

    void Game::render(float frameExtrapolationTimeStep)
    {
        m_renderer->clear();
        m_currentScene->render(frameExtrapolationTimeStep);
        if (m_devGui) {
            m_devGui->newFrame();
            if (m_isDevModeEnabled) {
                m_devGui->show();
            }
            m_devGui->render();
        }
        m_renderer->present();
    }
} // namespace Engine
