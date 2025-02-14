#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "ECS.h"
#include "resources/ResourceManager.h"
#include <SDL.h>

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
        void update(SDL_Renderer* renderer, const ResourceManager& resourceManager,
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
        void update();

    private:
        constexpr bool aabbHasCollided(double aX, double aY, double aW, double aH, double bX, double bY,
                                       double bW, double bH);
    };

    class BoxColliderRenderingSystem final : public System
    {
    public:
        BoxColliderRenderingSystem();
        void update(SDL_Renderer* renderer, float frameExtrapolationTimeStep);
    };
} // namespace Engine

#endif
