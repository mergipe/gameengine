#ifndef SYSTEMS_H
#define SYSTEMS_H

#include <Components.h>
#include <ECS.h>
#include <SDL.h>

class MovementSystem : public System {
public:
    MovementSystem(Registry *registry);
    void update(float timeStep);
};

class RenderSystem : public System {
private:
    SDL_Renderer *renderer{};

public:
    RenderSystem(Registry *registry, SDL_Renderer *renderer);
    RenderSystem(const RenderSystem &);
    void operator=(const RenderSystem &);
    void update(float frameExtrapolationTimeStep);
};

#endif
