#include "Scene.h"
#include "Components.h"
#include "Game.h"
#include "LevelLoader.h"
#include "Systems.h"
#include "core/IO.h"
#include "core/Logger.h"

namespace Engine
{
    Scene::Scene(Renderer* renderer, AssetManager* assetManager, int windowWidth, int windowHeight)
        : m_renderer{renderer}
        , m_assetManager{assetManager}
        , m_registry{std::make_unique<entt::registry>()}
        , m_movementSystem{std::make_unique<MovementSystem>(m_registry.get())}
        , m_spriteRenderingSystem{std::make_unique<SpriteRenderingSystem>(m_registry.get())}
        , m_animationSystem{std::make_unique<AnimationSystem>(m_registry.get())}
        , m_collisionSystem{std::make_unique<CollisionSystem>(m_registry.get())}
        , m_damageSystem{std::make_unique<DamageSystem>(m_registry.get())}
        , m_playerInputSystem{std::make_unique<PlayerInputSystem>(m_registry.get())}
        , m_projectileEmitSystem{std::make_unique<ProjectileEmitSystem>(m_registry.get())}
        , m_lifecycleSystem{std::make_unique<LifecycleSystem>(m_registry.get())}
        , m_cameraMovementSystem{std::make_unique<CameraMovementSystem>(m_registry.get())}
        , m_healthBarRenderingSystem{std::make_unique<HealthBarRenderingSystem>(m_registry.get())}
        , m_textRenderingSystem{std::make_unique<TextRenderingSystem>(m_registry.get())}
    {
        if (Game::instance().hasDeveloperMode()) {
            m_boxColliderRenderingSystem = std::make_unique<BoxColliderRenderingSystem>(m_registry.get());
        }
        m_sceneData.camera = {0, 0, windowWidth, windowHeight};
        LevelLoader levelLoader{};
        std::filesystem::path scriptsFolder{m_assetManager->getAssetPath(Game::s_scriptsFolder)};
        levelLoader.loadLevel(scriptsFolder / "level1.lua", *m_registry, *m_assetManager, *m_renderer);
    }

    Scene::~Scene() { m_assetManager->clearAssets(); }

    void Scene::update(float timeStep)
    {
        EventBus& eventBus{Game::instance().getEventBus()};
        eventBus.reset();
        m_damageSystem->subscribeToEvents(eventBus);
        m_playerInputSystem->subscribeToEvents(eventBus);
        m_projectileEmitSystem->subscribeToEvents(eventBus);
        m_movementSystem->update(timeStep, m_sceneData);
        m_collisionSystem->update();
        m_animationSystem->update();
        m_projectileEmitSystem->update();
        m_lifecycleSystem->update();
        m_cameraMovementSystem->update(m_sceneData);
    }

    void Scene::render(float frameExtrapolationTimeStep)
    {
        m_renderer->setDrawColor(Color{0, 0, 0, 255});
        m_renderer->clear();
        m_spriteRenderingSystem->update(*m_renderer, *m_assetManager, m_sceneData.camera,
                                        frameExtrapolationTimeStep);
        m_textRenderingSystem->update(*m_renderer, *m_assetManager, m_sceneData.camera);
        m_healthBarRenderingSystem->update(*m_renderer, m_assetManager->getFont("charriot-15"),
                                           m_sceneData.camera);
        if (Game::instance().isDeveloperModeEnabled()) {
            m_boxColliderRenderingSystem->update(*m_renderer, m_sceneData.camera, frameExtrapolationTimeStep);
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
                auto tile{m_registry->create()};
                m_registry->emplace<Tile>(tile);
                m_registry->emplace<TransformComponent>(tile,
                                                        glm::vec2{static_cast<float>(j) * tileWidth * scale,
                                                                  static_cast<float>(i) * tileHeight * scale},
                                                        glm::vec2{scale, scale});
                const int tileId{values[j]};
                m_registry->emplace<SpriteComponent>(tile, tilesetId, tileWidth, tileHeight, 0, false,
                                                     tileWidth * static_cast<float>(tileId % tilesetColumns),
                                                     tileHeight *
                                                         static_cast<float>(tileId / tilesetColumns));
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
        auto chopper{m_registry->create()};
        m_registry->emplace<Player>(chopper);
        m_registry->emplace<TransformComponent>(chopper, glm::vec2{300}, glm::vec2{2});
        m_registry->emplace<RigidBodyComponent>(chopper, glm::vec2{0.0, 0.0});
        m_registry->emplace<SpriteComponent>(chopper, "chopper", 32.0f, 32.0f, 1);
        m_registry->emplace<AnimationComponent>(chopper, 2, 15);
        m_registry->emplace<BoxColliderComponent>(chopper, 32, 32);
        m_registry->emplace<PlayerInputComponent>(chopper);
        m_registry->emplace<CameraFollowComponent>(chopper);
        m_registry->emplace<HealthComponent>(chopper);
        m_registry->emplace<ProjectileEmitterComponent>(chopper, glm::vec2{0.4, 0.4}, 200, 10000, 10, true,
                                                        false);
        auto radar{m_registry->create()};
        m_registry->emplace<TransformComponent>(radar, glm::vec2{400, 10}, glm::vec2{2});
        m_registry->emplace<SpriteComponent>(radar, "radar", 64.0f, 64.0f, 2, true);
        m_registry->emplace<AnimationComponent>(radar, 8, 5);
        auto tank{m_registry->create()};
        m_registry->emplace<Enemy>(tank);
        m_registry->emplace<TransformComponent>(tank, glm::vec2{500, 50}, glm::vec2{2});
        m_registry->emplace<RigidBodyComponent>(tank, glm::vec2{0.0, 0.0});
        m_registry->emplace<SpriteComponent>(tank, "tank-right", 32.0f, 32.0f, 1);
        m_registry->emplace<BoxColliderComponent>(tank, 32, 32);
        m_registry->emplace<ProjectileEmitterComponent>(tank, glm::vec2{0.1, 0}, 5000, 3000, 10, false);
        m_registry->emplace<HealthComponent>(tank);
        auto truck{m_registry->create()};
        m_registry->emplace<Enemy>(truck);
        m_registry->emplace<TransformComponent>(truck, glm::vec2{50, 50}, glm::vec2{2});
        m_registry->emplace<RigidBodyComponent>(truck, glm::vec2{0.0, 0.0});
        m_registry->emplace<SpriteComponent>(truck, "truck-right", 32.0f, 32.0f, 1);
        m_registry->emplace<BoxColliderComponent>(truck, 32, 32);
        m_registry->emplace<ProjectileEmitterComponent>(truck, glm::vec2{0, 0.1}, 2000, 5000, 10, false);
        m_registry->emplace<HealthComponent>(truck);
        auto label{m_registry->create()};
        m_registry->emplace<TransformComponent>(label, glm::vec2{20, 20});
        m_registry->emplace<TextComponent>(label, "CHOPPER 1.0", "charriot-20", Color{0, 255, 0, 255}, true);
    }

    void Scene::loadLevel()
    {
        Logger::info("Loading level");
        std::filesystem::path fontsPath{m_assetManager->getAssetPath(Game::s_fontsFolder)};
        m_assetManager->addFont("charriot-20", fontsPath / "charriot.ttf", 20.0f);
        m_assetManager->addFont("charriot-15", fontsPath / "charriot.ttf", 15.0f);
        loadMap("jungle.png", "jungle.map", 32, 32, 10, 4.0);
        loadEntities();
    }
} // namespace Engine
