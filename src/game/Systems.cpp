#include "Systems.h"
#include "Components.h"
#include "Game.h"
#include "core/Timer.h"
#include "events/Events.h"
#include "renderer/Color.h"
#include "renderer/Rect.h"
#include "renderer/Renderer.h"
#include <SDL3/SDL.h>
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

    constexpr bool isObjectOutOfMap(const glm::vec2& position, float objectWidth, float objectHeight,
                                    float mapWidth, float mapHeight)
    {
        return position.x + objectWidth < 0 || position.x > mapWidth || position.y + objectHeight < 0 ||
               position.y > mapHeight;
    }

    void MovementSystem::update(float timeStep, const SceneData& sceneData)
    {
        auto view{getRegistry().view<TransformComponent, RigidBodyComponent>()};
        for (auto entity : view) {
            auto& transform{view.get<TransformComponent>(entity)};
            auto& rigidBody{view.get<RigidBodyComponent>(entity)};
            transform.position += rigidBody.velocity * timeStep;
            const auto* sprite{getRegistry().try_get<SpriteComponent>(entity)};
            float width{0.0f};
            float height{0.0f};
            if (sprite) {
                width = sprite->width * transform.scale.x;
                height = sprite->height * transform.scale.y;
            }
            bool isPlayer = getRegistry().any_of<Player>(entity);
            float mapWidth{sceneData.levelData.mapData.width};
            float mapHeight{sceneData.levelData.mapData.height};
            if (isPlayer) {
                transform.position.x = std::clamp(transform.position.x, 0.0f, mapWidth - width);
                transform.position.y = std::clamp(transform.position.y, 0.0f, mapHeight - height);
            } else if (isObjectOutOfMap(transform.position, width, height, mapWidth, mapHeight)) {
                getRegistry().destroy(entity);
            }
        }
    };

    void SpriteRenderingSystem::update(Renderer& renderer, const AssetManager& assetManager,
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
            const auto* rigidBody{getRegistry().try_get<RigidBodyComponent>(entity)};
            if (rigidBody) {
                renderPosition = getExtrapolatedPosition(transform.position, rigidBody->velocity,
                                                         frameExtrapolationTimeStep);
            }
            if (!sprite.hasFixedPosition) {
                renderPosition = getRenderPosition(renderPosition, camera);
            }
            float spriteWidth{static_cast<float>(sprite.width) * transform.scale.x};
            float spriteHeight{static_cast<float>(sprite.height) * transform.scale.y};
            if (renderPosition.x + spriteWidth < 0 ||
                renderPosition.x > static_cast<float>(camera.display.w) ||
                renderPosition.y + spriteHeight < 0 ||
                renderPosition.y > static_cast<float>(camera.display.h)) {
                continue;
            }
            const FRect destinationRect{renderPosition.x, renderPosition.y, spriteWidth, spriteHeight};
            renderer.drawTexture(assetManager.getTexture(sprite.textureId), sprite.sourceRect,
                                 destinationRect, 0.0);
        }
    }

    void AnimationSystem::update()
    {
        auto view{getRegistry().view<SpriteComponent, AnimationComponent>()};
        for (auto entity : view) {
            auto& sprite{view.get<SpriteComponent>(entity)};
            auto& animation{view.get<AnimationComponent>(entity)};
            animation.currentFrame = (static_cast<int>(Timer::getTicks() - animation.startTime) *
                                      animation.framesPerSecond / 1000) %
                                     animation.framesCount;
            sprite.sourceRect.x = static_cast<float>(animation.currentFrame) * sprite.width;
        }
    }

    void CollisionSystem::update()
    {
        auto view{getRegistry().view<const TransformComponent, BoxColliderComponent>()};
        for (auto entity : view) {
            view.get<BoxColliderComponent>(entity).isColliding = false;
        }
        for (auto i{view.begin()}; i != view.end(); ++i) {
            auto entity{*i};
            const auto& transform{view.get<TransformComponent>(entity)};
            auto& boxCollider{view.get<BoxColliderComponent>(entity)};
            for (auto j{i}; j != view.end(); ++j) {
                if (i == j)
                    continue;
                auto otherEntity{*j};
                const auto& otherTransform{view.get<TransformComponent>(otherEntity)};
                auto& otherBoxCollider{view.get<BoxColliderComponent>(otherEntity)};
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

    void BoxColliderRenderingSystem::update(Renderer& renderer, const Camera& camera,
                                            float frameExtrapolationTimeStep)
    {
        auto view{getRegistry().view<const TransformComponent, const BoxColliderComponent>()};
        for (auto entity : view) {
            const auto& transform{view.get<TransformComponent>(entity)};
            const auto& boxCollider{view.get<BoxColliderComponent>(entity)};
            glm::vec2 renderPosition{transform.position};
            const auto* rigidBody{getRegistry().try_get<RigidBodyComponent>(entity)};
            if (rigidBody) {
                renderPosition = getExtrapolatedPosition(transform.position, rigidBody->velocity,
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

    void DamageSystem::subscribeToEvents(EventBus& eventBus)
    {
        eventBus.addSubscriber<CollisionEvent, DamageSystem>(this, &DamageSystem::onCollision);
    }

    void DamageSystem::onCollision(CollisionEvent& event)
    {
        if (getRegistry().any_of<EnemyProjectile>(event.entity) &&
            getRegistry().any_of<Player>(event.otherEntity)) {
            onProjectileHitsEntity(event.entity, event.otherEntity);
        } else if (getRegistry().any_of<Player>(event.entity) &&
                   getRegistry().any_of<EnemyProjectile>(event.otherEntity)) {
            onProjectileHitsEntity(event.otherEntity, event.entity);
        } else if (getRegistry().any_of<PlayerProjectile>(event.entity) &&
                   getRegistry().any_of<Enemy>(event.otherEntity)) {
            onProjectileHitsEntity(event.entity, event.otherEntity);
        } else if (getRegistry().any_of<Enemy>(event.entity) &&
                   getRegistry().any_of<PlayerProjectile>(event.otherEntity)) {
            onProjectileHitsEntity(event.otherEntity, event.entity);
        }
    }

    void DamageSystem::onProjectileHitsEntity(entt::entity& projectile, entt::entity& entity)
    {
        auto* health{getRegistry().try_get<HealthComponent>(entity)};
        const auto* damage{getRegistry().try_get<DamageComponent>(projectile)};
        if (!health || !damage)
            return;
        health->healthPercentage -= damage->value;
        if (health->healthPercentage <= 0)
            getRegistry().destroy(entity);
        getRegistry().destroy(projectile);
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
            case SDLK_SPACE:
                const auto* projectileEmitter{getRegistry().try_get<ProjectileEmitterComponent>(entity)};
                if (projectileEmitter) {
                    if (projectileEmitter->canShoot()) {
                        glm::vec2 projectileVelocity{0, -projectileEmitter->projectileVelocity.y};
                        const auto* transform{getRegistry().try_get<TransformComponent>(entity)};
                        if (transform) {
                            projectileVelocity =
                                glm::rotate(projectileVelocity, glm::radians(transform->rotation));
                        }
                        Game::instance().getEventBus().dispatchEvent<ProjectileEmitEvent>(
                            entity, projectileVelocity, projectileEmitter->projectileDuration,
                            projectileEmitter->hitPercentageDamage, true);
                    }
                }
                break;
            }
        }
    }

    void PlayerInputSystem::move(const entt::entity& entity, glm::vec2 velocity, int spriteIndex)
    {
        auto* rigidBody{getRegistry().try_get<RigidBodyComponent>(entity)};
        if (rigidBody) {
            rigidBody->velocity = velocity;
        }
        auto* sprite{getRegistry().try_get<SpriteComponent>(entity)};
        if (sprite) {
            sprite->sourceRect.y = sprite->height * static_cast<float>(spriteIndex);
        }
        auto* transform{getRegistry().try_get<TransformComponent>(entity)};
        if (transform) {
            transform->rotation = static_cast<float>(spriteIndex) * 90.0f;
        }
    }

    void CameraMovementSystem::update(SceneData& sceneData)
    {
        auto view{getRegistry().view<const CameraFollowComponent, const TransformComponent>()};
        for (auto entity : view) {
            const auto& transform{view.get<TransformComponent>(entity)};
            Rect& cameraDisplay{sceneData.camera.display};
            cameraDisplay.x = static_cast<int>(transform.position.x - (cameraDisplay.w / 2.0));
            cameraDisplay.y = static_cast<int>(transform.position.y - (cameraDisplay.h / 2.0));
            cameraDisplay.x = std::clamp(
                cameraDisplay.x, 0, static_cast<int>(sceneData.levelData.mapData.width) - cameraDisplay.w);
            cameraDisplay.y = std::clamp(
                cameraDisplay.y, 0, static_cast<int>(sceneData.levelData.mapData.height) - cameraDisplay.h);
        }
    }

    void ProjectileEmitSystem::subscribeToEvents(EventBus& eventBus)
    {
        eventBus.addSubscriber<ProjectileEmitEvent, ProjectileEmitSystem>(
            this, &ProjectileEmitSystem::onEmitProjectile);
    }

    void ProjectileEmitSystem::update()
    {
        auto view{getRegistry().view<ProjectileEmitterComponent, const TransformComponent>()};
        for (auto entity : view) {
            auto& projectileEmitter{view.get<ProjectileEmitterComponent>(entity)};
            if (projectileEmitter.isAutoShoot && projectileEmitter.canShoot()) {
                emitProjectile(entity, projectileEmitter.projectileVelocity,
                               projectileEmitter.projectileDuration, projectileEmitter.hitPercentageDamage,
                               projectileEmitter.isProjectileFriendly);
            }
        }
    }

    void ProjectileEmitSystem::emitProjectile(const entt::entity& entity, glm::vec2 velocity, int duration,
                                              int damage, bool isFriendly)
    {
        const auto& transform{getRegistry().get<TransformComponent>(entity)};
        glm::vec2 projectilePosition{transform.position};
        const auto* sprite{getRegistry().try_get<SpriteComponent>(entity)};
        if (sprite) {
            projectilePosition.x += static_cast<float>(sprite->width) * transform.scale.x / 2.0f;
            projectilePosition.y += static_cast<float>(sprite->height) * transform.scale.y / 2.0f;
        }
        auto projectile{getRegistry().create()};
        std::string group{isFriendly ? "player-projectiles" : "enemy-projectiles"};
        if (isFriendly) {
            getRegistry().emplace<PlayerProjectile>(projectile);
        } else {
            getRegistry().emplace<EnemyProjectile>(projectile);
        }
        getRegistry().emplace<TransformComponent>(projectile, projectilePosition);
        getRegistry().emplace<RigidBodyComponent>(projectile, velocity);
        getRegistry().emplace<SpriteComponent>(projectile, "bullet-texture", 4.0f, 4.0f, 4);
        getRegistry().emplace<BoxColliderComponent>(projectile, 4, 4);
        getRegistry().emplace<LifecycleComponent>(projectile, duration);
        getRegistry().emplace<DamageComponent>(projectile, damage);
        auto& projectileEmitter{getRegistry().get<ProjectileEmitterComponent>(entity)};
        projectileEmitter.lastEmissionTime = Timer::getTicks();
    }

    void ProjectileEmitSystem::onEmitProjectile(ProjectileEmitEvent& event)
    {
        emitProjectile(event.entity, event.velocity, event.projectileDuration, event.projectileDamage,
                       event.isProjectileFriendly);
    }

    void LifecycleSystem::update()
    {
        auto view{getRegistry().view<const LifecycleComponent>()};
        for (auto entity : view) {
            const auto& lifecycle{view.get<LifecycleComponent>(entity)};
            if (static_cast<int>(Timer::getTicks() - lifecycle.startTime) > lifecycle.duration) {
                getRegistry().destroy(entity);
            }
        }
    }

    void TextRenderingSystem::update(Renderer& renderer, const AssetManager& assetManager,
                                     const Camera& camera)
    {
        auto view{getRegistry().view<const TransformComponent, const TextComponent>()};
        for (auto entity : view) {
            const auto& transform{view.get<TransformComponent>(entity)};
            const auto& text{view.get<TextComponent>(entity)};
            const Font& font{assetManager.getFont(text.fontId)};
            glm::vec2 renderPosition{transform.position};
            if (!text.hasFixedPosition) {
                renderPosition = getRenderPosition(renderPosition, camera);
            }
            renderer.drawText(font, text.text, text.color, renderPosition);
        }
    }

    void HealthBarRenderingSystem::update(Renderer& renderer, const Font& font, const Camera& camera)
    {
        auto view{
            getRegistry().view<const TransformComponent, const SpriteComponent, const HealthComponent>()};
        for (auto entity : view) {
            const auto& transform{view.get<TransformComponent>(entity)};
            const auto& sprite{view.get<SpriteComponent>(entity)};
            const auto& health{view.get<HealthComponent>(entity)};
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
