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
        m_resourceManager = std::make_unique<ResourceManager>();
        m_eventBus = std::make_unique<EventBus>();
        if (m_hasDeveloperMode) {
            m_developerModeGui = std::make_unique<DeveloperModeGui>(*m_window);
        }
        Logger::info("Game initialized");
    }

    void Game::run()
    {
        Logger::info("Game started to run");
        m_isRunning = true;
        m_currentScene = std::make_unique<Scene>(m_renderer.get(), m_resourceManager.get(),
                                                 m_window->getWidth(), m_window->getHeight());
        Timer::Ticks previousTicks{Timer::getTicks()};
        float lag{0.0f};
        while (m_isRunning) {
            const Timer::Ticks currentTicks{Timer::getTicks()};
            lag += static_cast<float>(currentTicks - previousTicks);
            previousTicks = currentTicks;
            processInput();
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
        if (m_hasDeveloperMode) {
            m_developerModeGui.reset();
        }
        m_eventBus.reset();
        m_resourceManager.reset();
        m_renderer.reset();
        m_window.reset();
        Logger::info("Game resources destroyed");
    }

    void Game::processInput()
    {
        SDL_Event event{};
        while (SDL_PollEvent(&event)) {
            if (m_isDeveloperModeEnabled) {
                m_developerModeGui->processEvent(event);
            }
            switch (event.type) {
            case SDL_EVENT_QUIT:
                m_isRunning = false;
                break;
            case SDL_EVENT_KEY_DOWN:
                if (m_hasDeveloperMode && event.key.key == SDLK_F12) {
                    m_isDeveloperModeEnabled = !m_isDeveloperModeEnabled;
                }
                m_eventBus->dispatchEvent<KeyPressedEvent>(event.key.key);
                break;
            }
        }
    }

    void Game::update() { m_currentScene->update(s_timeStepInMs); }

    void Game::render(float frameExtrapolationTimeStep)
    {
        m_currentScene->render(frameExtrapolationTimeStep);
        if (m_isDeveloperModeEnabled) {
            m_developerModeGui->show();
        }
        m_renderer->present();
    }
} // namespace Engine
