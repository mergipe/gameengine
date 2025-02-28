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
        double rotation{0.0};
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

    struct KeyboardControlComponent final {
        glm::vec2 upVelocity{0};
        glm::vec2 rightVelocity{0};
        glm::vec2 downVelocity{0};
        glm::vec2 leftVelocity{0};
    };

    struct CameraFollowComponent final {
    };

    struct ProjectileEmitterComponent final {
        ProjectileEmitterComponent(glm::vec2 projectileVelocity = glm::vec2(0), int repeatFrequency = 0,
                                   int projectileDuration = 10000, int hitPercentDamage = 10,
                                   bool isProjectileFriendly = false)
            : projectileVelocity{projectileVelocity}
            , repeatFrequency{repeatFrequency}
            , projectileDuration{projectileDuration}
            , hitPercentDamage{hitPercentDamage}
            , isProjectileFriendly{isProjectileFriendly}
        {
        }
        glm::vec2 projectileVelocity{};
        std::uint64_t lastEmissionTime{Timer::getTicks()};
        int repeatFrequency{};
        int projectileDuration{};
        int hitPercentDamage{};
        bool isProjectileFriendly{};
    };

    struct HealthComponent final {
        int healthPercentage{100};
    };
} // namespace Engine

#endif
