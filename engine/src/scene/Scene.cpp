#include "Scene.h"

#include "Components.h"
#include "Engine.h"
#include "core/Locator.h"
#include "core/Math.h"

namespace Engine
{
    Scene::Scene(entt::registry* registry)
        : m_registry{registry}
    {
    }

    Scene::~Scene()
    {
        Locator::GetResourceManager()->Clear(); // TODO: improve resource management between scenes
    }

    void Scene::Start()
    {
        StartScripts();
        StartPlayerInput();
        StartPhysics2D();
    }

    void Scene::Update(float timeStep)
    {
        auto* eventBus{Locator::GetEventBus()};
        eventBus->Reset();
        UpdatePlayerInput(eventBus);
        UpdatePhysics2D(timeStep);
        UpdateScripts(timeStep);
    }

    void Scene::Render(float frameExtrapolationTimeStep)
    {
        const auto cameraView{m_registry->view<const TransformComponent, CameraComponent>()};
        Camera* cameraPtr{};
        for (const auto entity : cameraView) {
            auto [transform, camera] = cameraView.get<TransformComponent, CameraComponent>(entity);
            cameraPtr = &camera.camera;
            if (!cameraPtr)
                continue;
            cameraPtr->SetTransform(transform.position, transform.rotation, transform.scale);
            break;
        }
        if (!cameraPtr)
            return;
        Locator::GetRenderManager()->SetCamera(*cameraPtr);
        m_registry->sort<SpriteComponent>(
            [](const SpriteComponent& lhs, const SpriteComponent& rhs) { return lhs.zIndex < rhs.zIndex; });
        auto spriteView{m_registry->view<const TransformComponent, const SpriteComponent>()};
        spriteView.use<SpriteComponent>();
        for (const auto entity : spriteView) {
            auto [transform, spriteComponent] = spriteView.get<TransformComponent, SpriteComponent>(entity);
            glm::vec2 renderPosition{transform.position};
            if (const auto* rigidBody{m_registry->try_get<RigidBody2DRuntimeComponent>(entity)}) {
                const auto velocity{Locator::GetPhysicsEngine2D()->GetLinearVelocity(rigidBody->bodyId)};
                renderPosition = Math::CalculateExtrapolatedPosition(transform.position, velocity,
                                                                     frameExtrapolationTimeStep);
            }
            const Texture2D* texture{(&Locator::GetResourceManager()->GetTexture(spriteComponent.textureId))};
            glm::vec2 subTextureUvTopLeft{0.0f, 0.0f};
            glm::vec2 subTextureSize{};
            if (!spriteComponent.spriteId.GetString().empty()) {
                if (const auto sprite{texture->GetSprite(spriteComponent.spriteId)}) {
                    subTextureUvTopLeft = sprite->uvTopLeft;
                    subTextureSize = sprite->size;
                }
            } else {
                subTextureSize.x = static_cast<float>(texture->GetWidth());
                subTextureSize.y = static_cast<float>(texture->GetHeight());
            }
            const float spriteWidth{subTextureSize.x * transform.scale.x};
            const float spriteHeight{subTextureSize.y * transform.scale.y};
            Locator::GetRenderManager()->AddSprite(glm::vec3{renderPosition, 0.0f}, transform.rotation,
                                                   glm::vec3{spriteWidth, spriteHeight, 1.0f},
                                                   spriteComponent.pivotPoint, texture, subTextureUvTopLeft,
                                                   subTextureSize, spriteComponent.color);
            if (Engine::Instance().IsDevModeEnabled()) {
                DebugRenderer* debugRenderer{Locator::GetDebugRenderer()};
                debugRenderer->AddTransformAxes(renderPosition, transform.rotation.z, spriteWidth * 0.75f);
            }
        }
    }

    void Scene::OnViewportResize(int width, int height)
    {
        const auto view{m_registry->view<CameraComponent>()};
        for (const auto entity : view) {
            auto& cameraComponent{view.get<CameraComponent>(entity)};
            cameraComponent.camera.SetViewport(static_cast<float>(width), static_cast<float>(height));
        }
    }

    void Scene::StartPlayerInput()
    {
        const auto view{m_registry->view<PlayerInputComponent>()};
        const StringId* defaultInputScope{};
        for (const auto entity : view) {
            auto& playerInput{view.get<PlayerInputComponent>(entity)};
            if (!defaultInputScope) {
                defaultInputScope = &playerInput.defaultInputScope;
            }
            auto& playerInputRuntime{m_registry->get_or_emplace<PlayerInputRuntimeComponent>(entity)};
            playerInputRuntime.inputDeviceId = Locator::GetInputManager()->AcquireAvailableDevice();
            std::unordered_map<StringId, ScriptInstance*> scriptInstances{};
            auto* scriptComponent = m_registry->try_get<ScriptRuntimeComponent>(entity);
            if (!scriptComponent) {
                continue;
            }
            for (auto& scriptInstance : scriptComponent->scriptInstances) {
                scriptInstances.insert(std::make_pair(scriptInstance.GetClassId(), &scriptInstance));
            }
            for (const auto& [commandId, callbackDef] : playerInput.callbackDefs) {
                if (auto it{scriptInstances.find(callbackDef.scriptId)}; it != scriptInstances.end()) {
                    ScriptInstance* scriptInstance{it->second};
                    playerInputRuntime.callbackMapping.SetCommandCallback(
                        commandId, InputCallback{std::function<void(InputValue)>{
                                       [scriptInstance, callbackDef](InputValue inputValue) {
                                           scriptInstance->InvokeFunction(callbackDef.callbackName,
                                                                          inputValue);
                                       }}});
                }
            }
        }
        if (defaultInputScope) {
            Locator::GetInputManager()->SwitchScope(*defaultInputScope);
        }
    }

