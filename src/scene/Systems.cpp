#include <Systems.h>
#include <algorithm>
#include <memory>

glm::vec2 getExtrapolatedPosition(glm::vec2 position, glm::vec2 velocity, float extrapolationTimeStep) {
    return glm::vec2{position + velocity * extrapolationTimeStep};
}

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
        glm::vec2 extrapolatedPosition{
            getExtrapolatedPosition(transform.position, rigidBody.velocity, frameExtrapolationTimeStep)};
        SDL_FRect spriteRect = {extrapolatedPosition.x, extrapolatedPosition.y,
                                static_cast<float>(sprite.width) * transform.scale.x,
                                static_cast<float>(sprite.height) * transform.scale.y};
        SDL_RenderCopyExF(renderer, assetStore->getTexture(sprite.assetId), &sprite.sourceRect, &spriteRect,
                          transform.rotation, NULL, SDL_FLIP_NONE);
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
    for (auto entity : entities) {
        registry->getComponent<BoxColliderComponent>(entity).isColliding = false;
    }
    for (auto i{entities.begin()}; i != entities.end(); ++i) {
        const Entity entity{*i};
        const auto transform{registry->getComponent<TransformComponent>(entity)};
        auto &boxCollider{registry->getComponent<BoxColliderComponent>(entity)};
        for (auto j{i + 1}; j != entities.end(); ++j) {
            const Entity otherEntity{*j};
            const auto otherTransform{registry->getComponent<TransformComponent>(otherEntity)};
            auto &otherBoxCollider{registry->getComponent<BoxColliderComponent>(otherEntity)};
            const bool collisionHappened{aabbHasCollided(
                transform.position.x + boxCollider.offset.x, transform.position.y + boxCollider.offset.y,
                static_cast<float>(boxCollider.width) * transform.scale.x,
                static_cast<float>(boxCollider.height) * transform.scale.y,
                otherTransform.position.x + otherBoxCollider.offset.x,
                otherTransform.position.y + otherBoxCollider.offset.y,
                static_cast<float>(otherBoxCollider.width) * otherTransform.scale.x,
                static_cast<float>(otherBoxCollider.height) * otherTransform.scale.y)};
            if (collisionHappened) {
                boxCollider.isColliding = otherBoxCollider.isColliding = true;
                Logger::trace("Entity {} is colliding with entity {}", entity.getId(), otherEntity.getId());
            }
        }
    }
}

bool CollisionSystem::aabbHasCollided(double aX, double aY, double aW, double aH, double bX, double bY,
                                      double bW, double bH) {
    return (aX < bX + bW && aX + aW > bX && aY < bY + bH && aY + aH > bY);
}

DebugRenderSystem::DebugRenderSystem(Registry *registry, SDL_Renderer *renderer)
    : System{registry}, renderer{renderer} {
    requireComponent<TransformComponent>();
    requireComponent<BoxColliderComponent>();
}

void DebugRenderSystem::update(float frameExtrapolationTimeStep) {
    for (auto entity : getEntities()) {
        const auto transform{registry->getComponent<TransformComponent>(entity)};
        const auto boxCollider{registry->getComponent<BoxColliderComponent>(entity)};
        const auto rigidBody{registry->getComponent<RigidBodyComponent>(entity)};
        glm::vec2 extrapolatedPosition{
            getExtrapolatedPosition(transform.position, rigidBody.velocity, frameExtrapolationTimeStep)};
        SDL_FRect colliderRect = {extrapolatedPosition.x + boxCollider.offset.x,
                                  extrapolatedPosition.y + boxCollider.offset.y,
                                  static_cast<float>(boxCollider.width) * transform.scale.x,
                                  static_cast<float>(boxCollider.height) * transform.scale.y};
        if (boxCollider.isColliding) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        }
        SDL_RenderDrawRectF(renderer, &colliderRect);
    }
}