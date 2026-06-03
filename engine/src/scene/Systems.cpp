#include "Systems.h"

#include "Components.h"
#include "Engine.h"
#include "Entity.h"
#include "ScriptingApi.h"
#include "core/AABB.h"
#include "core/Locator.h"
#include "events/Events.h"
#include "renderer/RenderManager.h"

#include <ranges>
#include <string>
#include <utility>

namespace Engine
{
    constexpr glm::vec2 GetExtrapolatedPosition(const glm::vec2 position, const glm::vec2 velocity,
                                                float extrapolationTimeStep)
    {
        return glm::vec2{position + velocity * extrapolationTimeStep};
    }

    void PhysicsSystem::Start()
    {
        const auto view{GetRegistry().view<const TransformComponent, const RigidBody2DComponent>(
            entt::exclude<RigidBody2DRuntimeComponent>)};
        auto& registry{GetRegistry()};
        auto physicsEngine{Locator::GetPhysicsEngine2D()};
        for (const auto entity : view) {
            auto [transform, rigidBody] = view.get<TransformComponent, RigidBody2DComponent>(entity);
            const auto bodyId = physicsEngine->CreateBody(
                rigidBody.bodyData, glm::vec2{transform.position.x, transform.position.y},
                transform.rotation.z, entity);
            auto& rigidBodyRuntime{registry.emplace<RigidBody2DRuntimeComponent>(entity, bodyId)};
            bool hasCollider{false};
            if (const auto* boxCollider{registry.try_get<BoxCollider2DComponent>(entity)}) {
                physicsEngine->CreateBoxShape(
                    rigidBodyRuntime.bodyId, boxCollider->shapeData, boxCollider->width * transform.scale.x,
                    boxCollider->height * transform.scale.y, boxCollider->edgeRadius);
                hasCollider = true;
            }
            if (const auto* circleCollider{registry.try_get<CircleCollider2DComponent>(entity)}) {
                physicsEngine->CreateCircleShape(rigidBodyRuntime.bodyId, circleCollider->shapeData,
                                                 circleCollider->radius *
                                                     std::max(transform.scale.x, transform.scale.y));
                hasCollider = true;
            }
            if (!hasCollider) {
                physicsEngine->CreateDefaultShape(rigidBodyRuntime.bodyId);
            }
        }
    }

