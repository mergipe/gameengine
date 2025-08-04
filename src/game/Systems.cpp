#include "Systems.h"
#include "Components.h"
#include "Entity.h"
#include "Game.h"
#include "ScriptingApi.h"
#include "core/Locator.h"
#include "core/Timer.h"
#include "events/Events.h"
#include "renderer/Renderer2D.h"
#include <string>
#include <utility>

namespace Engine
{
    constexpr bool aabbHasCollided(const Rect& lhs, const Rect& rhs)
    {
        return lhs.getLeftX() < rhs.getRightX() && lhs.getRightX() > rhs.getLeftX() &&
               lhs.getTopY() > rhs.getBottomY() && lhs.getBottomY() < rhs.getTopY();
    }

    constexpr glm::vec2 getExtrapolatedPosition(const glm::vec2& position, const glm::vec2& velocity,
                                                float extrapolationTimeStep)
    {
        return glm::vec2{position + velocity * extrapolationTimeStep};
    }

    constexpr bool isOutsideOrthoCameraView(const Camera& camera, const Rect& rect)
    {
        const Rect cameraPlaneGeometry{camera.getNearPlaneGeometry()};
        return rect.getRightX() < cameraPlaneGeometry.getLeftX() ||
               rect.getLeftX() > cameraPlaneGeometry.getRightX() ||
               rect.getTopY() < cameraPlaneGeometry.getBottomY() ||
               rect.getBottomY() > cameraPlaneGeometry.getTopY();
    }

    void MovementSystem::update(float timeStep)
    {
        const auto view{getRegistry().view<TransformComponent, const RigidBody2DComponent>()};
        for (const auto entity : view) {
            auto [transform, rigidBody] = view.get<TransformComponent, RigidBody2DComponent>(entity);
            transform.position += rigidBody.velocity * timeStep;
        }
    }

    void RenderingSystem::update(Renderer2D& renderer, float frameExtrapolationTimeStep)
    {
        const auto cameraView{getRegistry().view<const TransformComponent, const CameraComponent>()};
        Camera* cameraPtr{};
        for (const auto entity : cameraView) {
            auto [transform, camera] = cameraView.get<TransformComponent, CameraComponent>(entity);
            cameraPtr = camera.camera.get();
            if (!cameraPtr)
                continue;
            cameraPtr->setModelTransformation(transform.getTransformation());
            break;
        }
        if (!cameraPtr)
            return;
        renderer.setupCamera(*cameraPtr);
        getRegistry().sort<SpriteComponent>(
            [](const SpriteComponent& lhs, const SpriteComponent& rhs) { return lhs.zIndex < rhs.zIndex; });
        auto spriteView{getRegistry().view<const TransformComponent, const SpriteComponent>()};
        spriteView.use<SpriteComponent>();
        for (const auto entity : spriteView) {
            auto [transform, sprite] = spriteView.get<TransformComponent, SpriteComponent>(entity);
            glm::vec2 renderPosition{transform.position};
            if (const auto* rigidBody{getRegistry().try_get<RigidBody2DComponent>(entity)}) {
                renderPosition = getExtrapolatedPosition(transform.position, rigidBody->velocity,
                                                         frameExtrapolationTimeStep);
            }
            const float spriteWidth{sprite.textureArea.width * transform.scale.x};
            const float spriteHeight{sprite.textureArea.height * transform.scale.y};
            const Rect spriteGeometry{renderPosition, spriteWidth, spriteHeight};
            if (!isOutsideOrthoCameraView(*cameraPtr, spriteGeometry)) {
                renderer.drawSprite(spriteGeometry, transform.rotation,
                                    Locator::getResourceManager()->getTexture(sprite.textureId),
                                    sprite.textureArea, sprite.color);
            }
        }
    }

