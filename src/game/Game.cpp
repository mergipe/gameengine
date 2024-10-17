#include "Game.h"
#include "Components.h"
#include "ECS.h"
#include "SDL_filesystem.h"
#include "Systems.h"
#include "core/IO.h"
#include "core/Logger.h"
#include <SDL.h>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>

namespace Engine
{
    Game::Game(bool debugCapability) : m_debugCapability{debugCapability} {}

    void Game::init()
    {
        Logger::init();
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
            Logger::critical("Failed to initialize SDL: {}", SDL_GetError());
            std::abort();
        }
        m_basePath = std::filesystem::canonical(SDL_GetBasePath()).parent_path();
        Logger::addFileSink(m_basePath / "logs" / "log.txt");
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
        m_resourceManager = std::make_unique<ResourceManager>(m_basePath / "resources", *m_renderer);
        Logger::info("Game initialized");
    }

    void Game::loadMap(std::string_view tilesetFilename, std::string_view tilemapFilename, int tileWidth,
                       int tileHeight, int tilesetColumns, float scale)
    {
        Logger::info("Loading map {}", tilemapFilename);
        const std::filesystem::path tilemapsPath{"tilemaps"};
        m_resourceManager->addTexture("tileset", tilemapsPath / tilesetFilename);
        const std::filesystem::path tilemapFilepath{
            m_resourceManager->getResourceAbsolutePath(tilemapsPath / tilemapFilename)};
        std::ifstream tilemapFile{tilemapFilepath};
        if (!tilemapFile) {
            Logger::error("Error opening {} file for reading", tilemapFilepath.c_str());
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
        std::filesystem::path texturesPath{"textures"};
        m_resourceManager->addTexture("chopper", texturesPath / "chopper.png");
        m_resourceManager->addTexture("radar", texturesPath / "radar.png");
        m_resourceManager->addTexture("tank-right", texturesPath / "tank-panther-right.png");
        m_resourceManager->addTexture("truck-right", texturesPath / "truck-ford-right.png");
        Entity chopper{m_registry->createEntity()};
        chopper.addComponent<TransformComponent>(glm::vec2(300));
        chopper.addComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
        chopper.addComponent<SpriteComponent>("chopper", 32, 32, 1);
        chopper.addComponent<AnimationComponent>(2, 15);
        chopper.addComponent<BoxColliderComponent>(32, 32);
        Entity radar{m_registry->createEntity()};
        radar.addComponent<TransformComponent>(glm::vec2(400, 10));
        radar.addComponent<SpriteComponent>("radar", 64, 64, 2);
        radar.addComponent<AnimationComponent>(8, 5);
        Entity tank{m_registry->createEntity()};
        tank.addComponent<TransformComponent>(glm::vec2(10), glm::vec2(2));
        tank.addComponent<RigidBodyComponent>(glm::vec2(0.1, 0.1));
        tank.addComponent<SpriteComponent>("tank-right", 32, 32, 1);
        tank.addComponent<BoxColliderComponent>(32, 32);
        Entity truck{m_registry->createEntity()};
        truck.addComponent<TransformComponent>(glm::vec2(50, 50));
        truck.addComponent<RigidBodyComponent>(glm::vec2(0.05, 0.05));
        truck.addComponent<SpriteComponent>("truck-right", 32, 32, 1);
        truck.addComponent<BoxColliderComponent>(32, 32);
        truck.kill();
    }

    void Game::loadLevel(int level)
    {
        Logger::info("Loading level {}", level);
        m_registry = std::make_unique<Registry>();
        m_registry->addSystem<SpriteRenderingSystem>(*m_renderer, *m_resourceManager);
        m_registry->addSystem<MovementSystem>();
        m_registry->addSystem<AnimationSystem>();
        m_registry->addSystem<CollisionSystem>();
        if (m_debugCapability) {
            m_registry->addSystem<BoxColliderRenderingSystem>(*m_renderer);
        }
        loadMap("jungle.png", "jungle.map", 32, 32, 10, 3.0);
        loadEntities();
    }

    void Game::setup() { loadLevel(1); }

    void Game::run()
    {
        Logger::info("Game started to run");
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
        m_registry->getSystem<SpriteRenderingSystem>().update(frameExtrapolationTimeStep);
        if (m_debugModeActivated) {
            m_registry->getSystem<BoxColliderRenderingSystem>().update(frameExtrapolationTimeStep);
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
        Logger::info("Game resources destroyed");
    }
} // namespace Engine