    void Scene::StartPhysics2D()
    {
        const auto view{m_registry->view<const TransformComponent, const RigidBody2DComponent>(
            entt::exclude<RigidBody2DRuntimeComponent>)};
        auto physicsEngine{Locator::GetPhysicsEngine2D()};
        for (const auto entity : view) {
            auto [transform, rigidBody] = view.get<TransformComponent, RigidBody2DComponent>(entity);
            const auto bodyId = physicsEngine->CreateBody(
                rigidBody.bodyData, glm::vec2{transform.position.x, transform.position.y},
                transform.rotation.z, entity);
            auto& rigidBodyRuntime{m_registry->emplace<RigidBody2DRuntimeComponent>(entity, bodyId)};
            bool hasCollider{false};
            if (const auto* boxCollider{m_registry->try_get<BoxCollider2DComponent>(entity)}) {
                physicsEngine->CreateBoxShape(
                    rigidBodyRuntime.bodyId, boxCollider->shapeData, boxCollider->width * transform.scale.x,
                    boxCollider->height * transform.scale.y, boxCollider->edgeRadius, boxCollider->offset,
                    boxCollider->rotation);
                hasCollider = true;
            }
            if (const auto* circleCollider{m_registry->try_get<CircleCollider2DComponent>(entity)}) {
                physicsEngine->CreateCircleShape(rigidBodyRuntime.bodyId, circleCollider->shapeData,
                                                 circleCollider->radius *
                                                     std::max(transform.scale.x, transform.scale.y),
                                                 circleCollider->offset);
                hasCollider = true;
            }
            if (!hasCollider) {
                physicsEngine->CreateDefaultShape(rigidBodyRuntime.bodyId);
            }
        }
    }

    void Scene::StartScripts()
    {
        const auto view{m_registry->view<ScriptComponent>()};
        for (const auto entity : view) {
            auto& scriptComponent = view.get<ScriptComponent>(entity);
            for (auto& scriptData : scriptComponent.scriptDatas) {
                std::optional scriptInstance{Locator::GetScriptSystem()->CreateScriptInstance(
                    scriptData, entt::handle{*m_registry, entity})};
                if (scriptInstance) {
                    auto& scriptRuntime{m_registry->get_or_emplace<ScriptRuntimeComponent>(entity)};
                    scriptRuntime.scriptInstances.push_back(*scriptInstance);
                    scriptInstance->InvokeOnStart();
                }
            }
        }
    }

    void Scene::OnInputCommand(const InputEvent& event)
    {
        const auto view{m_registry->view<const PlayerInputRuntimeComponent>()};
        for (const auto entity : view) {
            const auto& playerInput{view.get<PlayerInputRuntimeComponent>(entity)};
            if (event.inputDeviceId == playerInput.inputDeviceId) {
                const InputCallback* callback{
                    playerInput.callbackMapping.GetCommandCallback(event.commandId)};
                if (callback) {
                    callback->Execute(event.inputValue);
                }
            }
        }
    }

    void Scene::UpdatePlayerInput(EventBus* eventBus)
    {
        eventBus->AddSubscriber<InputEvent, Scene>(this, &Scene::OnInputCommand);
    }

    void Scene::UpdatePhysics2D(float timeStep)
    {
        const auto physicsEngine{Locator::GetPhysicsEngine2D()};
        const auto view{m_registry->view<TransformComponent, RigidBody2DRuntimeComponent>()};
        for (const auto entity : view) {
            auto [transform, rigidBody] = view.get<TransformComponent, RigidBody2DRuntimeComponent>(entity);
            const auto position{glm::vec2{transform.position}};
            const auto rotation{transform.rotation.z};
            if (physicsEngine->GetPosition(rigidBody.bodyId) != position ||
                physicsEngine->GetRotationAngle(rigidBody.bodyId) != rotation) {
                physicsEngine->SetTransform(rigidBody.bodyId, position, rotation);
            }
        }
        physicsEngine->Update(timeStep);
        b2BodyEvents events{physicsEngine->GetBodyEvents()};
        for (int i{0}; i < events.moveCount; ++i) {
            const b2BodyMoveEvent* event{events.moveEvents + i};
            entt::entity entity{static_cast<entt::entity>(reinterpret_cast<std::uintptr_t>(event->userData))};
            if (auto* transform{m_registry->try_get<TransformComponent>(entity)}) {
                transform->position.x = event->transform.p.x;
                transform->position.y = event->transform.p.y;
                transform->rotation.z = b2Rot_GetAngle(event->transform.q);
            }
        }
    }

    void Scene::UpdateScripts(float timeStep)
    {
        const auto view{m_registry->view<ScriptRuntimeComponent>()};
        for (const auto entity : view) {
            auto& scriptComponent = view.get<ScriptRuntimeComponent>(entity);
            for (auto& script : scriptComponent.scriptInstances) {
                script.InvokeOnUpdate(timeStep);
            }
        }
    }
} // namespace Engine
