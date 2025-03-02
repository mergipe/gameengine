#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "core/Timer.h"
#include "renderer/Rect.h"
#include <glm/glm.hpp>
#include <string>

namespace Engine
{
    struct TransformComponent final {
        glm::vec2 position{0};
        glm::vec2 scale{1};
        float rotation{0.0};
    };

    struct RigidBodyComponent final {
        glm::vec2 velocity{0};
    };

    struct SpriteComponent final {
        SpriteComponent(std::string_view resourceId = "", int width = 0, int height = 0, int zIndex = 0,
                        bool isPositionFixed = false, int sourceRectX = 0, int sourceRectY = 0)
            : resourceId{resourceId}
            , sourceRect{Rect{sourceRectX, sourceRectY, width, height}}
            , width{width}
            , height{height}
            , zIndex{zIndex}
            , hasFixedPosition{isPositionFixed}
        {
        }
        std::string resourceId{};
        Rect sourceRect{};
        int width{};
        int height{};
        int zIndex{};
        bool hasFixedPosition{};
    };

    struct AnimationComponent final {
        AnimationComponent(int framesCount = 1, int framesPerSecond = 1, bool shouldLoop = true)
            : framesCount{framesCount}, framesPerSecond{framesPerSecond}, shouldLoop{shouldLoop}
        {
        }
        std::uint64_t startTime{Timer::getTicks()};
        int currentFrame{0};
        int framesCount{};
        int framesPerSecond{};
        bool shouldLoop{};
    };

    struct BoxColliderComponent final {
        BoxColliderComponent(int width = 0, int height = 0, const glm::vec2& offset = glm::vec2(0))
            : offset{offset}, width{width}, height{height}
        {
        }
        glm::vec2 offset{};
        int width{};
        int height{};
        bool isColliding{};
    };

    struct PlayerInputComponent final {
    };

    struct CameraFollowComponent final {
    };

    // this and ProjectileEmitSystem should become a script in the future
    struct ProjectileEmitterComponent final {
        ProjectileEmitterComponent(glm::vec2 projectileVelocity = glm::vec2(0), int repeatFrequency = 0,
                                   int projectileDuration = 10000, int hitPercentDamage = 10,
                                   bool isProjectileFriendly = false, bool isAutoShoot = true)
            : projectileVelocity{projectileVelocity}
            , cooldown{repeatFrequency}
            , projectileDuration{projectileDuration}
            , hitPercentDamage{hitPercentDamage}
            , isProjectileFriendly{isProjectileFriendly}
            , isAutoShoot{isAutoShoot}
        {
        }
        bool canShoot() { return static_cast<int>(Timer::getTicks() - lastEmissionTime) > cooldown; }
        glm::vec2 projectileVelocity{};
        std::uint64_t lastEmissionTime{Timer::getTicks()};
        int cooldown{};
        int projectileDuration{};
        int hitPercentDamage{};
        bool isProjectileFriendly{};
        bool isAutoShoot{};
    };

    struct HealthComponent final {
        int healthPercentage{100};
    };

    struct DamageComponent final {
        int value{0};
    };

    struct LifecycleComponent final {
        LifecycleComponent(int duration = 0)
            : duration{duration}
        {
        }
        int duration{};
        std::uint64_t startTime{Timer::getTicks()};
    };
} // namespace Engine

#endif
