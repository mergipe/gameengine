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

    class RenderSystem : public System
    {
    public:
        RenderSystem(SDL_Renderer& renderer, const ResourceManager& resourceManager);
        void update(float frameExtrapolationTimeStep);

    private:
        SDL_Renderer& m_renderer;
        const ResourceManager& m_resourceManager;
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

    class DebugRenderSystem : public System
    {
    public:
        DebugRenderSystem(SDL_Renderer& renderer);
        void update(float frameExtrapolationTimeStep);

    private:
        SDL_Renderer& m_renderer;
    };
} // namespace Engine

#endif
