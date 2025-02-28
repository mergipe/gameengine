#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "ECS.h"
#include "events/EventBus.h"
#include "game/Scene.h"
#include "renderer/Camera.h"
#include "renderer/Renderer.h"
#include "resources/ResourceManager.h"

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
        void update(Renderer& renderer, const ResourceManager& resourceManager, const Camera& camera,
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
    };

    class KeyboardControlSystem final : public System
    {
    public:
        KeyboardControlSystem();
        void subscribeToEvents(EventBus& eventBus);
        void onKeyPressed(KeyPressedEvent& event);
    };

    class CameraMovementSystem final : public System
    {
    public:
        CameraMovementSystem();
        void update(SceneData& sceneData);
    };

    class ProjectileEmitSystem final : public System
    {
    public:
        ProjectileEmitSystem();
        void update(Registry& registry);
    };
} // namespace Engine

#endif