    void DebugRenderingSystem::update(Renderer2D& renderer, float frameExtrapolationTimeStep)
    {
        const auto view{getRegistry().view<const TransformComponent, const BoxCollider2DComponent>()};
        for (const auto entity : view) {
            auto [transform, boxCollider] = view.get<TransformComponent, BoxCollider2DComponent>(entity);
            glm::vec2 renderPosition{transform.position};
            if (const auto* rigidBody{getRegistry().try_get<RigidBody2DComponent>(entity)}) {
                renderPosition = getExtrapolatedPosition(transform.position, rigidBody->velocity,
                                                         frameExtrapolationTimeStep);
            }
            glm::vec4 color;
            if (boxCollider.isColliding) {
                color = {1.0f, 0.0f, 0.0f, 1.0f};
            } else {
                color = {1.0f, 1.0f, 0.0f, 1.0f};
            }
            Rect rect{renderPosition + boxCollider.offset,
                      static_cast<float>(boxCollider.width) * transform.scale.x,
                      static_cast<float>(boxCollider.height) * transform.scale.y};
            renderer.drawRectangle(rect, color, glm::vec3{0.0f});
        }
    }

    void SpriteAnimationSystem::update()
    {
        const auto view{getRegistry().view<SpriteComponent, SpriteAnimationComponent>()};
        for (const auto entity : view) {
            auto [sprite, spriteAnimation] = view.get<SpriteComponent, SpriteAnimationComponent>(entity);
            spriteAnimation.currentFrame = (static_cast<int>(Timer::getTicks() - spriteAnimation.startTime) *
                                            spriteAnimation.framesPerSecond / 1000) %
                                           spriteAnimation.framesCount;
            sprite.textureArea.position.x =
                static_cast<float>(spriteAnimation.currentFrame) * sprite.textureArea.width;
        }
    }

    void CollisionSystem::update()
    {
        const auto view{getRegistry().view<const TransformComponent, BoxCollider2DComponent>()};
        for (const auto entity : view) {
            view.get<BoxCollider2DComponent>(entity).isColliding = false;
        }
        for (auto i{view.begin()}; i != view.end(); ++i) {
            const auto entity{*i};
            auto [transform, boxCollider] = view.get<TransformComponent, BoxCollider2DComponent>(entity);
            for (auto j{i}; j != view.end(); ++j) {
                if (i == j)
                    continue;
                const auto otherEntity{*j};
                auto [otherTransform, otherBoxCollider] =
                    view.get<TransformComponent, BoxCollider2DComponent>(otherEntity);
                const bool collisionHappened{aabbHasCollided(
                    Rect{glm::vec2{transform.position} + boxCollider.offset,
                         boxCollider.width * transform.scale.x, boxCollider.height * transform.scale.y},
                    Rect{glm::vec2{otherTransform.position} + otherBoxCollider.offset,
                         otherBoxCollider.width * otherTransform.scale.x,
                         otherBoxCollider.height * otherTransform.scale.y})};
                if (collisionHappened) {
                    boxCollider.isColliding = otherBoxCollider.isColliding = true;
                    Locator::getEventBus()->dispatchEvent<CollisionEvent>(entity, otherEntity);
                }
            }
        }
    }

    void PlayerInputSystem::start(InputHandler& inputHandler)
    {
        const auto view{getRegistry().view<PlayerInputComponent>()};
        StringId* defaultInputScope{};
        for (const auto entity : view) {
            auto& playerInput{view.get<PlayerInputComponent>(entity)};
            playerInput.inputDeviceId = inputHandler.acquireAvailableDevice();
            if (!defaultInputScope) {
                defaultInputScope = &playerInput.defaultInputScope;
            }
        }
        if (defaultInputScope) {
            inputHandler.switchScope(*defaultInputScope);
        }
    }

    void PlayerInputSystem::subscribeToEvents(EventBus& eventBus)
    {
        eventBus.addSubscriber<InputEvent, PlayerInputSystem>(this, &PlayerInputSystem::onInputCommand);
    }

    void PlayerInputSystem::onInputCommand(const InputEvent& event) const
    {
        const auto view{getRegistry().view<const PlayerInputComponent>()};
        for (const auto entity : view) {
            const auto& playerInput{view.get<PlayerInputComponent>(entity)};
            if (event.inputDeviceId == playerInput.inputDeviceId) {
                const InputCallback* callback{
                    playerInput.callbackMapping.getCommandCallback(event.commandId)};
                if (callback) {
                    callback->execute(event.inputValue);
                }
            }
        }
    }

