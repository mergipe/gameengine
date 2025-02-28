#include "Systems.h"
#include "Components.h"
#include "Game.h"
#include "core/Timer.h"
#include "events/Events.h"
#include "renderer/Color.h"
#include "renderer/Rect.h"
#include "renderer/Renderer.h"
#include <SDL_keyboard.h>
#include <algorithm>

namespace Engine
{
    constexpr bool aabbHasCollided(double aX, double aY, double aW, double aH, double bX, double bY,
                                   double bW, double bH)
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
        return glm::vec2{position.x - static_cast<float>(camera.display.x),
                         position.y - static_cast<float>(camera.display.y)};
    }

    MovementSystem::MovementSystem()
        : System{}
    {
        requireComponent<TransformComponent>();
        requireComponent<RigidBodyComponent>();
    }

    void MovementSystem::update(float timeStep)
    {
        for (const auto& entity : getEntities()) {
            auto& transform{entity.getComponent<TransformComponent>()};
            const auto& rigidBody{entity.getComponent<RigidBodyComponent>()};
            transform.position += rigidBody.velocity * timeStep;
        }
    };

    SpriteRenderingSystem::SpriteRenderingSystem()
        : System{}
    {
        requireComponent<TransformComponent>();
        requireComponent<SpriteComponent>();
    }

    void SpriteRenderingSystem::update(Renderer& renderer, const ResourceManager& resourceManager,
                                       const Camera& camera, float frameExtrapolationTimeStep)
    {
        std::vector<Entity> entities{getEntities()};
        std::sort(entities.begin(), entities.end(), [](Entity a, Entity b) {
            return a.getComponent<SpriteComponent>().zIndex < b.getComponent<SpriteComponent>().zIndex;
        });
        for (const auto& entity : entities) {
            const auto& transform{entity.getComponent<TransformComponent>()};
            const auto& sprite{entity.getComponent<SpriteComponent>()};
            glm::vec2 renderPosition{transform.position};
            if (entity.hasComponent<RigidBodyComponent>()) {
                const auto& rigidBody{entity.getComponent<RigidBodyComponent>()};
                renderPosition = getExtrapolatedPosition(transform.position, rigidBody.velocity,
                                                         frameExtrapolationTimeStep);
            }
            if (!sprite.hasFixedPosition) {
                renderPosition = getRenderPosition(renderPosition, camera);
            }
            const FRect destinationRect{renderPosition.x, renderPosition.y,
                                        static_cast<float>(sprite.width) * transform.scale.x,
                                        static_cast<float>(sprite.height) * transform.scale.y};
            renderer.drawTexture(resourceManager.getTexture(sprite.resourceId), sprite.sourceRect,
                                 destinationRect, transform.rotation);
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
        for (const auto& entity : getEntities()) {
            auto& sprite{entity.getComponent<SpriteComponent>()};
            auto& animation{entity.getComponent<AnimationComponent>()};
            animation.currentFrame = (static_cast<int>(Timer::getTicks() - animation.startTime) *
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

    void CollisionSystem::update(EventBus& eventBus)
    {
        const auto entities{getEntities()};
        for (const auto& entity : entities) {
            entity.getComponent<BoxColliderComponent>().isColliding = false;
        }
        for (auto i{entities.begin()}; i != entities.end(); ++i) {
            const Entity entity{*i};
            const auto& transform{entity.getComponent<TransformComponent>()};
            auto& boxCollider{entity.getComponent<BoxColliderComponent>()};
            for (auto j{i + 1}; j != entities.end(); ++j) {
                const Entity otherEntity{*j};
                const auto& otherTransform{otherEntity.getComponent<TransformComponent>()};
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
                    eventBus.dispatchEvent<CollisionEvent>(entity, otherEntity);
                }
            }
        }
    }

    BoxColliderRenderingSystem::BoxColliderRenderingSystem()
        : System{}
    {
        requireComponent<TransformComponent>();
        requireComponent<BoxColliderComponent>();
    }

    void BoxColliderRenderingSystem::update(Renderer& renderer, const Camera& camera,
                                            float frameExtrapolationTimeStep)
    {
        for (const auto& entity : getEntities()) {
            const auto& transform{entity.getComponent<TransformComponent>()};
            const auto& boxCollider{entity.getComponent<BoxColliderComponent>()};
            glm::vec2 renderPosition{transform.position};
            if (entity.hasComponent<RigidBodyComponent>()) {
                const auto& rigidBody{entity.getComponent<RigidBodyComponent>()};
                renderPosition = getExtrapolatedPosition(transform.position, rigidBody.velocity,
                                                         frameExtrapolationTimeStep);
            }
            if (boxCollider.isColliding) {
                renderer.setDrawColor(Color{255, 0, 0, 255});
            } else {
                renderer.setDrawColor(Color{255, 255, 0, 255});
            }
            renderPosition = getRenderPosition(renderPosition, camera);
            renderer.drawRectangle(renderPosition.x + boxCollider.offset.x,
                                   renderPosition.y + boxCollider.offset.y,
                                   static_cast<float>(boxCollider.width) * transform.scale.x,
                                   static_cast<float>(boxCollider.height) * transform.scale.y);
        }
    }

    DamageSystem::DamageSystem()
        : System{}
    {
        requireComponent<BoxColliderComponent>();
    }

    void DamageSystem::subscribeToEvents(EventBus& eventBus)
    {
        eventBus.addSubscriber<CollisionEvent, DamageSystem>(this, &DamageSystem::onCollision);
    }

    void DamageSystem::onCollision(CollisionEvent& event)
    {
        Logger::trace("The damage system received an event collision between entities {} and {}",
                      event.entity.getId(), event.otherEntity.getId());
        // event.entity.kill();
        // event.otherEntity.kill();
    }

    KeyboardControlSystem::KeyboardControlSystem()
        : System{}
    {
        requireComponent<KeyboardControlComponent>();
        requireComponent<SpriteComponent>();
        requireComponent<RigidBodyComponent>();
    }

    void KeyboardControlSystem::subscribeToEvents(EventBus& eventBus)
    {
        eventBus.addSubscriber<KeyPressedEvent, KeyboardControlSystem>(this,
                                                                       &KeyboardControlSystem::onKeyPressed);
    }

    void KeyboardControlSystem::onKeyPressed(KeyPressedEvent& event)
    {
        for (const auto& entity : getEntities()) {
            const auto& keyboardControl{entity.getComponent<KeyboardControlComponent>()};
            auto& sprite{entity.getComponent<SpriteComponent>()};
            auto& rigidBody{entity.getComponent<RigidBodyComponent>()};
            switch (event.keyCode) {
            case SDLK_UP:
            case SDLK_w:
                rigidBody.velocity = keyboardControl.upVelocity;
                sprite.sourceRect.y = sprite.height * 0;
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                rigidBody.velocity = keyboardControl.rightVelocity;
                sprite.sourceRect.y = sprite.height * 1;
                break;
            case SDLK_DOWN:
            case SDLK_s:
                rigidBody.velocity = keyboardControl.downVelocity;
                sprite.sourceRect.y = sprite.height * 2;
                break;
            case SDLK_LEFT:
            case SDLK_a:
                rigidBody.velocity = keyboardControl.leftVelocity;
                sprite.sourceRect.y = sprite.height * 3;
                break;
            }
        }
    }

    CameraMovementSystem::CameraMovementSystem()
        : System{}
    {
        requireComponent<CameraFollowComponent>();
        requireComponent<TransformComponent>();
    }

    void CameraMovementSystem::update(SceneData& sceneData)
    {
        for (const auto& entity : getEntities()) {
            const auto& transform{entity.getComponent<TransformComponent>()};
            Rect& cameraDisplay{sceneData.camera.display};
            cameraDisplay.x = static_cast<int>(transform.position.x - (cameraDisplay.w / 2.0));
            cameraDisplay.y = static_cast<int>(transform.position.y - (cameraDisplay.h / 2.0));
            cameraDisplay.x = std::clamp(cameraDisplay.x, 0, sceneData.mapWidth - cameraDisplay.w);
            cameraDisplay.y = std::clamp(cameraDisplay.y, 0, sceneData.mapHeight - cameraDisplay.h);
        }
    }

    ProjectileEmitSystem::ProjectileEmitSystem()
        : System{}
    {
        requireComponent<ProjectileEmitterComponent>();
        requireComponent<TransformComponent>();
    }

    void ProjectileEmitSystem::update(Registry& registry)
    {
        for (const auto& entity : getEntities()) {
            auto& projectileEmitter{entity.getComponent<ProjectileEmitterComponent>()};
            const auto& transform{entity.getComponent<TransformComponent>()};
            if (static_cast<int>(Timer::getTicks() - projectileEmitter.lastEmissionTime) >
                projectileEmitter.repeatFrequency) {
                glm::vec2 projectilePosition{transform.position};
                if (entity.hasComponent<SpriteComponent>()) {
                    const auto& sprite{entity.getComponent<SpriteComponent>()};
                    projectilePosition.x += static_cast<float>(sprite.width) * transform.scale.x / 2.0f;
                    projectilePosition.y += static_cast<float>(sprite.height) * transform.scale.y / 2.0f;
                }
                Entity projectile{registry.createEntity()};
                projectile.addComponent<TransformComponent>(projectilePosition);
                projectile.addComponent<RigidBodyComponent>(projectileEmitter.projectileVelocity);
                projectile.addComponent<SpriteComponent>("bullet", 4, 4, 4);
                projectile.addComponent<BoxColliderComponent>(4, 4);
                projectileEmitter.lastEmissionTime = Timer::getTicks();
            }
        }
    }
} // namespace Engine
