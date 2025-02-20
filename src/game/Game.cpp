#include "Game.h"
#include "Components.h"
#include "ECS.h"
#include "Systems.h"
#include "core/IO.h"
#include "core/Logger.h"
#include "core/Timer.h"
#include "renderer/Color.h"
#include "renderer/Renderer.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

namespace Engine
{
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
        Logger::info("Game initialized");
    }

    void Game::loadMap(std::string_view tilesetFilename, std::string_view tilemapFilename, int tileWidth,
                       int tileHeight, int tilesetColumns, float scale)
    {
        Logger::info("Loading map {}", tilemapFilename);
        const std::filesystem::path tilemapsPath{m_resourceManager->getResourcePath(s_tilemapsFolder)};
        const std::string_view tilesetId{"tileset"};
        m_resourceManager->addTexture(tilesetId, m_renderer->loadTexture(tilemapsPath / tilesetFilename));
        const std::filesystem::path tilemapFilepath{
            m_resourceManager->getResourcePath(tilemapsPath / tilemapFilename)};
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
                m_registry->addComponent<SpriteComponent>(tile, tilesetId, tileWidth, tileHeight, 0,
                                                          tileWidth * (tileId % tilesetColumns),
                                                          tileHeight * (tileId / tilesetColumns));
            }
        }
    }

    void Game::loadEntities()
    {
        std::filesystem::path texturesPath{m_resourceManager->getResourcePath(s_texturesFolder)};
        m_resourceManager->addTexture("chopper", m_renderer->loadTexture(texturesPath / "chopper.png"));
        m_resourceManager->addTexture("radar", m_renderer->loadTexture(texturesPath / "radar.png"));
        m_resourceManager->addTexture("tank-right",
                                      m_renderer->loadTexture(texturesPath / "tank-panther-right.png"));
        m_resourceManager->addTexture("truck-right",
                                      m_renderer->loadTexture(texturesPath / "truck-ford-right.png"));
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
        m_registry->addSystem<SpriteRenderingSystem>();
        m_registry->addSystem<MovementSystem>();
        m_registry->addSystem<AnimationSystem>();
        m_registry->addSystem<CollisionSystem>();
        if (m_debugCapability) {
            m_registry->addSystem<BoxColliderRenderingSystem>();
        }
        loadMap("jungle.png", "jungle.map", 32, 32, 10, 3.0);
        loadEntities();
    }

    void Game::setup() { loadLevel(1); }

    void Game::run()
    {
        Logger::info("Game started to run");
        setup();
        std::uint64_t previousTicks{Timer::getTicks()};
        float lag{0.0f};
        m_isRunning = true;
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
                if (m_debugCapability && event.key.keysym.sym == SDLK_F1) {
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
        m_renderer->setDrawColor(Color{0, 0, 0, 255});
        m_renderer->clear();
        m_registry->getSystem<SpriteRenderingSystem>().update(*m_renderer, *m_resourceManager,
                                                              frameExtrapolationTimeStep);
        if (m_debugModeActivated) {
            m_registry->getSystem<BoxColliderRenderingSystem>().update(*m_renderer,
                                                                       frameExtrapolationTimeStep);
        }
        m_renderer->present();
    }

    void Game::destroy()
    {
        m_resourceManager.reset();
        m_renderer.reset();
        m_window.reset();
        Logger::info("Game resources destroyed");
        IMG_Quit();
        SDL_Quit();
    }
} // namespace Engine
