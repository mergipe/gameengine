#include "Scene.h"
#include "Game.h"
#include "Systems.h"
#include "core/Locator.h"

namespace Engine
{
    Scene::Scene(Renderer2D* renderer, ResourceManager* resourceManager,
                 std::unique_ptr<entt::registry> registry, std::unique_ptr<ScriptingSystem> scriptingSystem,
                 const SceneData& sceneData)
        : m_renderer{renderer}
        , m_resourceManager{resourceManager}
        , m_registry{std::move(registry)}
        , m_scriptingSystem{std::move(scriptingSystem)}
        , m_movementSystem{std::make_unique<MovementSystem>(m_registry.get())}
        , m_renderingSystem{std::make_unique<RenderingSystem>(m_registry.get())}
        , m_animationSystem{std::make_unique<SpriteAnimationSystem>(m_registry.get())}
        , m_collisionSystem{std::make_unique<CollisionSystem>(m_registry.get())}
        , m_playerInputSystem{std::make_unique<PlayerInputSystem>(m_registry.get())}
        , m_sceneData{sceneData}
    {
        if (Game::instance().hasDevMode()) {
            m_debugRenderingSystem = std::make_unique<DebugRenderingSystem>(m_registry.get());
        }
        m_scriptingSystem->start();
    }

    Scene::~Scene()
    {
        m_resourceManager->clear(); // NOTE: maybe we shouldn't do this
        // it's important to destroy the registry before the scripting system
        m_registry.reset();
        m_scriptingSystem.reset();
    }

    void Scene::update(float timeStep)
    {
        Locator::getEventBus()->reset();
        m_movementSystem->update(timeStep);
        m_collisionSystem->update();
        m_animationSystem->update();
        m_scriptingSystem->update(timeStep);
    }

    void Scene::render(float frameExtrapolationTimeStep)
    {
        m_renderingSystem->update(*m_renderer, *m_resourceManager, frameExtrapolationTimeStep);
        if (Game::instance().isDevModeEnabled()) {
            m_debugRenderingSystem->update(*m_renderer, frameExtrapolationTimeStep);
        }
    }
} // namespace Engine
