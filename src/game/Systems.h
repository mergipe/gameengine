#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "ECS.h"
#include "assets/AssetManager.h"
#include "events/EventBus.h"
#include "game/Scene.h"
#include "renderer/Camera.h"
#include "renderer/Renderer.h"
#include <glm/glm.hpp>

namespace Engine
{
    class MovementSystem final : public System
    {
    public:
        MovementSystem();
        void update(float timeStep);
    };

    class SpriteRenderingSystem final : public System
    {
    public:
        SpriteRenderingSystem();
        void update(Renderer& renderer, const AssetManager& assetManager, const Camera& camera,
                    float frameExtrapolationTimeStep);
    };

    class AnimationSystem final : public System
    {
    public:
        AnimationSystem();
        void update();
    };

    class CollisionSystem final : public System
    {
    public:
        CollisionSystem();
        void update(EventBus& eventBus);
    };

    class BoxColliderRenderingSystem final : public System
    {
    public:
        BoxColliderRenderingSystem();
        void update(Renderer& renderer, const Camera& camera, float frameExtrapolationTimeStep);
    };

    class DamageSystem final : public System
    {
    public:
        DamageSystem();
        void subscribeToEvents(EventBus& eventBus);
        void onCollision(CollisionEvent& event);

    private:
        void onProjectileHitsEntity(Entity& projectile, Entity& entity);
    };

    class PlayerInputSystem final : public System
    {
    public:
        PlayerInputSystem();
        void subscribeToEvents(EventBus& eventBus);
        void onKeyPressed(KeyPressedEvent& event);

    private:
        void move(const Entity& entity, glm::vec2 velocity, int spriteIndex);
    };

    class CameraMovementSystem final : public System
    {
    public:
        CameraMovementSystem();
        void update(SceneData& sceneData);
    };

    // this and ProjectileEmmiterComponent should become a script in the future
    class ProjectileEmitSystem final : public System
    {
    public:
        ProjectileEmitSystem();
        void subscribeToEvents(EventBus& eventBus);
        void update();
        void onEmitProjectile(ProjectileEmitEvent& event);

    private:
        void emitProjectile(const Entity& entity, glm::vec2 velocity, int duration, int damage,
                            bool isFriendly);
    };

    class LifecycleSystem final : public System
    {
    public:
        LifecycleSystem();
        void update();
    };

    class TextRenderingSystem final : public System
    {
    public:
        TextRenderingSystem();
        void update(Renderer& renderer, const AssetManager& assetManager, const Camera& camera);
    };

    class HealthBarRenderingSystem final : public System
    {
    public:
        HealthBarRenderingSystem();
        void update(Renderer& renderer, const Font& font, const Camera& camera);
    };
} // namespace Engine

#endif