    ScriptingSystem::ScriptingSystem(entt::registry* registry)
        : System{registry}
    {
        m_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math);
        const std::string packagePath{m_lua["package"]["path"]};
        m_lua["package"]["path"] = packagePath + (packagePath.empty() ? "" : ";") +
                                   (Filesystem::getScriptingLibraryPath() / "?.lua").string();
        m_lua.require_file("utils", Filesystem::getScriptingLibraryPath() / "utils.lua");
        m_lua.script_file(Filesystem::getScriptingLibraryPath() / "entity_script.lua");
        createScriptBindings();
        m_lua.create_named_table("scriptClasses");
        m_lua.create_named_table("scriptInstances");
    }

    ScriptingSystem::~ScriptingSystem() { m_scriptClasses.clear(); }

    std::unique_ptr<ScriptClass> ScriptingSystem::loadScriptClass(const std::filesystem::path& filepath,
                                                                  std::string_view className)
    {
        const auto absoluteFilepath{Filesystem::getResourcesPath() / filepath};
        const auto result{m_lua.script_file(absoluteFilepath, sol::script_pass_on_error)};
        if (!result.valid()) {
            const sol::error error{result};
            const sol::call_status status{result.status()};
            Locator::getLogger()->error("Error loading script {}: {} error\n\t{}", absoluteFilepath.c_str(),
                                        sol::to_string(status), error.what());
            return {};
        }
        const sol::optional<sol::table> maybeScriptClass{m_lua[className]};
        if (!maybeScriptClass) {
            Locator::getLogger()->warn("Script '{}' doesn't have '{}' class", filepath.c_str(), className);
            return {};
        }
        sol::table scriptClass{maybeScriptClass.value()};
        const sol::table entityScriptClass{m_lua["EntityScript"]};
        const auto isEntityScript{m_lua["utils"]["instance_of"](scriptClass, entityScriptClass)};
        if (!isEntityScript.get<bool>()) {
            Locator::getLogger()->warn("Script '{}' doesn't inherit from EntityScript", className);
            return {};
        }
        sol::table scriptClasses{m_lua["scriptClasses"]};
        scriptClasses[scriptClasses.size()] = scriptClass;
        return std::make_unique<ScriptClass>(className, scriptClass);
    }

    std::optional<ScriptInstance> ScriptingSystem::createScriptInstance(const std::filesystem::path& filepath,
                                                                        std::string_view className,
                                                                        entt::entity entity)
    {
        StringId scriptClassId{filepath.c_str()};
        ScriptClass* scriptClass{getScriptClass(scriptClassId)};
        if (!scriptClass) {
            if (std::unique_ptr<ScriptClass> newScriptClass{loadScriptClass(filepath, className)}) {
                storeScriptClass(scriptClassId, std::move(newScriptClass));
                scriptClass = getScriptClass(scriptClassId);
            } else {
                return {};
            }
        }
        const sol::optional<sol::function> maybeScriptClassConstructor{scriptClass->getConstructor()};
        if (!maybeScriptClassConstructor) {
            Locator::getLogger()->warn("Script '{}' doesn't have a constructor", scriptClass->getClassName());
            return {};
        }
        const sol::optional<sol::table> maybeScriptInstance{
            maybeScriptClassConstructor.value()(scriptClass->getLuaClass())};
        if (!maybeScriptInstance) {
            Locator::getLogger()->warn("Failed to instantiate '{}' script", scriptClass->getClassName());
            return {};
        }
        sol::table scriptInstance{maybeScriptInstance.value()};
        scriptInstance["entity"] = Entity{entity, &getRegistry()};
        sol::table scriptInstances{m_lua["scriptInstances"]};
        scriptInstances[scriptInstances.size()] = scriptInstance;
        return ScriptInstance{scriptClassId, scriptInstance};
    }

    void ScriptingSystem::start()
    {
        const auto view{getRegistry().view<ScriptComponent>()};
        for (const auto entity : view) {
            auto& scriptComponent = view.get<ScriptComponent>(entity);
            for (auto& script : scriptComponent.scriptInstances) {
                script.onStart();
            }
        }
    }

    void ScriptingSystem::update(float timeStep)
    {
        const auto view{getRegistry().view<ScriptComponent>()};
        for (const auto entity : view) {
            auto& scriptComponent = view.get<ScriptComponent>(entity);
            for (auto& script : scriptComponent.scriptInstances) {
                script.onUpdate(timeStep);
            }
        }
    }

    void ScriptingSystem::createScriptBindings()
    {
        m_lua.new_usertype<StringId>("StringId", "id", &StringId::getSid, "str", &StringId::getString);
        m_lua.new_usertype<Entity>("Entity", "id", sol::property(&Entity::getId));
        m_lua.new_usertype<glm::vec2>("Vec2", sol::constructors<glm::vec2(float), glm::vec2(float, float)>(),
                                      "x", &glm::vec2::x, "y", &glm::vec2::y);
        m_lua.new_usertype<glm::vec3>("Vec3",
                                      sol::constructors<glm::vec3(float), glm::vec3(float, float, float)>(),
                                      "x", &glm::vec3::x, "y", &glm::vec3::y, "z", &glm::vec3::z);
        m_lua.new_usertype<Rect>("Rect", "position", &Rect::position, "width", &Rect::width, "height",
                                 &Rect::height, "pivot_point", &Rect::pivotPoint);
        m_lua.new_usertype<IdComponent>("IdComponent", "sid", &IdComponent::sid);
        m_lua.new_usertype<TagComponent>("TagComponent", "name", &TagComponent::name);
        m_lua.new_usertype<TransformComponent>(
            "TransformComponent", "position", &TransformComponent::position, "scale",
            &TransformComponent::scale, "rotation", &TransformComponent::rotation);
        m_lua.new_usertype<RigidBody2DComponent>("RigidBody2DComponent", "velocity",
                                                 &RigidBody2DComponent::velocity);
        m_lua.new_usertype<SpriteComponent>("SpriteComponent", "texture_id", &SpriteComponent::textureId,
                                            "texture_area", &SpriteComponent::textureArea, "color",
                                            &SpriteComponent::color, "z_index", &SpriteComponent::zIndex);
        m_lua.new_usertype<SpriteAnimationComponent>(
            "SpriteAnimationComponent", "start_time", &SpriteAnimationComponent::startTime, "current_frame",
            &SpriteAnimationComponent::currentFrame, "frames_count", &SpriteAnimationComponent::framesCount,
            "frames_per_second", &SpriteAnimationComponent::framesPerSecond, "should_loop",
            &SpriteAnimationComponent::shouldLoop);
        m_lua.new_usertype<BoxCollider2DComponent>(
            "BoxCollider2DComponent", "offset", &BoxCollider2DComponent::offset, "width",
            &BoxCollider2DComponent::width, "height", &BoxCollider2DComponent::height, "is_colliding",
            &BoxCollider2DComponent::isColliding);
        m_lua.new_usertype<PlayerInputComponent>("PlayerInputComponent");
        m_lua.new_usertype<CameraComponent>("CameraComponent");
        m_lua.new_usertype<InputValue>("InputValue", "value", &InputValue::value);
        m_lua.set_function("api_get_component_id", ScriptingApi::getComponent<IdComponent>);
        m_lua.set_function("api_get_component_tag", ScriptingApi::getComponent<TagComponent>);
        m_lua.set_function("api_get_component_transform", ScriptingApi::getComponent<TransformComponent>);
        m_lua.set_function("api_get_component_rigidbody2d", ScriptingApi::getComponent<RigidBody2DComponent>);
        m_lua.set_function("api_get_component_sprite", ScriptingApi::getComponent<SpriteComponent>);
        m_lua.set_function("api_get_component_spriteanimation",
                           ScriptingApi::getComponent<SpriteAnimationComponent>);
        m_lua.set_function("api_get_component_boxcollider2d",
                           ScriptingApi::getComponent<BoxCollider2DComponent>);
        m_lua.set_function("api_get_component_playerinput", ScriptingApi::getComponent<PlayerInputComponent>);
        m_lua.set_function("api_get_component_script", ScriptingApi::getComponent<ScriptComponent>);
        m_lua.set_function("api_get_component_camera", ScriptingApi::getComponent<CameraComponent>);
    }

    void ScriptingSystem::storeScriptClass(const StringId& scriptId, std::unique_ptr<ScriptClass> scriptClass)
    {
        m_scriptClasses[scriptId] = std::move(scriptClass);
    }

    ScriptClass* ScriptingSystem::getScriptClass(const StringId& scriptId) const
    {
        const auto scriptIterator{m_scriptClasses.find(scriptId)};
        if (scriptIterator != m_scriptClasses.end()) {
            return scriptIterator->second.get();
        }
        return nullptr;
    }
} // namespace Engine
