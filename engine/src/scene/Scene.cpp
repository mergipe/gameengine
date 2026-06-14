#include "Scene.h"

#include "Components.h"
#include "ECSUtils.h"
#include "Engine.h"
#include "core/Assert.h"
#include "core/Locator.h"
#include "core/Math.h"

namespace Engine
{
    void OnCollisionEnter(const Entity& entity, Entity& other, const CollisionManifold2D& manifold,
                          Shape2DId otherShapeId)
    {
        if (entity.HasComponent<ScriptRuntimeComponent>()) {
            for (auto& script : entity.GetComponent<ScriptRuntimeComponent>().scriptInstances) {
                script.InvokeOnCollisionEnter(other, manifold, otherShapeId);
            }
        }
    }

    void OnCollisionExit(const Entity& entity, Entity& other, Shape2DId otherShapeId)
    {
        if (entity.HasComponent<ScriptRuntimeComponent>()) {
            for (auto& script : entity.GetComponent<ScriptRuntimeComponent>().scriptInstances) {
                script.InvokeOnCollisionExit(other, otherShapeId);
            }
        }
    }

    void OnTriggerEnter(const Entity& entity, Entity& other, Shape2DId otherShapeId)
    {
        if (entity.HasComponent<ScriptRuntimeComponent>()) {
            for (auto& script : entity.GetComponent<ScriptRuntimeComponent>().scriptInstances) {
                script.InvokeOnTriggerEnter(other, otherShapeId);
            }
        }
    }

    void OnTriggerExit(const Entity& entity, Entity& other, Shape2DId otherShapeId)
    {
        if (entity.HasComponent<ScriptRuntimeComponent>()) {
            for (auto& script : entity.GetComponent<ScriptRuntimeComponent>().scriptInstances) {
                script.InvokeOnTriggerExit(other, otherShapeId);
            }
        }
    }

    Scene::Scene(entt::registry* registry)
        : m_mainRegistry{registry}
    {
        ECSUtils::CreateStorages(AllComponentTypes{}, m_stagingRegistry);
    }

    Scene::~Scene()
    {
        Locator::GetResourceManager()->Clear(); // TODO: improve resource management between scenes
    }

    void Scene::Start()
    {
        const auto view{m_mainRegistry->view<const IdComponent>()};
        for (const auto entity : view) {
            const auto& id{view.get<IdComponent>(entity).value};
            m_entityById.try_emplace(id, entt::handle{*m_mainRegistry, entity}, this);
        }

        m_mainRegistry->on_construct<RigidBody2DComponent>().connect<&Scene::OnAddRigidBody2DComponent>(this);
        m_mainRegistry->on_construct<BoxCollider2DComponent>().connect<&Scene::OnAddBoxCollider2DComponent>(
            this);
        m_mainRegistry->on_construct<CircleCollider2DComponent>()
            .connect<&Scene::OnAddCircleCollider2DComponent>(this);
        m_mainRegistry->on_destroy<RigidBody2DComponent>().connect<&Scene::OnRemoveRigidBody2DComponent>(
            this);
        m_mainRegistry->on_destroy<BoxCollider2DComponent>().connect<&Scene::OnRemoveBoxCollider2DComponent>(
            this);
        m_mainRegistry->on_destroy<CircleCollider2DComponent>()
            .connect<&Scene::OnRemoveCircleCollider2DComponent>(this);

        StartPhysics2D();
        StartScripts();
        StartPlayerInput();
    }

    void Scene::Update(float timeStep)
    {
        CreateEntities();
        AddComponents();
        auto* eventBus{Locator::GetEventBus()};
        eventBus->Reset();
        UpdatePlayerInput(eventBus);
        UpdatePhysics2D(timeStep);
        UpdateScripts(timeStep);
        RemoveComponents();
        DestroyEntities();
    }

