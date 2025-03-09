#include "Scene.h"
#include "Game.h"
#include "LevelLoader.h"
#include "Systems.h"

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
        , m_scriptSystem{std::make_unique<ScriptSystem>(m_registry.get())}
    {
        if (Game::instance().hasDeveloperMode()) {
            m_boxColliderRenderingSystem = std::make_unique<BoxColliderRenderingSystem>(m_registry.get());
        }
        std::filesystem::path scriptsFolder{m_assetManager->getAssetPath(Game::s_scriptsFolder)};
        LevelLoader levelLoader{};
        m_luaState.open_libraries(sol::lib::base, sol::lib::math, sol::lib::os);
        m_scriptSystem->createScriptBindings(m_luaState);
        LevelData levelData{levelLoader.loadLevel(m_luaState, scriptsFolder / "level2.lua", *m_registry,
                                                  *m_assetManager, *m_renderer)};
        m_sceneData.camera = {0, 0, windowWidth, windowHeight};
        m_sceneData.levelData = levelData;
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
        m_scriptSystem->update(timeStep);
    }

    void Scene::render(float frameExtrapolationTimeStep)
    {
        m_renderer->setDrawColor(Color{0, 0, 0, 255});
        m_renderer->clear();
        m_spriteRenderingSystem->update(*m_renderer, *m_assetManager, m_sceneData.camera,
                                        frameExtrapolationTimeStep);
        m_textRenderingSystem->update(*m_renderer, *m_assetManager, m_sceneData.camera);
        m_healthBarRenderingSystem->update(*m_renderer, m_assetManager->getFont("pico8-font-10"),
                                           m_sceneData.camera);
        if (Game::instance().isDeveloperModeEnabled()) {
            m_boxColliderRenderingSystem->update(*m_renderer, m_sceneData.camera, frameExtrapolationTimeStep);
            auto& developerModeGui{Game::instance().getDeveloperModeGui()};
            developerModeGui.newFrame();
            developerModeGui.render();
        }
        m_renderer->present();
    }
} // namespace Engine