    void PhysicsSystem::Update(float timeStep)
    {
        const auto physicsEngine{Locator::GetPhysicsEngine2D()};
        const auto view{GetRegistry().view<TransformComponent, RigidBody2DRuntimeComponent>()};
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
            if (auto* transform{GetRegistry().try_get<TransformComponent>(entity)}) {
                transform->position.x = event->transform.p.x;
                transform->position.y = event->transform.p.y;
                transform->rotation.z = b2Rot_GetAngle(event->transform.q);
            }
        }
    }

    void RenderingSystem::Update(float frameExtrapolationTimeStep)
    {
        const auto cameraView{GetRegistry().view<const TransformComponent, CameraComponent>()};
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
        GetRegistry().sort<SpriteComponent>(
            [](const SpriteComponent& lhs, const SpriteComponent& rhs) { return lhs.zIndex < rhs.zIndex; });
        auto spriteView{GetRegistry().view<const TransformComponent, const SpriteComponent>()};
        spriteView.use<SpriteComponent>();
        for (const auto entity : spriteView) {
            auto [transform, spriteComponent] = spriteView.get<TransformComponent, SpriteComponent>(entity);
            glm::vec2 renderPosition{transform.position};
            if (const auto* rigidBody{GetRegistry().try_get<RigidBody2DRuntimeComponent>(entity)}) {
                const auto velocity{Locator::GetPhysicsEngine2D()->GetLinearVelocity(rigidBody->bodyId)};
                renderPosition =
                    GetExtrapolatedPosition(transform.position, velocity, frameExtrapolationTimeStep);
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

    void SpriteAnimationSystem::Update()
    {
        /*        const auto view{GetRegistry().view<SpriteComponent, SpriteAnimationComponent>()};
                for (const auto entity : view) {
                    auto [sprite, spriteAnimation] = view.get<SpriteComponent,
           SpriteAnimationComponent>(entity); spriteAnimation.currentFrame =
           (static_cast<int>(Timer::GetTicks() - spriteAnimation.startTime) * spriteAnimation.framesPerSecond
           / 1000) % spriteAnimation.framesCount;
                    // sprite.textureArea.position.x = static_cast<float>(spriteAnimation.currentFrame) *
                    // sprite.textureArea.width;
                }*/
    }

    void PlayerInputSystem::Start()
    {
        const auto view{GetRegistry().view<PlayerInputComponent>()};
        const StringId* defaultInputScope{};
        for (const auto entity : view) {
            auto& playerInput{view.get<PlayerInputComponent>(entity)};
            if (!defaultInputScope) {
                defaultInputScope = &playerInput.defaultInputScope;
            }
            auto& playerInputRuntime{GetRegistry().get_or_emplace<PlayerInputRuntimeComponent>(entity)};
            playerInputRuntime.inputDeviceId = Locator::GetInputManager()->AcquireAvailableDevice();
            std::unordered_map<StringId, ScriptInstance*> scriptInstances{};
            auto* scriptComponent = GetRegistry().try_get<ScriptRuntimeComponent>(entity);
            if (!scriptComponent) {
                continue;
            }
            for (auto& scriptInstance : scriptComponent->scriptInstances) {
                scriptInstances.insert(std::make_pair(scriptInstance.GetScriptClassId(), &scriptInstance));
            }
            for (const auto& [commandId, callbackDef] : playerInput.callbackDefs) {
                if (auto it{scriptInstances.find(callbackDef.scriptId)}; it != scriptInstances.end()) {
                    ScriptInstance* scriptInstance{it->second};
                    playerInputRuntime.callbackMapping.SetCommandCallback(
                        commandId, InputCallback{std::function<void(InputValue)>{
                                       [scriptInstance, callbackDef](InputValue inputValue) {
                                           scriptInstance->Call(callbackDef.callbackName, inputValue);
                                       }}});
                }
            }
        }
        if (defaultInputScope) {
            Locator::GetInputManager()->SwitchScope(*defaultInputScope);
        }
    }

    void PlayerInputSystem::SubscribeToEvents(EventBus& eventBus)
    {
        eventBus.AddSubscriber<InputEvent, PlayerInputSystem>(this, &PlayerInputSystem::OnInputCommand);
    }

    void PlayerInputSystem::OnInputCommand(const InputEvent& event) const
    {
        const auto view{GetRegistry().view<const PlayerInputRuntimeComponent>()};
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

    ScriptingSystem::ScriptingSystem(entt::registry* registry)
        : System{registry}
    {
        m_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math);
        const std::string packagePath{m_lua["package"]["path"]};
        m_lua["package"]["path"] =
            packagePath + (packagePath.empty() ? "" : ";") + (s_scriptingLibPath / "?.lua").string();
        m_lua.require_file("utils", s_scriptingLibPath / "utils.lua");
        m_lua.script_file(s_scriptingLibPath / "entity_script.lua");
        CreateScriptBindings();
        m_lua.create_named_table("scriptClasses");
        m_lua.create_named_table("scriptInstances");
    }

    ScriptingSystem::~ScriptingSystem() { m_scriptClasses.clear(); }

    std::unique_ptr<ScriptClass> ScriptingSystem::LoadScriptClass(const std::filesystem::path& filePath,
                                                                  std::string_view className)
    {
        const auto absoluteFilePath{ResourceManager::GetResourcePath(filePath)};
        const auto result{m_lua.script_file(absoluteFilePath, sol::script_pass_on_error)};
        if (!result.valid()) {
            const sol::error error{result};
            const sol::call_status status{result.status()};
            Locator::GetLogger()->Error("Error loading script {}: {} error\n\t{}", absoluteFilePath.c_str(),
                                        sol::to_string(status), error.what());
            return {};
        }
        const sol::optional<sol::table> maybeScriptClass{m_lua[className]};
        if (!maybeScriptClass) {
            Locator::GetLogger()->Warn("Script '{}' doesn't have '{}' class", filePath.c_str(), className);
            return {};
        }
        sol::table scriptClass{maybeScriptClass.value()};
        const sol::table entityScriptClass{m_lua["EntityScript"]};
        const auto isEntityScript{m_lua["utils"]["instance_of"](scriptClass, entityScriptClass)};
        if (!isEntityScript.get<bool>()) {
            Locator::GetLogger()->Warn("Script '{}' doesn't inherit from EntityScript", className);
            return {};
        }
        sol::table scriptClasses{m_lua["scriptClasses"]};
        scriptClasses[scriptClasses.size()] = scriptClass;
        return std::make_unique<ScriptClass>(className, scriptClass);
    }

    std::optional<ScriptInstance> ScriptingSystem::CreateScriptInstance(const std::filesystem::path& filePath,
                                                                        std::string_view className,
                                                                        entt::entity entity)
    {
        StringId scriptClassId{StringId::Intern(filePath.c_str())};
        ScriptClass* scriptClass{GetScriptClass(scriptClassId)};
        if (!scriptClass) {
            if (std::unique_ptr<ScriptClass> newScriptClass{LoadScriptClass(filePath, className)}) {
                StoreScriptClass(scriptClassId, std::move(newScriptClass));
                scriptClass = GetScriptClass(scriptClassId);
            } else {
                return {};
            }
        }
        const sol::optional<sol::function> maybeScriptClassConstructor{scriptClass->GetConstructor()};
        if (!maybeScriptClassConstructor) {
            Locator::GetLogger()->Warn("Script '{}' doesn't have a constructor", scriptClass->GetClassName());
            return {};
        }
        const sol::optional<sol::table> maybeScriptInstance{
            maybeScriptClassConstructor.value()(scriptClass->GetLuaClass())};
        if (!maybeScriptInstance) {
            Locator::GetLogger()->Warn("Failed to instantiate '{}' script", scriptClass->GetClassName());
            return {};
        }
        sol::table scriptInstance{maybeScriptInstance.value()};
        scriptInstance["entity"] = Entity{entt::handle{GetRegistry(), entity}};
        sol::table scriptInstances{m_lua["scriptInstances"]};
        scriptInstances[scriptInstances.size()] = scriptInstance;
        return ScriptInstance{scriptClassId, scriptInstance};
    }

    void ScriptingSystem::Start()
    {
        const auto view{GetRegistry().view<ScriptComponent>()};
        for (const auto entity : view) {
            auto& scriptComponent = view.get<ScriptComponent>(entity);
            for (auto& scriptDef : scriptComponent.scriptDefs) {
                std::optional scriptInstance{
                    CreateScriptInstance(scriptDef.filePath, scriptDef.className, entity)};
                if (scriptInstance) {
                    auto& scriptRuntime{GetRegistry().get_or_emplace<ScriptRuntimeComponent>(entity)};
                    scriptRuntime.scriptInstances.push_back(*scriptInstance);
                    scriptInstance->OnStart();
                }
            }
        }
    }

    void ScriptingSystem::Update(float timeStep)
    {
        const auto view{GetRegistry().view<ScriptRuntimeComponent>()};
        for (const auto entity : view) {
            auto& scriptComponent = view.get<ScriptRuntimeComponent>(entity);
            for (auto& script : scriptComponent.scriptInstances) {
                script.OnUpdate(timeStep);
            }
        }
    }

    void ScriptingSystem::ShutDown() {}

    void ScriptingSystem::CreateScriptBindings()
    {
        m_lua.new_usertype<StringId>("StringId", "id", &StringId::GetSid, "str", &StringId::GetString);
        m_lua.new_usertype<Entity>("Entity", "id", sol::property(&Entity::GetId));
        m_lua.new_usertype<glm::vec2>(
            "Vec2", sol::constructors<glm::vec2(), glm::vec2(float), glm::vec2(float, float)>(), "x",
            &glm::vec2::x, "y", &glm::vec2::y);
        m_lua.new_usertype<glm::vec3>(
            "Vec3", sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>(), "x",
            &glm::vec3::x, "y", &glm::vec3::y, "z", &glm::vec3::z);
        m_lua.new_usertype<AABB>("AABB", "center", &AABB::center, "width", &AABB::width, "height",
                                 &AABB::height);
        m_lua.new_usertype<InputValue>("InputValue", "value", &InputValue::value);
        sol::usertype transform{m_lua.new_usertype<ScriptingApi::Transform>("Transform")};
        transform["position"] =
            sol::property(&ScriptingApi::Transform::GetPosition, &ScriptingApi::Transform::SetPosition);
        transform["rotation"] =
            sol::property(&ScriptingApi::Transform::GetRotation, &ScriptingApi::Transform::SetRotation);
        transform["scale"] =
            sol::property(&ScriptingApi::Transform::GetScale, &ScriptingApi::Transform::SetScale);
        transform["up"] = sol::property(&ScriptingApi::Transform::GetUp);
        transform["right"] = sol::property(&ScriptingApi::Transform::GetRight);
        transform["forward"] = sol::property(&ScriptingApi::Transform::GetForward);
        sol::usertype rigidBody2D{m_lua.new_usertype<ScriptingApi::RigidBody2D>("RigidBody2D")};
        rigidBody2D["position"] = sol::property(&ScriptingApi::RigidBody2D::GetPosition);
        rigidBody2D["rotationAngle"] = sol::property(&ScriptingApi::RigidBody2D::GetRotationAngle);
        rigidBody2D["linearVelocity"] = sol::property(&ScriptingApi::RigidBody2D::GetLinearVelocity,
                                                      &ScriptingApi::RigidBody2D::SetLinearVelocity);
        rigidBody2D["ApplyForceToCenter"] = &ScriptingApi::RigidBody2D::ApplyForceToCenter;
        rigidBody2D["ApplyTorque"] = &ScriptingApi::RigidBody2D::ApplyTorque;
        m_componentTypes.emplace(
            m_lua.create_named_table("Transform").pointer(),
            [this](const Entity entity) { return sol::make_object(m_lua, ScriptingApi::Transform{entity}); });
        m_componentTypes.emplace(m_lua.create_named_table("RigidBody2D").pointer(),
                                 [this](const Entity entity) {
                                     return sol::make_object(m_lua, ScriptingApi::RigidBody2D{entity});
                                 });
        m_lua.set_function("ApiGetComponent", [this](const Entity entity, const sol::table& table) {
            return GetComponent(entity, table);
        });
    }

    void ScriptingSystem::StoreScriptClass(const StringId& scriptId, std::unique_ptr<ScriptClass> scriptClass)
    {
        m_scriptClasses[scriptId] = std::move(scriptClass);
    }

    sol::object ScriptingSystem::GetComponent(Entity entity, const sol::table& type)
    {
        if (const auto it{m_componentTypes.find(type.pointer())}; it != m_componentTypes.end()) {
            return m_componentTypes[type.pointer()](entity);
        }
        return sol::nil;
    }

    ScriptClass* ScriptingSystem::GetScriptClass(const StringId& scriptId) const
    {
        const auto scriptIterator{m_scriptClasses.find(scriptId)};
        if (scriptIterator != m_scriptClasses.end()) {
            return scriptIterator->second.get();
        }
        return nullptr;
    }
} // namespace Engine
