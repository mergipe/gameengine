#include "Scene.h"
#include "Components.h"
#include "Game.h"
#include "Systems.h"
#include "core/IO.h"

namespace Engine
{
    Scene::Scene(Renderer* renderer, AssetManager* assetManager, int windowWidth, int windowHeight)
        : m_renderer{renderer}, m_assetManager{assetManager}
    {
        m_sceneData.camera = {0, 0, windowWidth, windowHeight};
        loadLevel(1);
    }

    Scene::~Scene() { m_assetManager->clearAssets(); }

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
        m_registry->getSystem<SpriteRenderingSystem>().update(*m_renderer, *m_assetManager,
                                                              m_sceneData.camera, frameExtrapolationTimeStep);
        m_registry->getSystem<TextRenderingSystem>().update(*m_renderer, *m_assetManager, m_sceneData.camera);
        m_registry->getSystem<HealthBarRenderingSystem>().update(
            *m_renderer, m_assetManager->getFont("charriot-15"), m_sceneData.camera);
        if (Game::instance().isDeveloperModeEnabled()) {
            m_registry->getSystem<BoxColliderRenderingSystem>().update(*m_renderer, m_sceneData.camera,
                                                                       frameExtrapolationTimeStep);
            auto& developerModeGui{Game::instance().getDeveloperModeGui()};
            developerModeGui.newFrame();
            developerModeGui.render();
        }
        m_renderer->present();
    }

    void Scene::loadMap(std::string_view tilesetFilename, std::string_view tilemapFilename, float tileWidth,
                        float tileHeight, int tilesetColumns, float scale)
    {
        Logger::info("Loading map {}", tilemapFilename);
        const std::filesystem::path tilemapsPath{m_assetManager->getAssetPath(Game::s_tilemapsFolder)};
        const std::string_view tilesetId{"tileset"};
        m_assetManager->addTexture(tilesetId, m_renderer->loadTexture(tilemapsPath / tilesetFilename));
        const std::filesystem::path tilemapFilepath{
            m_assetManager->getAssetPath(tilemapsPath / tilemapFilename)};
        std::ifstream tilemapFile{tilemapFilepath};
        if (!tilemapFile) {
            Logger::error("Error opening {} file for reading", tilemapFilepath.c_str());
        }
        const std::vector<std::vector<int>> tilemap{IO::parseIntCsvFile(tilemapFile)};
        tilemapFile.close();
        int scaledTileWidth{static_cast<int>(tileWidth * scale)};
        int scaledTileHeight{static_cast<int>(tileHeight * scale)};
        for (size_t i{0}; i < tilemap.size(); ++i) {
            const std::vector<int> values{tilemap[i]};
            for (size_t j{0}; j < values.size(); ++j) {
                Entity tile{m_registry->createEntity()};
                tile.group("tiles");
                m_registry->addComponent<TransformComponent>(
                    tile,
                    glm::vec2{static_cast<float>(j) * tileWidth * scale,
                              static_cast<float>(i) * tileHeight * scale},
                    glm::vec2{scale, scale});
                const int tileId{values[j]};
                m_registry->addComponent<SpriteComponent>(
                    tile, tilesetId, tileWidth, tileHeight, 0, false,
                    tileWidth * static_cast<float>(tileId % tilesetColumns),
                    tileHeight * static_cast<float>(tileId / tilesetColumns));
            }
        }
        m_sceneData.mapWidth = static_cast<int>(tilemap[0].size()) * scaledTileWidth;
        m_sceneData.mapHeight = static_cast<int>(tilemap.size()) * scaledTileHeight;
    }

    void Scene::loadEntities()
    {
        std::filesystem::path texturesPath{m_assetManager->getAssetPath(Game::s_texturesFolder)};
        m_assetManager->addTexture("chopper",
                                   m_renderer->loadTexture(texturesPath / "chopper-spritesheet.png"));
        m_assetManager->addTexture("radar", m_renderer->loadTexture(texturesPath / "radar.png"));
        m_assetManager->addTexture("tank-right",
                                   m_renderer->loadTexture(texturesPath / "tank-panther-right.png"));
        m_assetManager->addTexture("truck-right",
                                   m_renderer->loadTexture(texturesPath / "truck-ford-right.png"));
        m_assetManager->addTexture("bullet", m_renderer->loadTexture(texturesPath / "bullet.png"));
        Entity chopper{m_registry->createEntity()};
        chopper.tag("player");
        chopper.addComponent<TransformComponent>(glm::vec2{300}, glm::vec2{2});
        chopper.addComponent<RigidBodyComponent>(glm::vec2{0.0, 0.0});
        chopper.addComponent<SpriteComponent>("chopper", 32.0f, 32.0f, 1);
        chopper.addComponent<AnimationComponent>(2, 15);
        chopper.addComponent<BoxColliderComponent>(32, 32);
        chopper.addComponent<PlayerInputComponent>();
        chopper.addComponent<CameraFollowComponent>();
        chopper.addComponent<HealthComponent>();
        chopper.addComponent<ProjectileEmitterComponent>(glm::vec2{0.4, 0.4}, 200, 10000, 10, true, false);
        Entity radar{m_registry->createEntity()};
        radar.addComponent<TransformComponent>(glm::vec2{400, 10}, glm::vec2{2});
        radar.addComponent<SpriteComponent>("radar", 64.0f, 64.0f, 2, true);
        radar.addComponent<AnimationComponent>(8, 5);
        Entity tank{m_registry->createEntity()};
        tank.group("enemies");
        tank.addComponent<TransformComponent>(glm::vec2{500, 50}, glm::vec2{2});
        tank.addComponent<RigidBodyComponent>(glm::vec2{0.0, 0.0});
        tank.addComponent<SpriteComponent>("tank-right", 32.0f, 32.0f, 1);
        tank.addComponent<BoxColliderComponent>(32, 32);
        tank.addComponent<ProjectileEmitterComponent>(glm::vec2{0.1, 0}, 5000, 3000, 10, false);
        tank.addComponent<HealthComponent>();
        Entity truck{m_registry->createEntity()};
        truck.group("enemies");
        truck.addComponent<TransformComponent>(glm::vec2{50, 50}, glm::vec2{2});
        truck.addComponent<RigidBodyComponent>(glm::vec2{0.0, 0.0});
        truck.addComponent<SpriteComponent>("truck-right", 32.0f, 32.0f, 1);
        truck.addComponent<BoxColliderComponent>(32, 32);
        truck.addComponent<ProjectileEmitterComponent>(glm::vec2{0, 0.1}, 2000, 5000, 10, false);
        truck.addComponent<HealthComponent>();
        Entity label{m_registry->createEntity()};
        label.addComponent<TransformComponent>(glm::vec2{20, 20});
        label.addComponent<TextComponent>("CHOPPER 1.0", "charriot-20", Color{0, 255, 0, 255}, true);
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
        m_registry->addSystem<TextRenderingSystem>();
        m_registry->addSystem<HealthBarRenderingSystem>();
        if (Game::instance().hasDeveloperMode()) {
            m_registry->addSystem<BoxColliderRenderingSystem>();
        }
        std::filesystem::path fontsPath{m_assetManager->getAssetPath(Game::s_fontsFolder)};
        m_assetManager->addFont("charriot-20", fontsPath / "charriot.ttf", 20.0f);
        m_assetManager->addFont("charriot-15", fontsPath / "charriot.ttf", 15.0f);
        loadMap("jungle.png", "jungle.map", 32, 32, 10, 4.0);
        loadEntities();
    }
} // namespace Engine
