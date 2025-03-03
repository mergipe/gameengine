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

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

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
    {
        requireComponent<TransformComponent>();
        requireComponent<SpriteComponent>();
    }

    void SpriteRenderingSystem::update(Renderer& renderer, const AssetManager& assetManager,
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
            renderer.drawTexture(assetManager.getTexture(sprite.assetId), sprite.sourceRect, destinationRect,
                                 0.0);
        }
    }

    AnimationSystem::AnimationSystem()
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
            renderer.drawRectangle({renderPosition.x + boxCollider.offset.x,
                                    renderPosition.y + boxCollider.offset.y,
                                    static_cast<float>(boxCollider.width) * transform.scale.x,
                                    static_cast<float>(boxCollider.height) * transform.scale.y},
                                   false);
        }
    }

    DamageSystem::DamageSystem() { requireComponent<BoxColliderComponent>(); }

    void DamageSystem::subscribeToEvents(EventBus& eventBus)
    {
        eventBus.addSubscriber<CollisionEvent, DamageSystem>(this, &DamageSystem::onCollision);
    }

    void DamageSystem::onCollision(CollisionEvent& event)
    {
        Logger::trace("Collision between entities {} and {}", event.entity.getId(),
                      event.otherEntity.getId());
        if (event.entity.belongsToGroup("enemy-projectiles") && event.otherEntity.hasTag("player")) {
            onProjectileHitsEntity(event.entity, event.otherEntity);
        } else if (event.entity.hasTag("player") && event.otherEntity.belongsToGroup("enemy-projectiles")) {
            onProjectileHitsEntity(event.otherEntity, event.entity);
        } else if (event.entity.belongsToGroup("player-projectiles") &&
                   event.otherEntity.belongsToGroup("enemies")) {
            onProjectileHitsEntity(event.entity, event.otherEntity);
        } else if (event.entity.belongsToGroup("enemies") &&
                   event.otherEntity.belongsToGroup("player-projectiles")) {
            onProjectileHitsEntity(event.otherEntity, event.entity);
        }
    }

    void DamageSystem::onProjectileHitsEntity(Entity& projectile, Entity& entity)
    {
        auto& health{entity.getComponent<HealthComponent>()};
        const auto& damage{projectile.getComponent<DamageComponent>()};
        health.healthPercentage -= damage.value;
        if (health.healthPercentage <= 0)
            entity.kill();
        projectile.kill();
    }

    PlayerInputSystem::PlayerInputSystem() { requireComponent<PlayerInputComponent>(); }

    void PlayerInputSystem::subscribeToEvents(EventBus& eventBus)
    {
        eventBus.addSubscriber<KeyPressedEvent, PlayerInputSystem>(this, &PlayerInputSystem::onKeyPressed);
    }

    void PlayerInputSystem::onKeyPressed(KeyPressedEvent& event)
    {
        float velocityMagnitude{0.2f};
        for (const auto& entity : getEntities()) {
            switch (event.keyCode) {
            case SDLK_UP:
            case SDLK_w:
                move(entity, glm::vec2{0, -velocityMagnitude}, 0);
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                move(entity, glm::vec2{velocityMagnitude, 0}, 1);
                break;
            case SDLK_DOWN:
            case SDLK_s:
                move(entity, glm::vec2{0, velocityMagnitude}, 2);
                break;
            case SDLK_LEFT:
            case SDLK_a:
                move(entity, glm::vec2{-velocityMagnitude, 0}, 3);
                break;
            case SDLK_SPACE:
                if (entity.hasComponent<ProjectileEmitterComponent>()) {
                    auto& projectileEmitter{entity.getComponent<ProjectileEmitterComponent>()};
                    if (projectileEmitter.canShoot()) {
                        glm::vec2 projectileVelocity{0, -projectileEmitter.projectileVelocity.y};
                        if (entity.hasComponent<TransformComponent>()) {
                            const auto& transform{entity.getComponent<TransformComponent>()};
                            projectileVelocity =
                                glm::rotate(projectileVelocity, glm::radians(transform.rotation));
                        }
                        Game::instance().getEventBus().dispatchEvent<ProjectileEmitEvent>(
                            entity, projectileVelocity, projectileEmitter.projectileDuration,
                            projectileEmitter.hitPercentDamage, true);
                    }
                }
                break;
            }
        }
    }

    void PlayerInputSystem::move(const Entity& entity, glm::vec2 velocity, int spriteIndex)
    {
        if (entity.hasComponent<RigidBodyComponent>()) {
            auto& rigidBody{entity.getComponent<RigidBodyComponent>()};
            rigidBody.velocity = velocity;
        }
        if (entity.hasComponent<SpriteComponent>()) {
            auto& sprite{entity.getComponent<SpriteComponent>()};
            sprite.sourceRect.y = sprite.height * spriteIndex;
        }
        if (entity.hasComponent<TransformComponent>()) {
            auto& transform{entity.getComponent<TransformComponent>()};
            transform.rotation = static_cast<float>(spriteIndex) * 90.0f;
        }
    }

    CameraMovementSystem::CameraMovementSystem()
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
    {
        requireComponent<ProjectileEmitterComponent>();
        requireComponent<TransformComponent>();
    }

    void ProjectileEmitSystem::subscribeToEvents(EventBus& eventBus)
    {
        eventBus.addSubscriber<ProjectileEmitEvent, ProjectileEmitSystem>(
            this, &ProjectileEmitSystem::onEmitProjectile);
    }

    void ProjectileEmitSystem::update()
    {
        for (const auto& entity : getEntities()) {
            auto& projectileEmitter{entity.getComponent<ProjectileEmitterComponent>()};
            if (projectileEmitter.isAutoShoot && projectileEmitter.canShoot()) {
                emitProjectile(entity, projectileEmitter.projectileVelocity,
                               projectileEmitter.projectileDuration, projectileEmitter.hitPercentDamage,
                               projectileEmitter.isProjectileFriendly);
            }
        }
    }

    void ProjectileEmitSystem::emitProjectile(const Entity& entity, glm::vec2 velocity, int duration,
                                              int damage, bool isFriendly)
    {
        const auto& transform{entity.getComponent<TransformComponent>()};
        glm::vec2 projectilePosition{transform.position};
        if (entity.hasComponent<SpriteComponent>()) {
            const auto& sprite{entity.getComponent<SpriteComponent>()};
            projectilePosition.x += static_cast<float>(sprite.width) * transform.scale.x / 2.0f;
            projectilePosition.y += static_cast<float>(sprite.height) * transform.scale.y / 2.0f;
        }
        Entity projectile{entity.getRegistry().createEntity()};
        std::string group{isFriendly ? "player-projectiles" : "enemy-projectiles"};
        projectile.group(group);
        projectile.addComponent<TransformComponent>(projectilePosition);
        projectile.addComponent<RigidBodyComponent>(velocity);
        projectile.addComponent<SpriteComponent>("bullet", 4, 4, 4);
        projectile.addComponent<BoxColliderComponent>(4, 4);
        projectile.addComponent<LifecycleComponent>(duration);
        projectile.addComponent<DamageComponent>(damage);
        auto& projectileEmitter{entity.getComponent<ProjectileEmitterComponent>()};
        projectileEmitter.lastEmissionTime = Timer::getTicks();
    }

    void ProjectileEmitSystem::onEmitProjectile(ProjectileEmitEvent& event)
    {
        emitProjectile(event.entity, event.velocity, event.projectileDuration, event.projectileDamage,
                       event.isProjectileFriendly);
    }

    LifecycleSystem::LifecycleSystem() { requireComponent<LifecycleComponent>(); }

    void LifecycleSystem::update()
    {
        for (auto& entity : getEntities()) {
            const auto& lifecycle{entity.getComponent<LifecycleComponent>()};
            if (static_cast<int>(Timer::getTicks() - lifecycle.startTime) > lifecycle.duration) {
                entity.kill();
            }
        }
    }

    TextRenderingSystem::TextRenderingSystem()
    {
        requireComponent<TransformComponent>();
        requireComponent<TextComponent>();
    }

    void TextRenderingSystem::update(Renderer& renderer, const AssetManager& assetManager,
                                     const Camera& camera)
    {
        for (const auto& entity : getEntities()) {
            const auto& transform{entity.getComponent<TransformComponent>()};
            const auto& text{entity.getComponent<TextComponent>()};
            const Font& font{assetManager.getFont(text.assetId)};
            glm::vec2 renderPosition{transform.position};
            if (!text.hasFixedPosition) {
                renderPosition = getRenderPosition(renderPosition, camera);
            }
            renderer.drawText(font, text.text, text.color, renderPosition);
        }
    }

    HealthBarRenderingSystem::HealthBarRenderingSystem()
    {
        requireComponent<TransformComponent>();
        requireComponent<SpriteComponent>();
        requireComponent<HealthComponent>();
    }

    void HealthBarRenderingSystem::update(Renderer& renderer, const Font& font, const Camera& camera)
    {
        for (const auto& entity : getEntities()) {
            const auto& transform{entity.getComponent<TransformComponent>()};
            const auto& sprite{entity.getComponent<SpriteComponent>()};
            const auto& health{entity.getComponent<HealthComponent>()};
            Color color;
            if (health.healthPercentage <= 20) {
                color = {255, 0, 0, 255};
            } else if (health.healthPercentage <= 70) {
                color = {255, 165, 0, 255};
            } else {
                color = {0, 255, 0, 255};
            }
            std::string text{std::to_string(health.healthPercentage) + '%'};
            int barHeight{4};
            glm::vec2 entityPosition{getRenderPosition(transform.position, camera)};
            glm::vec2 barPosition{entityPosition.x, entityPosition.y - 2.0f - static_cast<float>(barHeight)};
            glm::vec2 textPosition{barPosition.x, barPosition.y - 18.0f};
            renderer.setDrawColor(color);
            renderer.drawText(font, text, color, textPosition);
            renderer.drawRectangle({barPosition.x, barPosition.y,
                                    static_cast<float>(sprite.width) * transform.scale.x *
                                        static_cast<float>(health.healthPercentage) / 100.0f,
                                    static_cast<float>(barHeight)},
                                   true);
        }
    }
} // namespace Engine
