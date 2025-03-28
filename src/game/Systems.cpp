#include "Systems.h"
#include "Components.h"
#include "Game.h"
#include "core/Timer.h"
#include "events/Events.h"
#include "renderer/Renderer2D.h"
#include <SDL3/SDL.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

namespace Engine
{
    constexpr bool aabbHasCollided(float aX, float aY, float aW, float aH, float bX, float bY, float bW,
                                   float bH)
    {
        return (aX < bX + bW && aX + aW > bX && aY < bY + bH && aY + aH > bY);
    }

    constexpr glm::vec2 getExtrapolatedPosition(const glm::vec2& position, const glm::vec2& velocity,
                                                float extrapolationTimeStep)
    {
        return glm::vec2{position + velocity * extrapolationTimeStep};
    }

    constexpr glm::vec2 getRenderPosition(const glm::vec2& position, const Camera& camera)
    {
        return position - camera.position;
    }

    void MovementSystem::update(float timeStep)
    {
        auto view{getRegistry().view<TransformComponent, RigidBody2DComponent>()};
        for (auto entity : view) {
            auto& transform{view.get<TransformComponent>(entity)};
            auto& rigidBody{view.get<RigidBody2DComponent>(entity)};
            transform.position += rigidBody.velocity * timeStep;
        }
    };

    void SpriteRenderingSystem::update(Renderer2D& renderer, const ResourceManager& resourceManager,
                                       const Camera& camera, float frameExtrapolationTimeStep)
    {
        getRegistry().sort<SpriteComponent>(
            [](const SpriteComponent& lhs, const SpriteComponent& rhs) { return lhs.zIndex < rhs.zIndex; });
        auto view{getRegistry().view<const TransformComponent, const SpriteComponent>()};
        view.use<SpriteComponent>();
        for (auto entity : view) {
            const auto& transform{view.get<TransformComponent>(entity)};
            const auto& sprite{view.get<SpriteComponent>(entity)};
            glm::vec2 renderPosition{transform.position};
            const auto* rigidBody{getRegistry().try_get<RigidBody2DComponent>(entity)};
            if (rigidBody) {
                renderPosition = getExtrapolatedPosition(transform.position, rigidBody->velocity,
                                                         frameExtrapolationTimeStep);
            }
            if (!sprite.hasFixedPosition) {
                renderPosition = getRenderPosition(renderPosition, camera);
            }
            float spriteWidth{sprite.textureArea.width * transform.scale.x};
            float spriteHeight{sprite.textureArea.height * transform.scale.y};
            if (renderPosition.x + spriteWidth < 0 || renderPosition.x > camera.width ||
                renderPosition.y + spriteHeight < 0 || renderPosition.y > camera.height) {
                continue;
            }
            renderer.drawSprite(renderPosition, spriteWidth, spriteHeight, 0,
                                resourceManager.getTexture(sprite.textureId), sprite.textureArea,
                                sprite.color);
        }
    }

    void SpriteAnimationSystem::update()
    {
        auto view{getRegistry().view<SpriteComponent, SpriteAnimationComponent>()};
        for (auto entity : view) {
            auto& sprite{view.get<SpriteComponent>(entity)};
            auto& animation{view.get<SpriteAnimationComponent>(entity)};
            animation.currentFrame = (static_cast<int>(Timer::getTicks() - animation.startTime) *
                                      animation.framesPerSecond / 1000) %
                                     animation.framesCount;
            sprite.textureArea.position.x =
                static_cast<float>(animation.currentFrame) * sprite.textureArea.width;
        }
    }

