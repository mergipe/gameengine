#include "Scene.h"

#include "Components.h"
#include "Engine.h"
#include "Systems.h"
#include "core/Locator.h"

namespace Engine
{
    Scene::Scene(SceneContext* sceneContext)
        : m_physicsSystem{std::make_unique<PhysicsSystem>(&sceneContext->registry)}
        , m_renderingSystem{std::make_unique<RenderingSystem>(&sceneContext->registry)}
        , m_animationSystem{std::make_unique<SpriteAnimationSystem>(&sceneContext->registry)}
        , m_playerInputSystem{std::make_unique<PlayerInputSystem>(&sceneContext->registry)}
        , m_sceneContext{sceneContext}
    {
        m_sceneContext->scriptingSystem->Start();
        m_playerInputSystem->Start();
        m_physicsSystem->Start();
    }

    Scene::~Scene()
    {
        // the destruction order is important
        m_sceneContext->scriptingSystem.reset();
        Locator::GetResourceManager()->Clear(); // TODO: improve resource management between scenes
    }

    void Scene::Update(float timeStep)
    {
        EventBus& eventBus{*Locator::GetEventBus()};
        eventBus.Reset();
        m_playerInputSystem->SubscribeToEvents(eventBus);
        m_physicsSystem->Update(timeStep);
        m_animationSystem->Update();
        m_sceneContext->scriptingSystem->Update(timeStep);
    }

    void Scene::Render(float frameExtrapolationTimeStep)
    {
        m_renderingSystem->Update(frameExtrapolationTimeStep);
    }

    void Scene::OnViewportResize(int width, int height)
    {
        const auto view{m_sceneContext->registry.view<CameraComponent>()};
        for (const auto entity : view) {
            auto& cameraComponent{view.get<CameraComponent>(entity)};
            cameraComponent.camera.SetViewport(static_cast<float>(width), static_cast<float>(height));
        }
    }
} // namespace Engine
