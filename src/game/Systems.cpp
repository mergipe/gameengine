#include "Systems.h"
#include "Components.h"
#include "Game.h"
#include "core/Timer.h"
#include "events/Events.h"
#include "renderer/Renderer2D.h"
#include "resources/ResourceManager.h"

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
        Rect cameraPlaneGeometry{camera.getNearPlaneGeometry()};
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
    };

    void RenderingSystem::update(Renderer2D& renderer, const ResourceManager& resourceManager,
                                 float frameExtrapolationTimeStep)
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
            const auto* rigidBody{getRegistry().try_get<RigidBody2DComponent>(entity)};
            if (rigidBody) {
                renderPosition = getExtrapolatedPosition(transform.position, rigidBody->velocity,
                                                         frameExtrapolationTimeStep);
            }
            const float spriteWidth{sprite.textureArea.width * transform.scale.x};
            const float spriteHeight{sprite.textureArea.height * transform.scale.y};
            const Rect spriteGeometry{renderPosition, spriteWidth, spriteHeight};
            if (!isOutsideOrthoCameraView(*cameraPtr, spriteGeometry)) {
                renderer.drawSprite(spriteGeometry, transform.rotation,
                                    resourceManager.getTexture(sprite.textureId), sprite.textureArea,
                                    sprite.color);
            }
        }
    }

    void DebugRenderingSystem::update(Renderer2D& renderer, float frameExtrapolationTimeStep)
    {
        const auto view{getRegistry().view<const TransformComponent, const BoxCollider2DComponent>()};
        for (const auto entity : view) {
            auto [transform, boxCollider] = view.get<TransformComponent, BoxCollider2DComponent>(entity);
            glm::vec2 renderPosition{transform.position};
            const auto* rigidBody{getRegistry().try_get<RigidBody2DComponent>(entity)};
            if (rigidBody) {
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
                         static_cast<float>(boxCollider.width) * transform.scale.x,
                         static_cast<float>(boxCollider.height) * transform.scale.y},
                    Rect{glm::vec2{otherTransform.position} + otherBoxCollider.offset,
                         static_cast<float>(otherBoxCollider.width) * otherTransform.scale.x,
                         static_cast<float>(otherBoxCollider.height) * otherTransform.scale.y})};
                if (collisionHappened) {
                    boxCollider.isColliding = otherBoxCollider.isColliding = true;
                    Game::instance().getEventBus().dispatchEvent<CollisionEvent>(entity, otherEntity);
                }
            }
        }
    }

    void ScriptSystem::update(float timeStep)
    {
        const auto view{getRegistry().view<const ScriptComponent>()};
        const Timer::Ticks elapsedTime{Timer::getTicks()};
        for (const auto entity : view) {
            const auto& script{view.get<ScriptComponent>(entity)};
            script.func(entity, timeStep, elapsedTime);
        }
    }

    void ScriptSystem::createScriptBindings(sol::state& lua)
    {
        lua.new_usertype<entt::entity>("Entity");
        lua.new_usertype<glm::vec3>("Vec3",
                                    sol::constructors<glm::vec3(float), glm::vec3(float, float, float)>(),
                                    "x", &glm::vec3::x, "y", &glm::vec3::y, "z", &glm::vec3::z);
        lua.set_function("get_position", &ScriptSystem::getEntityPosition, this);
        lua.set_function("get_velocity", &ScriptSystem::getEntityVelocity, this);
        lua.set_function("set_position", &ScriptSystem::setEntityPosition, this);
        lua.set_function("set_velocity", &ScriptSystem::setEntityVelocity, this);
        lua.set_function("set_rotation", &ScriptSystem::setEntityRotation, this);
    }

    glm::vec3 ScriptSystem::getEntityPosition(entt::entity entity)
    {
        auto* transform{getRegistry().try_get<TransformComponent>(entity)};
        if (transform) {
            return transform->position;
        }
        return {};
    }

    glm::vec3 ScriptSystem::getEntityVelocity(entt::entity entity)
    {
        auto* rigidBody{getRegistry().try_get<RigidBody2DComponent>(entity)};
        if (rigidBody) {
            return rigidBody->velocity;
        }
        return {};
    }

    void ScriptSystem::setEntityPosition(entt::entity entity, glm::vec3 position)
    {
        auto* transform{getRegistry().try_get<TransformComponent>(entity)};
        if (transform) {
            transform->position = position;
        }
    }

    void ScriptSystem::setEntityVelocity(entt::entity entity, glm::vec3 velocity)
    {
        auto* rigidBody{getRegistry().try_get<RigidBody2DComponent>(entity)};
        if (rigidBody) {
            rigidBody->velocity = velocity;
        }
    }

    void ScriptSystem::setEntityRotation(entt::entity entity, glm::vec3 rotation)
    {
        auto* transform{getRegistry().try_get<TransformComponent>(entity)};
        if (transform) {
            transform->rotation = rotation;
        }
    }
} // namespace Engine
