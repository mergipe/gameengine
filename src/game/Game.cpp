#include "Game.h"
#include "core/Logger.h"
#include "core/Timer.h"
#include "renderer/Renderer.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

namespace Engine
{
    Game& Game::instance()
    {
        static Game instance{};
        return instance;
    }

    void Game::init()
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
            std::cerr << "Failed to initialize SDL: " << SDL_GetError() << '\n';
            std::abort();
        }
        int flags{IMG_INIT_PNG};
        if (!(IMG_Init(flags) & flags)) {
            Logger::error("Failed to initialize SDL_image: {}", IMG_GetError());
        }
        m_basePath = std::filesystem::canonical(SDL_GetBasePath()).parent_path();
        Logger::init(m_basePath / s_logFilepath);
        m_window = std::make_unique<Window>(s_windowTitle, s_windowWidth, s_windowHeight);
        m_renderer = std::make_unique<Renderer>(*m_window);
        m_resourceManager = std::make_unique<ResourceManager>(m_basePath / s_resourcesFolder);
        m_eventBus = std::make_unique<EventBus>();
        Logger::info("Game initialized");
    }

    void Game::run()
    {
        Logger::info("Game started to run");
        m_isRunning = true;
        m_currentScene = std::make_unique<Scene>(m_renderer.get(), m_resourceManager.get(),
                                                 m_window->getWidth(), m_window->getHeight());
        std::uint64_t previousTicks{Timer::getTicks()};
        float lag{0.0f};
        while (m_isRunning) {
            const std::uint64_t currentTicks{Timer::getTicks()};
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
        m_eventBus.reset();
        m_resourceManager.reset();
        m_renderer.reset();
        m_window.reset();
        Logger::info("Game resources destroyed");
        IMG_Quit();
        SDL_Quit();
    }

    void Game::processInput()
    {
        SDL_Event event{};
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                m_isRunning = false;
                break;
            case SDL_KEYDOWN:
                if (m_hasDebugCapability && event.key.keysym.sym == SDLK_F1) {
                    m_isDebugModeActivated = !m_isDebugModeActivated;
                }
                m_eventBus->dispatchEvent<KeyPressedEvent>(event.key.keysym.sym);
                break;
            }
        }
    }

    void Game::update() { m_currentScene->update(s_timeStepInMs); }

    void Game::render(float frameExtrapolationTimeStep)
    {
        m_currentScene->render(frameExtrapolationTimeStep);
    }
} // namespace Engine