    void CollisionSystem::update()
    {
        auto view{getRegistry().view<const TransformComponent, BoxCollider2DComponent>()};
        for (auto entity : view) {
            view.get<BoxCollider2DComponent>(entity).isColliding = false;
        }
        for (auto i{view.begin()}; i != view.end(); ++i) {
            auto entity{*i};
            const auto& transform{view.get<TransformComponent>(entity)};
            auto& boxCollider{view.get<BoxCollider2DComponent>(entity)};
            for (auto j{i}; j != view.end(); ++j) {
                if (i == j)
                    continue;
                auto otherEntity{*j};
                const auto& otherTransform{view.get<TransformComponent>(otherEntity)};
                auto& otherBoxCollider{view.get<BoxCollider2DComponent>(otherEntity)};
                const bool collisionHappened{aabbHasCollided(
                    transform.position.x + boxCollider.offset.x, transform.position.y + boxCollider.offset.y,
                    static_cast<float>(boxCollider.width) * transform.scale.x,
                    static_cast<float>(boxCollider.height) * transform.scale.y,
                    otherTransform.position.x + otherBoxCollider.offset.x,
                    otherTransform.position.y + otherBoxCollider.offset.y,
                    static_cast<float>(otherBoxCollider.width) * otherTransform.scale.x,
                    static_cast<float>(otherBoxCollider.height) * otherTransform.scale.y)};
                if (collisionHappened) {
                    boxCollider.isColliding = otherBoxCollider.isColliding = true;
                    Game::instance().getEventBus().dispatchEvent<CollisionEvent>(entity, otherEntity);
                }
            }
        }
    }

    void BoxColliderRenderingSystem::update(Renderer2D& renderer, const Camera& camera,
                                            float frameExtrapolationTimeStep)
    {
        auto view{getRegistry().view<const TransformComponent, const BoxCollider2DComponent>()};
        for (auto entity : view) {
            const auto& transform{view.get<TransformComponent>(entity)};
            const auto& boxCollider{view.get<BoxCollider2DComponent>(entity)};
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
            renderPosition = getRenderPosition(renderPosition, camera);
            Rect rect{renderPosition + boxCollider.offset,
                      static_cast<float>(boxCollider.width) * transform.scale.x,
                      static_cast<float>(boxCollider.height) * transform.scale.y};
            renderer.drawRectangle(rect, color, 0);
        }
    }

    void PlayerInputSystem::subscribeToEvents(EventBus& eventBus)
    {
        eventBus.addSubscriber<KeyPressedEvent, PlayerInputSystem>(this, &PlayerInputSystem::onKeyPressed);
    }

    void PlayerInputSystem::onKeyPressed(KeyPressedEvent& event)
    {
        float velocityMagnitude{0.2f};
        auto view{getRegistry().view<const PlayerInputComponent>()};
        for (auto entity : view) {
            switch (event.keyCode) {
            case SDLK_UP:
            case SDLK_W:
                move(entity, glm::vec2{0, -velocityMagnitude}, 0);
                break;
            case SDLK_RIGHT:
            case SDLK_D:
                move(entity, glm::vec2{velocityMagnitude, 0}, 1);
                break;
            case SDLK_DOWN:
            case SDLK_S:
                move(entity, glm::vec2{0, velocityMagnitude}, 2);
                break;
            case SDLK_LEFT:
            case SDLK_A:
                move(entity, glm::vec2{-velocityMagnitude, 0}, 3);
                break;
            }
        }
    }

    void PlayerInputSystem::move(const entt::entity& entity, glm::vec2 velocity, int spriteIndex)
    {
        auto* rigidBody{getRegistry().try_get<RigidBody2DComponent>(entity)};
        if (rigidBody) {
            rigidBody->velocity = glm::vec3{velocity, 0.0f};
        }
        auto* sprite{getRegistry().try_get<SpriteComponent>(entity)};
        if (sprite) {
            sprite->textureArea.position.y = sprite->textureArea.height * static_cast<float>(spriteIndex);
        }
        auto* transform{getRegistry().try_get<TransformComponent>(entity)};
        if (transform) {
            transform->rotation.z = static_cast<float>(spriteIndex) * 90.0f;
        }
    }

    void ScriptSystem::update(float timeStep)
    {
        auto view{getRegistry().view<const ScriptComponent>()};
        Timer::Ticks elapsedTime{Timer::getTicks()};
        for (auto entity : view) {
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
