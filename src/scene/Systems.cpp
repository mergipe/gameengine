#include "Components.h"
#include <Systems.h>
#include <algorithm>
#include <memory>

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

RenderSystem::RenderSystem(Registry *registry, SDL_Renderer *renderer,
                           std::unique_ptr<AssetStore> &assetStore)
    : System{registry}, renderer{renderer}, assetStore{assetStore} {
    requireComponent<TransformComponent>();
    requireComponent<SpriteComponent>();
}

void RenderSystem::update(float frameExtrapolationTimeStep) {
    std::vector<Entity> entities{getEntities()};
    std::sort(entities.begin(), entities.end(), [this](Entity a, Entity b) {
        return registry->getComponent<SpriteComponent>(a).zIndex <
               registry->getComponent<SpriteComponent>(b).zIndex;
    });
    for (auto entity : entities) {
        const auto transform{registry->getComponent<TransformComponent>(entity)};
        const auto sprite{registry->getComponent<SpriteComponent>(entity)};
        const auto rigidBody{registry->getComponent<RigidBodyComponent>(entity)};
        glm::vec2 position{transform.position + rigidBody.velocity * frameExtrapolationTimeStep};
        SDL_FRect destinationRect = {position.x, position.y,
                                     static_cast<float>(sprite.width) * transform.scale.x,
                                     static_cast<float>(sprite.height) * transform.scale.y};
        SDL_RenderCopyExF(renderer, assetStore->getTexture(sprite.assetId), &sprite.sourceRect,
                          &destinationRect, transform.rotation, NULL, SDL_FLIP_NONE);
    }
}

AnimationSystem::AnimationSystem(Registry *registry) : System{registry} {
    requireComponent<SpriteComponent>();
    requireComponent<AnimationComponent>();
}

void AnimationSystem::update() {
    for (auto entity : getEntities()) {
        auto &sprite{registry->getComponent<SpriteComponent>(entity)};
        auto &animation{registry->getComponent<AnimationComponent>(entity)};
        animation.currentFrame =
            (static_cast<int>(SDL_GetTicks64() - animation.startTime) * animation.framesPerSecond / 1000) %
            animation.framesCount;
        sprite.sourceRect.x = animation.currentFrame * sprite.width;
    }
}

CollisionSystem::CollisionSystem(Registry *registry) : System{registry} {
    requireComponent<TransformComponent>();
    requireComponent<BoxColliderComponent>();
}

void CollisionSystem::update() {
    const auto entities{getEntities()};
    for (auto i{entities.begin()}; i != entities.end(); ++i) {
        const Entity entity{*i};
        const auto transform{registry->getComponent<TransformComponent>(entity)};
        const auto boxCollider{registry->getComponent<BoxColliderComponent>(entity)};
        for (auto j{i + 1}; j != entities.end(); ++j) {
            const Entity otherEntity{*j};
            const auto otherTransform{registry->getComponent<TransformComponent>(otherEntity)};
            const auto otherBoxCollider{registry->getComponent<BoxColliderComponent>(otherEntity)};
        }
    }
}
