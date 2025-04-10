#include "Scene.h"
#include "Game.h"
#include "SceneLoader.h"
#include "Systems.h"

namespace Engine
{
    Scene::Scene(Renderer2D* renderer, ResourceManager* resourceManager)
        : m_renderer{renderer}
        , m_resourceManager{resourceManager}
        , m_registry{std::make_unique<entt::registry>()}
        , m_movementSystem{std::make_unique<MovementSystem>(m_registry.get())}
        , m_renderingSystem{std::make_unique<RenderingSystem>(m_registry.get())}
        , m_animationSystem{std::make_unique<SpriteAnimationSystem>(m_registry.get())}
        , m_collisionSystem{std::make_unique<CollisionSystem>(m_registry.get())}
        , m_playerInputSystem{std::make_unique<PlayerInputSystem>(m_registry.get())}
        , m_scriptingSystem{std::make_unique<ScriptingSystem>(m_registry.get())}
    {
        if (Game::instance().hasDevMode()) {
            m_debugRenderingSystem = std::make_unique<DebugRenderingSystem>(m_registry.get());
        }
        SceneLoader sceneLoader{};
        std::filesystem::path scenesFolder{m_resourceManager->getResourcePath("scenes")};
        m_sceneData = sceneLoader.load(scenesFolder / "scene1.lua", *m_registry, *m_resourceManager,
                                       *m_scriptingSystem);
        m_scriptingSystem->start();
    }

    Scene::~Scene()
    {
        m_resourceManager->clear();
        m_registry.reset();
        m_scriptingSystem.reset();
    }

    void Scene::update(float timeStep)
    {
        EventBus& eventBus{Game::instance().getEventBus()};
        eventBus.reset();
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
