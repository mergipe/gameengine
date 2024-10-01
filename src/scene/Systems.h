#ifndef SYSTEMS_H
#define SYSTEMS_H

#include <AssetStore.h>
#include <Components.h>
#include <ECS.h>
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
    std::unique_ptr<AssetStore> &assetStore;

public:
    RenderSystem(Registry *registry, SDL_Renderer *renderer, std::unique_ptr<AssetStore> &assetStore);
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
public:
    CollisionSystem(Registry *registry);
    void update();
};

#endif
