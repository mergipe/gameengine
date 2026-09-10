#include "Scene.h"

#include "Components.h"
#include "ECSUtils.h"
#include "Engine.h"
#include "core/Assert.h"
#include "core/Locator.h"
#include "core/Math.h"

#include <entt/entity/runtime_view.hpp>
#include <ranges>

namespace Engine
{
    void OnCollisionEnter(const Entity& entity, Entity& other, const CollisionManifold2D& manifold,
                          Shape2DId otherShapeId)
    {
        if (entity.HasComponent<ScriptBaseComponent>()) {
            const auto& scriptClassIds{entity.GetComponent<ScriptBaseComponent>().classIds};
            for (const auto& scriptClassId : scriptClassIds) {
                entity.GetScript(scriptClassId)->InvokeOnCollisionEnter(other, manifold, otherShapeId);
            }
        }
    }

    void OnCollisionExit(const Entity& entity, Entity& other, Shape2DId otherShapeId)
    {
        if (entity.HasComponent<ScriptBaseComponent>()) {
            const auto& scriptClassIds{entity.GetComponent<ScriptBaseComponent>().classIds};
            for (const auto& scriptClassId : scriptClassIds) {
                entity.GetScript(scriptClassId)->InvokeOnCollisionExit(other, otherShapeId);
            }
        }
    }

    void OnTriggerEnter(const Entity& entity, Entity& other, Shape2DId otherShapeId)
    {
        if (entity.HasComponent<ScriptBaseComponent>()) {
            const auto& scriptClassIds{entity.GetComponent<ScriptBaseComponent>().classIds};
            for (const auto& scriptClassId : scriptClassIds) {
                entity.GetScript(scriptClassId)->InvokeOnTriggerEnter(other, otherShapeId);
            }
        }
    }

