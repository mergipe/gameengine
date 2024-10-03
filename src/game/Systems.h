#ifndef SYSTEMS_H
#define SYSTEMS_H

#include <Components.h>
#include <ECS.h>
#include <ResourceManager.h>
#include <SDL.h>
#include <memory>

class MovementSystem : public System {
public:
    MovementSystem(Registry *registry);
    void update(float timeStep);
};

class RenderSystem : public System {
private:
    SDL_Renderer *renderer{};
    std::unique_ptr<ResourceManager> &resourceManager;

public:
    RenderSystem(Registry *registry, SDL_Renderer *renderer,
                 std::unique_ptr<ResourceManager> &resourceManager);
    RenderSystem(const RenderSystem &);
    void operator=(const RenderSystem &);
    void update(float frameExtrapolationTimeStep);
};

class AnimationSystem : public System {
public:
    AnimationSystem(Registry *registry);
    void update();
};

class CollisionSystem : public System {
private:
    bool aabbHasCollided(double aX, double aY, double aW, double aH, double bX, double bY, double bW,
                         double bH);

public:
    CollisionSystem(Registry *registry);
    void update();
};

class DebugRenderSystem : public System {
private:
    SDL_Renderer *renderer{};

public:
    DebugRenderSystem(Registry *registry, SDL_Renderer *renderer);
    DebugRenderSystem(const DebugRenderSystem &);
    void operator=(const DebugRenderSystem &);
    void update(float frameExtrapolationTimeStep);
};

#endif
