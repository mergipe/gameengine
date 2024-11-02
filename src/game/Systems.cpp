#include "Systems.h"
#include "Components.h"
#include <algorithm>

namespace Engine
{
    constexpr glm::vec2 getExtrapolatedPosition(const glm::vec2& position, const glm::vec2& velocity,
                                                float extrapolationTimeStep)
    {
        return glm::vec2{position + velocity * extrapolationTimeStep};
    }

    MovementSystem::MovementSystem()
        : System{}
    {
        requireComponent<TransformComponent>();
        requireComponent<RigidBodyComponent>();
    }

    void MovementSystem::update(float timeStep)
    {
        for (const auto entity : getEntities()) {
            auto& transform{entity.getComponent<TransformComponent>()};
            const auto rigidBody{entity.getComponent<RigidBodyComponent>()};
            transform.position += rigidBody.velocity * timeStep;
        }
    };

    SpriteRenderingSystem::SpriteRenderingSystem()
        : System{}
    {
        requireComponent<TransformComponent>();
        requireComponent<SpriteComponent>();
    }

    void SpriteRenderingSystem::update(SDL_Renderer* renderer, const ResourceManager& resourceManager,
                                       float frameExtrapolationTimeStep)
    {
        std::vector<Entity> entities{getEntities()};
        std::sort(entities.begin(), entities.end(), [](Entity a, Entity b) {
            return a.getComponent<SpriteComponent>().zIndex < b.getComponent<SpriteComponent>().zIndex;
        });
        for (const auto entity : entities) {
            const auto transform{entity.getComponent<TransformComponent>()};
            const auto sprite{entity.getComponent<SpriteComponent>()};
            glm::vec2 renderPosition{transform.position};
            if (entity.hasComponent<RigidBodyComponent>()) {
                const auto rigidBody{entity.getComponent<RigidBodyComponent>()};
                renderPosition = getExtrapolatedPosition(transform.position, rigidBody.velocity,
                                                         frameExtrapolationTimeStep);
            }
            const SDL_FRect destinationRect{renderPosition.x, renderPosition.y,
                                            static_cast<float>(sprite.width) * transform.scale.x,
                                            static_cast<float>(sprite.height) * transform.scale.y};
            SDL_RenderCopyExF(renderer, resourceManager.getTexture(sprite.resourceId), &sprite.sourceRect,
                              &destinationRect, transform.rotation, nullptr, SDL_FLIP_NONE);
        }
    }

    AnimationSystem::AnimationSystem()
        : System{}
    {
        requireComponent<SpriteComponent>();
        requireComponent<AnimationComponent>();
    }

    void AnimationSystem::update()
    {
        for (const auto entity : getEntities()) {
            auto& sprite{entity.getComponent<SpriteComponent>()};
            auto& animation{entity.getComponent<AnimationComponent>()};
            animation.currentFrame = (static_cast<int>(SDL_GetTicks64() - animation.startTime) *
                                      animation.framesPerSecond / 1000) %
                                     animation.framesCount;
            sprite.sourceRect.x = animation.currentFrame * sprite.width;
        }
    }

    CollisionSystem::CollisionSystem()
        : System{}
    {
        requireComponent<TransformComponent>();
        requireComponent<BoxColliderComponent>();
    }

    void CollisionSystem::update()
    {
        const auto entities{getEntities()};
        for (const auto entity : entities) {
            entity.getComponent<BoxColliderComponent>().isColliding = false;
        }
        for (auto i{entities.begin()}; i != entities.end(); ++i) {
            const Entity entity{*i};
            const auto transform{entity.getComponent<TransformComponent>()};
            auto& boxCollider{entity.getComponent<BoxColliderComponent>()};
            for (auto j{i + 1}; j != entities.end(); ++j) {
                const Entity otherEntity{*j};
                const auto otherTransform{otherEntity.getComponent<TransformComponent>()};
                auto& otherBoxCollider{otherEntity.getComponent<BoxColliderComponent>()};
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
                    Logger::trace("Entity {} is colliding with entity {}", entity.getId(),
                                  otherEntity.getId());
                }
            }
        }
    }

    constexpr bool CollisionSystem::aabbHasCollided(double aX, double aY, double aW, double aH, double bX,
                                                    double bY, double bW, double bH)
    {
        return (aX < bX + bW && aX + aW > bX && aY < bY + bH && aY + aH > bY);
    }

    BoxColliderRenderingSystem::BoxColliderRenderingSystem()
        : System{}
    {
        requireComponent<TransformComponent>();
        requireComponent<BoxColliderComponent>();
    }

    void BoxColliderRenderingSystem::update(SDL_Renderer* renderer, float frameExtrapolationTimeStep)
    {
        for (const auto entity : getEntities()) {
            const auto transform{entity.getComponent<TransformComponent>()};
            const auto boxCollider{entity.getComponent<BoxColliderComponent>()};
            glm::vec2 renderPosition{transform.position};
            if (entity.hasComponent<RigidBodyComponent>()) {
                const auto rigidBody{entity.getComponent<RigidBodyComponent>()};
                renderPosition = getExtrapolatedPosition(transform.position, rigidBody.velocity,
                                                         frameExtrapolationTimeStep);
            }
            const SDL_FRect colliderRect{renderPosition.x + boxCollider.offset.x,
                                         renderPosition.y + boxCollider.offset.y,
                                         static_cast<float>(boxCollider.width) * transform.scale.x,
                                         static_cast<float>(boxCollider.height) * transform.scale.y};
            if (boxCollider.isColliding) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            }
            SDL_RenderDrawRectF(renderer, &colliderRect);
        }
    }
} // namespace Engine
