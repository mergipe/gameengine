#include "Scene.h"
#include "Game.h"
#include "LevelLoader.h"
#include "Systems.h"

namespace Engine
{
    Scene::Scene(Renderer2D* renderer, ResourceManager* resourceManager, int windowWidth, int windowHeight)
        : m_renderer{renderer}
        , m_resourceManager{resourceManager}
        , m_registry{std::make_unique<entt::registry>()}
        , m_movementSystem{std::make_unique<MovementSystem>(m_registry.get())}
        , m_spriteRenderingSystem{std::make_unique<SpriteRenderingSystem>(m_registry.get())}
        , m_animationSystem{std::make_unique<SpriteAnimationSystem>(m_registry.get())}
        , m_collisionSystem{std::make_unique<CollisionSystem>(m_registry.get())}
        , m_playerInputSystem{std::make_unique<PlayerInputSystem>(m_registry.get())}
        , m_lifecycleSystem{std::make_unique<LifecycleSystem>(m_registry.get())}
        , m_cameraMovementSystem{std::make_unique<CameraMovementSystem>(m_registry.get())}
        , m_scriptSystem{std::make_unique<ScriptSystem>(m_registry.get())}
    {
        if (Game::instance().hasDeveloperMode()) {
            m_boxColliderRenderingSystem = std::make_unique<BoxColliderRenderingSystem>(m_registry.get());
        }
        LevelLoader levelLoader{};
        m_luaState.open_libraries(sol::lib::base, sol::lib::math, sol::lib::os);
        m_scriptSystem->createScriptBindings(m_luaState);
        std::filesystem::path scriptsFolder{m_resourceManager->getResourcePath("scripts")};
        LevelData levelData{
            levelLoader.loadLevel(m_luaState, scriptsFolder / "level0.lua", *m_registry, *resourceManager)};
        m_sceneData.camera = {glm::vec2{0.0f, 0.0f}, static_cast<float>(windowWidth),
                              static_cast<float>(windowHeight)};
        m_sceneData.levelData = levelData;
    }

    Scene::~Scene() { m_resourceManager->clear(); }

    void Scene::update(float timeStep)
    {
        EventBus& eventBus{Game::instance().getEventBus()};
        eventBus.reset();
        m_playerInputSystem->subscribeToEvents(eventBus);
        m_movementSystem->update(timeStep);
        m_collisionSystem->update();
        m_animationSystem->update();
        m_lifecycleSystem->update();
        m_cameraMovementSystem->update(m_sceneData);
        m_scriptSystem->update(timeStep);
    }

    void Scene::render(float frameExtrapolationTimeStep)
    {
        m_renderer->clear();
        m_spriteRenderingSystem->update(*m_renderer, *m_resourceManager, m_sceneData.camera,
                                        frameExtrapolationTimeStep);
        if (Game::instance().isDeveloperModeEnabled()) {
            m_boxColliderRenderingSystem->update(*m_renderer, m_sceneData.camera, frameExtrapolationTimeStep);
        }
    }
} // namespace Engine
