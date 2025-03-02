#include "Scene.h"
#include "Components.h"
#include "Game.h"
#include "Systems.h"
#include "core/IO.h"

namespace Engine
{
    Scene::Scene(Renderer* renderer, ResourceManager* resourceManager, int windowWidth, int windowHeight)
        : m_renderer{renderer}, m_resourceManager{resourceManager}
    {
        m_sceneData.camera = {0, 0, windowWidth, windowHeight};
        loadLevel(1);
    }

    Scene::~Scene() { m_resourceManager->clearResources(); }

    void Scene::update(float timeStep)
    {
        EventBus& eventBus{Game::instance().getEventBus()};
        eventBus.reset();
        m_registry->getSystem<DamageSystem>().subscribeToEvents(eventBus);
        m_registry->getSystem<PlayerInputSystem>().subscribeToEvents(eventBus);
        m_registry->getSystem<ProjectileEmitSystem>().subscribeToEvents(eventBus);
        m_registry->update();
        m_registry->getSystem<MovementSystem>().update(timeStep);
        m_registry->getSystem<CollisionSystem>().update(eventBus);
        m_registry->getSystem<AnimationSystem>().update();
        m_registry->getSystem<ProjectileEmitSystem>().update();
        m_registry->getSystem<LifecycleSystem>().update();
        m_registry->getSystem<CameraMovementSystem>().update(m_sceneData);
    }

    void Scene::render(float frameExtrapolationTimeStep)
    {
        m_renderer->setDrawColor(Color{0, 0, 0, 255});
        m_renderer->clear();
        m_registry->getSystem<SpriteRenderingSystem>().update(*m_renderer, *m_resourceManager,
                                                              m_sceneData.camera, frameExtrapolationTimeStep);
        if (Game::instance().isDebugModeActivated()) {
            m_registry->getSystem<BoxColliderRenderingSystem>().update(*m_renderer, m_sceneData.camera,
                                                                       frameExtrapolationTimeStep);
        }
        m_renderer->present();
    }

    void Scene::loadMap(std::string_view tilesetFilename, std::string_view tilemapFilename, int tileWidth,
                        int tileHeight, int tilesetColumns, float scale)
    {
        Logger::info("Loading map {}", tilemapFilename);
        const std::filesystem::path tilemapsPath{m_resourceManager->getResourcePath(Game::s_tilemapsFolder)};
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
        int scaledTileWidth{static_cast<int>(static_cast<float>(tileWidth) * scale)};
        int scaledTileHeight{static_cast<int>(static_cast<float>(tileHeight) * scale)};
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
                m_registry->addComponent<SpriteComponent>(tile, tilesetId, tileWidth, tileHeight, 0, false,
                                                          tileWidth * (tileId % tilesetColumns),
                                                          tileHeight * (tileId / tilesetColumns));
            }
        }
        m_sceneData.mapWidth = static_cast<int>(tilemap[0].size()) * scaledTileWidth;
        m_sceneData.mapHeight = static_cast<int>(tilemap.size()) * scaledTileHeight;
    }

    void Scene::loadEntities()
    {
        std::filesystem::path texturesPath{m_resourceManager->getResourcePath(Game::s_texturesFolder)};
        m_resourceManager->addTexture("chopper",
                                      m_renderer->loadTexture(texturesPath / "chopper-spritesheet.png"));
        m_resourceManager->addTexture("radar", m_renderer->loadTexture(texturesPath / "radar.png"));
        m_resourceManager->addTexture("tank-right",
                                      m_renderer->loadTexture(texturesPath / "tank-panther-right.png"));
        m_resourceManager->addTexture("truck-right",
                                      m_renderer->loadTexture(texturesPath / "truck-ford-right.png"));
        m_resourceManager->addTexture("bullet", m_renderer->loadTexture(texturesPath / "bullet.png"));
        Entity chopper{m_registry->createEntity()};
        chopper.addComponent<TransformComponent>(glm::vec2(300), glm::vec2(2));
        chopper.addComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
        chopper.addComponent<SpriteComponent>("chopper", 32, 32, 1);
        chopper.addComponent<AnimationComponent>(2, 15);
        chopper.addComponent<BoxColliderComponent>(32, 32);
        chopper.addComponent<PlayerInputComponent>();
        chopper.addComponent<CameraFollowComponent>();
        chopper.addComponent<HealthComponent>();
        chopper.addComponent<ProjectileEmitterComponent>(glm::vec2(0.4, 0.4), 200, 10000, 0, true, false);
        Entity radar{m_registry->createEntity()};
        radar.addComponent<TransformComponent>(glm::vec2(400, 10), glm::vec2(2));
        radar.addComponent<SpriteComponent>("radar", 64, 64, 2, true);
        radar.addComponent<AnimationComponent>(8, 5);
        Entity tank{m_registry->createEntity()};
        tank.addComponent<TransformComponent>(glm::vec2(500, 50), glm::vec2(2));
        tank.addComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
        tank.addComponent<SpriteComponent>("tank-right", 32, 32, 1);
        tank.addComponent<BoxColliderComponent>(32, 32);
        tank.addComponent<ProjectileEmitterComponent>(glm::vec2(0.1, 0), 5000, 3000, 0, false);
        tank.addComponent<HealthComponent>();
        Entity truck{m_registry->createEntity()};
        truck.addComponent<TransformComponent>(glm::vec2(50, 50), glm::vec2(2));
        truck.addComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
        truck.addComponent<SpriteComponent>("truck-right", 32, 32, 1);
        truck.addComponent<BoxColliderComponent>(32, 32);
        truck.addComponent<ProjectileEmitterComponent>(glm::vec2(0, 0.1), 2000, 5000, 0, false);
        truck.addComponent<HealthComponent>();
    }

    void Scene::loadLevel(int level)
    {
        Logger::info("Loading level {}", level);
        m_registry = std::make_unique<Registry>();
        m_registry->addSystem<SpriteRenderingSystem>();
        m_registry->addSystem<MovementSystem>();
        m_registry->addSystem<AnimationSystem>();
        m_registry->addSystem<CollisionSystem>();
        m_registry->addSystem<DamageSystem>();
        m_registry->addSystem<PlayerInputSystem>();
        m_registry->addSystem<ProjectileEmitSystem>();
        m_registry->addSystem<LifecycleSystem>();
        m_registry->addSystem<CameraMovementSystem>();
        if (Game::instance().hasDebugCapability()) {
            m_registry->addSystem<BoxColliderRenderingSystem>();
        }
        loadMap("jungle.png", "jungle.map", 32, 32, 10, 4.0);
        loadEntities();
    }
} // namespace Engine
