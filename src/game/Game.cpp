#include "Game.h"
#include "Components.h"
#include "ECS.h"
#include "Systems.h"
#include "core/IO.h"
#include "core/Logger.h"
#include <SDL.h>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <string>

namespace Engine
{
    Game::Game(bool debugCapability) : m_debugCapability{debugCapability} {}

    void Game::init()
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            Logger::critical("Failed to initialize SDL: {}", SDL_GetError());
            std::abort();
        }
        m_window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    m_windowWidth, m_windowHeight, SDL_WINDOW_BORDERLESS);
        if (!m_window) {
            Logger::critical("Failed to create a SDL window: {}", SDL_GetError());
            std::abort();
        }
        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
        if (!m_renderer) {
            Logger::critical("Failed to create a SDL renderer: {}", SDL_GetError());
            std::abort();
        }
        Logger::trace("Game initialized");
    }

    void Game::loadMap(std::string_view tilesetFilename, std::string_view tilemapFilename, int tileWidth,
                       int tileHeight, int tilesetColumns, float scale)
    {
        Logger::trace("Loading map {}", tilemapFilename);
        const std::string tilemapsFolder{"tilemaps/"};
        const std::string tilesetFilepath{tilemapsFolder + std::string{tilesetFilename}};
        m_resourceManager->addTexture("tileset", std::string{tilesetFilepath});
        const std::string tilemapFilepath{std::string{m_resourceManager->getResourcesBasePath()} +
                                          tilemapsFolder + std::string{tilemapFilename}};
        std::ifstream tilemapFile{tilemapFilepath};
        if (!tilemapFile) {
            Logger::error("Error opening {} file for reading", tilemapFilepath);
        }
        const std::vector<std::vector<int>> tilemap{IO::parseIntCsvFile(tilemapFile)};
        tilemapFile.close();
        for (size_t i{0}; i < tilemap.size(); ++i) {
            const std::vector<int> values{tilemap[i]};
            for (size_t j{0}; j < values.size(); ++j) {
                const Entity tile{m_registry->createEntity()};
                m_registry->addComponent<TransformComponent>(
                    tile,
                    glm::vec2(static_cast<float>(j) * static_cast<float>(tileWidth) * scale,
                              static_cast<float>(i) * static_cast<float>(tileHeight) * scale),
                    glm::vec2(scale, scale));
                const int tileId{values[j]};
                m_registry->addComponent<SpriteComponent>(tile, "tileset", tileWidth, tileHeight, 0,
                                                          tileWidth * (tileId % tilesetColumns),
                                                          tileHeight * (tileId / tilesetColumns));
            }
        }
    }

    void Game::loadEntities()
    {
        m_resourceManager->addTexture("chopper", "images/chopper.png");
        m_resourceManager->addTexture("radar", "images/radar.png");
        const Entity chopper{m_registry->createEntity()};
        m_registry->addComponent<TransformComponent>(chopper, glm::vec2(10, 10));
        m_registry->addComponent<RigidBodyComponent>(chopper, glm::vec2(0.0, 0.0));
        m_registry->addComponent<SpriteComponent>(chopper, "chopper", 32, 32, 1);
        m_registry->addComponent<AnimationComponent>(chopper, 2, 15);
        m_registry->addComponent<BoxColliderComponent>(chopper, 32, 32);
        const Entity radar{m_registry->createEntity()};
        m_registry->addComponent<TransformComponent>(radar, glm::vec2(400, 10));
        m_registry->addComponent<SpriteComponent>(radar, "radar", 64, 64, 2);
        m_registry->addComponent<AnimationComponent>(radar, 8, 5);
        m_resourceManager->addTexture("tank-right", "images/tank-panther-right.png");
        m_resourceManager->addTexture("truck-right", "images/truck-ford-right.png");
        const Entity tank{m_registry->createEntity()};
        m_registry->addComponent<TransformComponent>(tank, glm::vec2(10, 10), glm::vec2(2));
        m_registry->addComponent<RigidBodyComponent>(tank, glm::vec2(0.1, 0.1));
        m_registry->addComponent<SpriteComponent>(tank, "tank-right", 32, 32, 1);
        m_registry->addComponent<BoxColliderComponent>(tank, 32, 32);
        const Entity truck{m_registry->createEntity()};
        m_registry->addComponent<TransformComponent>(truck, glm::vec2(50, 50));
        m_registry->addComponent<RigidBodyComponent>(truck, glm::vec2(0.05, 0.05));
        m_registry->addComponent<SpriteComponent>(truck, "truck-right", 32, 32, 1);
        m_registry->addComponent<BoxColliderComponent>(truck, 32, 32);
        m_registry->killEntity(truck);
    }

    void Game::loadLevel(int level)
    {
        Logger::trace("Loading level {}", level);
        m_registry = std::make_unique<Registry>();
        m_resourceManager = std::make_unique<ResourceManager>(
            "/home/gustavo/workspaces/gamedev/gameengine/resources/", *m_renderer);
        m_registry->addSystem<RenderSystem>(*m_renderer, *m_resourceManager);
        m_registry->addSystem<MovementSystem>();
        m_registry->addSystem<AnimationSystem>();
        m_registry->addSystem<CollisionSystem>();
        if (m_debugCapability) {
            m_registry->addSystem<DebugRenderSystem>(*m_renderer);
        }
        loadMap("jungle.png", "jungle.map", 32, 32, 10, 2.0);
        loadEntities();
    }

    void Game::setup() { loadLevel(1); }

    void Game::run()
    {
        Logger::trace("Game started to run");
        setup();
        std::uint64_t previousTicks{SDL_GetTicks64()};
        float lag{0.0f};
        m_isRunning = true;
        while (m_isRunning) {
            const std::uint64_t currentTicks{SDL_GetTicks64()};
            lag += static_cast<float>(currentTicks - previousTicks);
            previousTicks = currentTicks;
            processInput();
            while (lag >= s_timeStepInMs) {
                update();
                lag -= s_timeStepInMs;
            }
            render(lag);
        }
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
                if (event.key.keysym.sym == SDLK_F1 && m_debugCapability) {
                    m_debugModeActivated = !m_debugModeActivated;
                }
                break;
            }
        }
    }

    void Game::update()
    {
        m_registry->update();
        m_registry->getSystem<MovementSystem>().update(s_timeStepInMs);
        m_registry->getSystem<CollisionSystem>().update();
        m_registry->getSystem<AnimationSystem>().update();
    }

    void Game::render(float frameExtrapolationTimeStep)
    {
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
        SDL_RenderClear(m_renderer);
        m_registry->getSystem<RenderSystem>().update(frameExtrapolationTimeStep);
        if (m_debugModeActivated) {
            m_registry->getSystem<DebugRenderSystem>().update(frameExtrapolationTimeStep);
        }
        SDL_RenderPresent(m_renderer);
    }

    void Game::destroy()
    {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
        SDL_Quit();
        Logger::trace("Game resources destroyed");
    }
} // namespace Engine
