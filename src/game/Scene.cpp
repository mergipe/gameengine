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
        if (Game::Instance().HasDevMode()) {
            m_debugRenderingSystem = std::make_unique<DebugRenderingSystem>(m_registry.get());
        }
        m_scriptingSystem->Start();
        m_playerInputSystem->Start(*m_inputHandler);
        m_physicsSystem->Start();
    }

    Scene::~Scene()
    {
        // the destruction order is important
        m_registry.reset();
        m_scriptingSystem.reset();
        Locator::GetResourceManager()->Clear(); // NOTE: we shouldn't always do this
    }

    void Scene::Update(float timeStep)
    {
        EventBus& eventBus{*Locator::GetEventBus()};
        eventBus.Reset();
        m_playerInputSystem->SubscribeToEvents(eventBus);
        m_physicsSystem->Update(timeStep);
        m_animationSystem->Update();
        m_scriptingSystem->Update(timeStep);
    }

    void Scene::Render(float frameExtrapolationTimeStep)
    {
        m_renderingSystem->Update(*m_renderer, frameExtrapolationTimeStep);
        if (Game::Instance().IsDevModeEnabled()) {
            m_debugRenderingSystem->Update(*m_renderer, frameExtrapolationTimeStep);
        }
    }
} // namespace Engine
