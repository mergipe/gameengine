#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "ECS.h"
#include "events/EventBus.h"
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
        void update(Renderer& renderer, const ResourceManager& resourceManager,
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
        void update(Renderer& renderer, float frameExtrapolationTimeStep);
    };

    class DamageSystem final : public System
    {
    public:
        DamageSystem();
        void subscribeToEvents(EventBus& eventBus);
        void onCollision(CollisionEvent& event);
        void update();
    };
} // namespace Engine

#endif