    void Scene::Render(float frameExtrapolationTimeStep)
    {
        const auto cameraView{m_mainRegistry->view<const TransformComponent, CameraComponent>()};
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
        m_mainRegistry->sort<SpriteComponent>(
            [](const SpriteComponent& lhs, const SpriteComponent& rhs) { return lhs.zIndex < rhs.zIndex; });
        auto spriteView{m_mainRegistry->view<const TransformComponent, const SpriteComponent>()};
        spriteView.use<SpriteComponent>();
        for (const auto entity : spriteView) {
            auto [transform, spriteComponent] = spriteView.get<TransformComponent, SpriteComponent>(entity);
            glm::vec2 renderPosition{transform.position};
            if (const auto* rigidBody{m_mainRegistry->try_get<RigidBody2DComponent>(entity)}) {
                ASSERT(rigidBody->bodyId);
                const auto velocity{
                    Locator::GetPhysicsEngine2D()->GetLinearVelocity(rigidBody->bodyId.value())};
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
        const auto view{m_mainRegistry->view<CameraComponent>()};
        for (const auto entity : view) {
            auto& cameraComponent{view.get<CameraComponent>(entity)};
            cameraComponent.camera.SetViewport(static_cast<float>(width), static_cast<float>(height));
        }
    }

    Entity& Scene::CreateEntity() { return CreateEntity(entt::handle{}); }

    Entity& Scene::CreateEntity(entt::handle entityTemplate)
    {
        auto newEntityHandle{entt::handle{m_stagingRegistry, m_stagingRegistry.create()}};
        if (entityTemplate.valid()) {
            ECSUtils::CopyEntity(entityTemplate, newEntityHandle);
        }
        static U32 s_nextId{0};
        auto& entityId{newEntityHandle.get<IdComponent>().value};
        entityId = StringId::Intern(entityId.GetString().data() + std::to_string(s_nextId++));
        auto& newEntity{m_entityById.try_emplace(entityId, newEntityHandle, this).first->second};
        m_entitiesToCreate.push_back(&newEntity);
        return newEntity;
    }

    void Scene::DestroyEntity(const Entity& entity) { m_entitiesToDestroy.push_back(entity); }

    void Scene::CreateEntities()
    {
        for (auto& entityToCreate : m_entitiesToCreate) {
            const auto newEntity{m_mainRegistry->create()};
            const auto newHandle{entt::handle{*m_mainRegistry, newEntity}};
            auto oldHandle{entityToCreate->GetHandle()};
            ECSUtils::CopyEntity(oldHandle, newHandle);
            entityToCreate->SetHandle(newHandle);
            oldHandle.destroy();
        }
        m_entitiesToCreate.clear();
    }

    void Scene::DestroyEntities()
    {
        for (const auto entity : m_entitiesToDestroy) {
            m_entityById.erase(entity.GetId());
            m_mainRegistry->destroy(entity.GetHandle());
        }
        m_entitiesToDestroy.clear();
    }

    void Scene::AddComponents()
    {
        for (const auto [entity, componentId] : m_componentsToAdd) {
            m_mainRegistry->storage(componentId)->push(entity->GetHandle());
        }
        m_componentsToAdd.clear();
    }

    void Scene::RemoveComponents()
    {
        for (const auto [entity, componentId] : m_componentsToRemove) {
            m_mainRegistry->storage(componentId)->remove(entity->GetHandle());
        }
        m_componentsToRemove.clear();
    }

    void Scene::OnAddRigidBody2DComponent(entt::registry& registry, entt::entity entity)
    {
        const auto& entityId{registry.get<IdComponent>(entity).value};
        ASSERT(m_entityById.contains(entityId));
        auto [transform, rigidBody] = registry.get<const TransformComponent, RigidBody2DComponent>(entity);
        rigidBody.bodyId = Locator::GetPhysicsEngine2D()->CreateBody(
            rigidBody.bodyData, glm::vec2{transform.position.x, transform.position.y}, transform.rotation.z,
            m_entityById.find(entityId)->second, entityId.GetString());
    }

    void Scene::OnAddBoxCollider2DComponent(entt::registry& registry, entt::entity entity)
    {
        const auto& entityId{registry.get<IdComponent>(entity).value};
        ASSERT(m_entityById.contains(entityId));
        const auto* rigidBody{registry.try_get<RigidBody2DComponent>(entity)};
        if (!rigidBody) {
            Locator::GetLogger()->Warn("Trying to add box collider into {} without rigid body!",
                                       registry.get<const IdComponent>(entity).value.GetString());
            return;
        }
        auto [transform, collider] = registry.get<const TransformComponent, BoxCollider2DComponent>(entity);
        ASSERT(rigidBody->bodyId);
        collider.shapeId = Locator::GetPhysicsEngine2D()->CreateBoxShape(
            rigidBody->bodyId.value(), collider.shapeData, collider.width * transform.scale.x,
            collider.height * transform.scale.y, collider.edgeRadius, collider.offset, collider.rotation,
            m_entityById.find(entityId)->second);
    }

    void Scene::OnAddCircleCollider2DComponent(entt::registry& registry, entt::entity entity)
    {
        const auto& entityId{registry.get<IdComponent>(entity).value};
        ASSERT(m_entityById.contains(entityId));
        const auto* rigidBody{registry.try_get<RigidBody2DComponent>(entity)};
        if (!rigidBody) {
            Locator::GetLogger()->Warn("Trying to add circle collider into {} without rigid body!",
                                       registry.get<const IdComponent>(entity).value.GetString());
            return;
        }
        auto [transform, collider] =
            registry.get<const TransformComponent, CircleCollider2DComponent>(entity);
        ASSERT(rigidBody->bodyId);
        collider.shapeId = Locator::GetPhysicsEngine2D()->CreateCircleShape(
            rigidBody->bodyId.value(), collider.shapeData,
            collider.radius * std::max(transform.scale.x, transform.scale.y), collider.offset,
            m_entityById.find(entityId)->second);
    }

    void Scene::OnRemoveRigidBody2DComponent(entt::registry& registry, entt::entity entity)
    {
        const auto& rigidBody{registry.get<const RigidBody2DComponent>(entity)};
        ASSERT(rigidBody.bodyId);
        Locator::GetPhysicsEngine2D()->DestroyBody(rigidBody.bodyId.value());
    }

    void Scene::OnRemoveBoxCollider2DComponent(entt::registry& registry, entt::entity entity)
    {
        const auto& collider{registry.get<const BoxCollider2DComponent>(entity)};
        ASSERT(collider.shapeId);
        Locator::GetPhysicsEngine2D()->DestroyShape(collider.shapeId.value(), true);
    }

    void Scene::OnRemoveCircleCollider2DComponent(entt::registry& registry, entt::entity entity)
    {
        const auto& collider{registry.get<const CircleCollider2DComponent>(entity)};
        ASSERT(collider.shapeId);
        Locator::GetPhysicsEngine2D()->DestroyShape(collider.shapeId.value(), true);
    }

    void Scene::StartPhysics2D()
    {
        const auto view{m_mainRegistry->view<RigidBody2DComponent>()};
        for (const auto entity : view) {
            OnAddRigidBody2DComponent(*m_mainRegistry, entity);
            if (m_mainRegistry->all_of<BoxCollider2DComponent>(entity)) {
                OnAddBoxCollider2DComponent(*m_mainRegistry, entity);
            }
            if (m_mainRegistry->all_of<CircleCollider2DComponent>(entity)) {
                OnAddCircleCollider2DComponent(*m_mainRegistry, entity);
            }
        }
    }

    void Scene::StartPlayerInput()
    {
        const auto view{m_mainRegistry->view<PlayerInputComponent>()};
        const StringId* defaultInputScope{};
        for (const auto entity : view) {
            auto& playerInput{view.get<PlayerInputComponent>(entity)};
            if (!defaultInputScope) {
                defaultInputScope = &playerInput.defaultInputScope;
            }
            auto& playerInputRuntime{m_mainRegistry->get_or_emplace<PlayerInputRuntimeComponent>(entity)};
            playerInputRuntime.inputDeviceId = Locator::GetInputManager()->AcquireAvailableDevice();
            std::unordered_map<StringId, ScriptInstance*> scriptInstances{};
            auto* scriptComponent = m_mainRegistry->try_get<ScriptRuntimeComponent>(entity);
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

    void Scene::StartScripts()
    {
        const auto view{m_mainRegistry->view<ScriptComponent>()};
        for (const auto entity : view) {
            const auto& entityId{m_mainRegistry->get<IdComponent>(entity).value};
            ASSERT(m_entityById.contains(entityId));
            auto& scriptComponent{view.get<ScriptComponent>(entity)};
            for (auto& scriptData : scriptComponent.scriptDatas) {
                std::optional scriptInstance{Locator::GetScriptSystem()->CreateScriptInstance(
                    scriptData, m_entityById.find(entityId)->second)};
                if (scriptInstance) {
                    auto& scriptRuntime{m_mainRegistry->get_or_emplace<ScriptRuntimeComponent>(entity)};
                    scriptRuntime.scriptInstances.push_back(*scriptInstance);
                    scriptInstance->InvokeOnStart();
                }
            }
        }
    }

    void Scene::OnInputCommand(const InputEvent& event)
    {
        const auto view{m_mainRegistry->view<const PlayerInputRuntimeComponent>()};
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
        const auto view{m_mainRegistry->view<TransformComponent, RigidBody2DComponent>()};
        for (const auto entity : view) {
            auto [transform, rigidBody] = view.get<TransformComponent, RigidBody2DComponent>(entity);
            const auto position{glm::vec2{transform.position}};
            const auto rotation{transform.rotation.z};
            ASSERT(rigidBody.bodyId);
            if (const auto bodyId{rigidBody.bodyId.value()};
                physicsEngine->GetPosition(bodyId) != position ||
                physicsEngine->GetRotationAngle(bodyId) != rotation) {
                physicsEngine->SetTransform(bodyId, position, rotation);
            }
        }
        physicsEngine->Update(timeStep);
        const auto bodyEvents{physicsEngine->GetBodyEvents()};
        for (int i{0}; i < bodyEvents.moveCount; ++i) {
            const auto* event{bodyEvents.moveEvents + i};
            Entity* entity{static_cast<Entity*>(event->userData)};
            if (auto* transform{m_mainRegistry->try_get<TransformComponent>(entity->GetHandle().entity())}) {
                transform->position.x = event->transform.p.x;
                transform->position.y = event->transform.p.y;
                transform->rotation.z = b2Rot_GetAngle(event->transform.q);
            }
        }
        const auto collisionEvents{physicsEngine->GetCollisionEvents()};
        for (int i{0}; i < collisionEvents.beginCount; ++i) {
            const auto* beginEvent{collisionEvents.beginEvents + i};
            auto& entityA{*physicsEngine->GetEntity(beginEvent->shapeIdA)};
            auto& entityB{*physicsEngine->GetEntity(beginEvent->shapeIdB)};
            OnCollisionEnter(entityA, entityB, beginEvent->manifold, beginEvent->shapeIdB);
            OnCollisionEnter(entityB, entityA, beginEvent->manifold, beginEvent->shapeIdA);
        }
        for (int i{0}; i < collisionEvents.endCount; ++i) {
            const auto* endEvent{collisionEvents.endEvents + i};
            if (physicsEngine->IsValid(endEvent->shapeIdA) && physicsEngine->IsValid(endEvent->shapeIdB)) {
                auto& entityA{*physicsEngine->GetEntity(endEvent->shapeIdA)};
                auto& entityB{*physicsEngine->GetEntity(endEvent->shapeIdB)};
                OnCollisionExit(entityA, entityB, endEvent->shapeIdB);
                OnCollisionExit(entityB, entityA, endEvent->shapeIdA);
            }
        }
        const auto triggerEvents{physicsEngine->GetTriggerEvents()};
        for (int i{0}; i < triggerEvents.beginCount; ++i) {
            const auto* beginEvent{triggerEvents.beginEvents + i};
            auto& triggerEntity{*physicsEngine->GetEntity(beginEvent->sensorShapeId)};
            auto& visitorEntity{*physicsEngine->GetEntity(beginEvent->visitorShapeId)};
            OnTriggerEnter(triggerEntity, visitorEntity, beginEvent->visitorShapeId);
        }
        for (int i{0}; i < triggerEvents.endCount; ++i) {
            const auto* endEvent{triggerEvents.endEvents + i};
            if (physicsEngine->IsValid(endEvent->visitorShapeId)) {
                auto& triggerEntity{*physicsEngine->GetEntity(endEvent->sensorShapeId)};
                auto& visitorEntity{*physicsEngine->GetEntity(endEvent->visitorShapeId)};
                OnTriggerExit(triggerEntity, visitorEntity, endEvent->visitorShapeId);
            }
        }
    }

    void Scene::UpdateScripts(float timeStep)
    {
        const auto view{m_mainRegistry->view<ScriptRuntimeComponent>()};
        for (const auto entity : view) {
            for (auto& script : view.get<ScriptRuntimeComponent>(entity).scriptInstances) {
                script.InvokeOnUpdate(timeStep);
            }
        }
        for (const auto entity : view) {
            for (auto& script : view.get<ScriptRuntimeComponent>(entity).scriptInstances) {
                script.InvokeOnLateUpdate(timeStep);
            }
        }
    }
} // namespace Engine
