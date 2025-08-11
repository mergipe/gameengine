#include "Scene.h"
#include "Game.h"
#include "Systems.h"
#include "core/Locator.h"
#include <utility>

namespace Engine
{
    Scene::Scene(InputHandler* inputHandler, Renderer2D* renderer, std::unique_ptr<entt::registry> registry,
                 std::unique_ptr<ScriptingSystem> scriptingSystem, const SceneData& sceneData)
        : m_inputHandler{inputHandler}
        , m_renderer{renderer}
        , m_registry{std::move(registry)}
        , m_scriptingSystem{std::move(scriptingSystem)}
        , m_physicsSystem{std::make_unique<PhysicsSystem>(m_registry.get())}
        , m_renderingSystem{std::make_unique<RenderingSystem>(m_registry.get())}
        , m_animationSystem{std::make_unique<SpriteAnimationSystem>(m_registry.get())}
        , m_playerInputSystem{std::make_unique<PlayerInputSystem>(m_registry.get())}
        , m_sceneData{sceneData}
    {
        if (Game::instance().hasDevMode()) {
            m_debugRenderingSystem = std::make_unique<DebugRenderingSystem>(m_registry.get());
        }
        m_scriptingSystem->start();
        m_playerInputSystem->start(*m_inputHandler);
        m_physicsSystem->start();
    }

    Scene::~Scene()
    {
        // the destruction order is important
        m_registry.reset();
        m_scriptingSystem.reset();
        Locator::getResourceManager()->clear(); // NOTE: we shouldn't always do this
    }

    void Scene::update(float timeStep)
    {
        EventBus& eventBus{*Locator::getEventBus()};
        eventBus.reset();
        m_playerInputSystem->subscribeToEvents(eventBus);
        m_physicsSystem->update(timeStep);
        m_animationSystem->update();
        m_scriptingSystem->update(timeStep);
    }

    void Scene::render(float frameExtrapolationTimeStep)
    {
        m_renderingSystem->update(*m_renderer, frameExtrapolationTimeStep);
        if (Game::instance().isDevModeEnabled()) {
            m_debugRenderingSystem->update(*m_renderer, frameExtrapolationTimeStep);
        }
    }
} // namespace Engine
