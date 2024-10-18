#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "ECS.h"
#include "resources/ResourceManager.h"
#include <SDL.h>

namespace Engine
{
    class MovementSystem : public System
    {
    public:
        MovementSystem();
        void update(float timeStep);
    };

    class SpriteRenderingSystem : public System
    {
    public:
        SpriteRenderingSystem();
        void update(SDL_Renderer* renderer, const ResourceManager& resourceManager,
                    float frameExtrapolationTimeStep);
    };

    class AnimationSystem : public System
    {
    public:
        AnimationSystem();
        void update();
    };

    class CollisionSystem : public System
    {
    public:
        CollisionSystem();
        void update();

    private:
        bool aabbHasCollided(double aX, double aY, double aW, double aH, double bX, double bY, double bW,
                             double bH);
    };

    class BoxColliderRenderingSystem : public System
    {
    public:
        BoxColliderRenderingSystem();
        void update(SDL_Renderer* renderer, float frameExtrapolationTimeStep);
    };
} // namespace Engine

#endif