    void OnTriggerExit(const Entity& entity, Entity& other, Shape2DId otherShapeId)
    {
        if (entity.HasComponent<ScriptBaseComponent>()) {
            const auto& scriptClassIds{entity.GetComponent<ScriptBaseComponent>().classIds};
            for (const auto& scriptClassId : scriptClassIds) {
                entity.GetScript(scriptClassId)->InvokeOnTriggerExit(other, otherShapeId);
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

        SetRegistryListeners();
        StartPhysics2D();
        StartScripts();
        StartPlayerInput();
    }

    void Scene::Update(float timeStep)
    {
        CreateEntities();
        AddComponents();
        AddScripts();

        auto* eventBus{Locator::GetEventBus()};
        eventBus->Reset();
        UpdatePlayerInput(eventBus);
        UpdatePhysics2D(timeStep);
        UpdateScripts(timeStep);

        RemoveScripts();
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

    Entity& Scene::CreateEntityOnNextStep() { return CreateEntityOnNextStep(entt::handle{}); }

    Entity& Scene::CreateEntityOnNextStep(entt::handle entityTemplate)
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

    void Scene::DestroyEntityOnNextStep(const Entity& entity) { m_entitiesToDestroy.push_back(entity); }

    std::optional<ScriptHandle> Scene::AddScriptOnNextStep(Entity& entity, const StringId& scriptClassId)
    {
        if (auto scriptInstance{Locator::GetScriptSystem()->CreateScriptInstance(entity, scriptClassId)}) {
            return m_scriptsToAdd.emplace_back(&entity, std::move(scriptInstance.value()))
                .scriptInstance.GetHandle();
        }
        return {};
    }

    void Scene::RemoveScriptOnNextStep(Entity& entity, const StringId& scriptClassId)
    {
        m_scriptsToRemove.emplace_back(&entity, scriptClassId);
    }

    void Scene::SetRegistryListeners()
    {
        m_mainRegistry->on_construct<RigidBody2DComponent>().connect<&Scene::OnAddRigidBody2DComponent>(this);
        m_mainRegistry->on_construct<BoxCollider2DComponent>().connect<&Scene::OnAddBoxCollider2DComponent>(
            this);
        m_mainRegistry->on_construct<CircleCollider2DComponent>()
            .connect<&Scene::OnAddCircleCollider2DComponent>(this);
        m_mainRegistry->on_construct<ScriptClassDatasComponent>()
            .connect<&Scene::OnAddScriptClassDatasComponent>(this);
        m_mainRegistry->on_destroy<RigidBody2DComponent>().connect<&Scene::OnRemoveRigidBody2DComponent>(
            this);
        m_mainRegistry->on_destroy<BoxCollider2DComponent>().connect<&Scene::OnRemoveBoxCollider2DComponent>(
            this);
        m_mainRegistry->on_destroy<CircleCollider2DComponent>()
            .connect<&Scene::OnRemoveCircleCollider2DComponent>(this);
    }

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
            if (entity.IsValid()) {
                m_entityById.erase(entity.GetId());
                m_mainRegistry->destroy(entity.GetHandle());
            }
        }
        m_entitiesToDestroy.clear();
    }

    void Scene::AddComponents()
    {
        for (const auto [entity, componentId] : m_componentsToAdd) {
            entity->AddComponent(componentId);
        }
        m_componentsToAdd.clear();
    }

    void Scene::RemoveComponents()
    {
        for (const auto [entity, componentId] : m_componentsToRemove) {
            entity->RemoveComponent(componentId);
        }
        m_componentsToRemove.clear();
    }

    void Scene::AddScripts()
    {
        for (auto& [entity, scriptInstance] : m_scriptsToAdd) {
            entity->AddScript(std::move(scriptInstance));
        }
        m_scriptsToAdd.clear();
    }

    void Scene::RemoveScripts()
    {
        for (const auto& [entity, scriptClassId] : m_scriptsToRemove) {
            entity->RemoveScript(scriptClassId);
        }
        m_scriptsToRemove.clear();
    }

    void InvokeOnStart(ScriptInstance& scriptInstance) { scriptInstance.InvokeOnStart(); }

    void InvokeOnUpdate(ScriptInstance& scriptInstance, float timeStep)
    {
        scriptInstance.InvokeOnUpdate(timeStep);
    }

    void InvokeOnLateUpdate(ScriptInstance& scriptInstance, float timeStep)
    {
        scriptInstance.InvokeOnLateUpdate(timeStep);
    }

    void Scene::InvokeOnAllScripts(const std::function<void(ScriptInstance&)>& function)
    {
        for (const auto& scriptClassId : Locator::GetScriptSystem()->GetScriptClasses() | std::views::keys) {
            auto& storage{m_mainRegistry->storage<ScriptInstanceComponent>(scriptClassId.GetId())};
            entt::runtime_view scriptInstanceView{};
            scriptInstanceView.iterate(storage);
            for (const auto entity : scriptInstanceView) {
                function(storage.get(entity).instance);
            }
        }
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

    void Scene::OnAddScriptClassDatasComponent(entt::registry& registry, entt::entity entity)
    {
        const auto& entityId{registry.get<IdComponent>(entity).value};
        ASSERT(m_entityById.contains(entityId));
        const auto& scriptComponent{registry.get<ScriptClassDatasComponent>(entity)};
        auto& entityRef{m_entityById.find(entityId)->second};
        for (auto& scriptClassData : scriptComponent.classDatas) {
            std::optional scriptInstance{
                Locator::GetScriptSystem()->CreateScriptInstance(entityRef, scriptClassData)};
            if (scriptInstance) {
                entityRef.AddScript(std::move(scriptInstance.value()));
            }
        }
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
            const auto& entityId{m_mainRegistry->get<IdComponent>(entity).value};
            ASSERT(m_entityById.contains(entityId));
            const auto& entityRef{m_entityById.find(entityId)->second};
            auto& playerInput{view.get<PlayerInputComponent>(entity)};
            if (!defaultInputScope) {
                defaultInputScope = &playerInput.defaultInputScope;
            }
            auto& playerInputRuntime{m_mainRegistry->get_or_emplace<PlayerInputRuntimeComponent>(entity)};
            playerInputRuntime.inputDeviceId = Locator::GetInputManager()->AcquireAvailableDevice();
            for (const auto& [commandId, callbackDef] : playerInput.callbackDefs) {
                playerInputRuntime.callbackMapping.SetCommandCallback(
                    commandId, InputCallback{[entityRef, callbackDef](InputValue inputValue) {
                        if (auto* scriptInstance{entityRef.GetScript(callbackDef.scriptClassId)}) {
                            scriptInstance->InvokeFunction(callbackDef.callbackName, inputValue);
                        }
                    }});
            }
        }
        if (defaultInputScope) {
            Locator::GetInputManager()->SwitchScope(*defaultInputScope);
        }
    }

    void Scene::StartScripts()
    {
        const auto scriptView{m_mainRegistry->view<const ScriptClassDatasComponent>()};
        for (const auto entity : scriptView) {
            OnAddScriptClassDatasComponent(*m_mainRegistry, entity);
        }
        InvokeOnAllScripts([](ScriptInstance& scriptInstance) { scriptInstance.InvokeOnStart(); });
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
            if (physicsEngine->IsValid(endEvent->sensorShapeId) &&
                physicsEngine->IsValid(endEvent->visitorShapeId)) {
                auto& triggerEntity{*physicsEngine->GetEntity(endEvent->sensorShapeId)};
                auto& visitorEntity{*physicsEngine->GetEntity(endEvent->visitorShapeId)};
                OnTriggerExit(triggerEntity, visitorEntity, endEvent->visitorShapeId);
            }
        }
    }

    void Scene::UpdateScripts(float timeStep)
    {
        InvokeOnAllScripts(
            [timeStep](ScriptInstance& scriptInstance) { scriptInstance.InvokeOnUpdate(timeStep); });
        InvokeOnAllScripts(
            [timeStep](ScriptInstance& scriptInstance) { scriptInstance.InvokeOnLateUpdate(timeStep); });
    }
} // namespace Engine
