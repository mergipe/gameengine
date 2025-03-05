#include "Game.h"
#include "core/Logger.h"
#include "core/Timer.h"
#include "renderer/Renderer.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
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
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
            std::cerr << "Failed to initialize SDL: " << SDL_GetError() << '\n';
            std::abort();
        }
        m_basePath = std::filesystem::canonical(SDL_GetBasePath()).parent_path();
        Logger::init(m_basePath / s_logFilepath);
        if (!TTF_Init()) {
            Logger::error("Failed to initialize SDL_ttf: {}", SDL_GetError());
        }
        m_window = std::make_unique<Window>(s_windowTitle, s_windowWidth, s_windowHeight);
        m_renderer = std::make_unique<Renderer>(*m_window);
        m_assetManager = std::make_unique<AssetManager>(m_basePath / s_assetsFolder);
        m_eventBus = std::make_unique<EventBus>();
        if (m_hasDeveloperMode) {
            m_developerModeGui = std::make_unique<DeveloperModeGui>(*m_window, m_renderer.get());
        }
        Logger::info("Game initialized");
    }

    void Game::run()
    {
        Logger::info("Game started to run");
        m_isRunning = true;
        m_currentScene = std::make_unique<Scene>(m_renderer.get(), m_assetManager.get(), m_window->getWidth(),
                                                 m_window->getHeight());
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
        if (m_hasDeveloperMode) {
            m_developerModeGui.reset();
        }
        m_eventBus.reset();
        m_assetManager.reset();
        m_renderer.reset();
        m_window.reset();
        TTF_Quit();
        SDL_Quit();
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
    }
} // namespace Engine
