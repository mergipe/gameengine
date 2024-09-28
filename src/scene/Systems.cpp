#include <Systems.h>

MovementSystem::MovementSystem(Registry *registry) : System{registry} {
    requireComponent<TransformComponent>();
    requireComponent<RigidBodyComponent>();
}

void MovementSystem::update(float timeStep) {
    for (auto entity : getEntities()) {
        auto &transform{registry->getComponent<TransformComponent>(entity)};
        const auto rigidBody{registry->getComponent<RigidBodyComponent>(entity)};
        transform.position += rigidBody.velocity * timeStep;
    }
};

RenderSystem::RenderSystem(Registry *registry, SDL_Renderer *renderer)
    : System{registry}, renderer{renderer} {
    requireComponent<TransformComponent>();
    requireComponent<SpriteComponent>();
}

void RenderSystem::update(float frameExtrapolationTimeStep) {
    for (auto entity : getEntities()) {
        const auto transform{registry->getComponent<TransformComponent>(entity)};
        const auto sprite{registry->getComponent<SpriteComponent>(entity)};
        const auto rigidBody{registry->getComponent<RigidBodyComponent>(entity)};
        glm::vec2 position{transform.position + rigidBody.velocity * frameExtrapolationTimeStep};
        SDL_Rect rect = {static_cast<int>(position.x), static_cast<int>(position.y), sprite.width,
                         sprite.height};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}
