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
        MovementSystem(const Registry& registry);
        void update(float timeStep);
    };

    class RenderSystem : public System
    {
    private:
        SDL_Renderer& m_renderer;
        const ResourceManager& m_resourceManager;

    public:
        RenderSystem(const Registry& registry, SDL_Renderer& renderer,
                     const ResourceManager& resourceManager);
        void update(float frameExtrapolationTimeStep);
    };

    class AnimationSystem : public System
    {
    public:
        AnimationSystem(const Registry& registry);
        void update();
    };

    class CollisionSystem : public System
    {
    private:
        bool aabbHasCollided(double aX, double aY, double aW, double aH, double bX, double bY, double bW,
                             double bH);

    public:
        CollisionSystem(const Registry& registry);
        void update();
    };

    class DebugRenderSystem : public System
    {
    private:
        SDL_Renderer& m_renderer;

    public:
        DebugRenderSystem(const Registry& registry, SDL_Renderer& renderer);
        void update(float frameExtrapolationTimeStep);
    };
} // namespace Engine

#endif
