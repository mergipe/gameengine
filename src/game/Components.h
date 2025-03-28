#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "core/Timer.h"
#include "renderer/Shapes.h"
#include <glm/glm.hpp>
#include <sol/sol.hpp>
#include <string>

namespace Engine
{
    struct TagComponent final {
        std::string name{};
    };

    struct TransformComponent final {
        glm::vec3 position{0.0f};
        glm::vec3 scale{1.0f};
        glm::vec3 rotation{0.0f};
    };

    struct RigidBody2DComponent final {
        glm::vec3 velocity{0.0f};
    };

    struct SpriteComponent final {
        std::string textureId{};
        Rect textureArea{};
        int zIndex{0};
        bool hasFixedPosition{false};
    };

    struct SpriteAnimationComponent final {
        SpriteAnimationComponent(int framesCount = 1, int framesPerSecond = 1, bool shouldLoop = true)
            : framesCount{framesCount}, framesPerSecond{framesPerSecond}, shouldLoop{shouldLoop}
        {
        }
        Timer::Ticks startTime{Timer::getTicks()};
        int currentFrame{0};
        int framesCount{};
        int framesPerSecond{};
        bool shouldLoop{};
    };

    struct BoxCollider2DComponent final {
        BoxCollider2DComponent(float width = 0, float height = 0, const glm::vec2& offset = glm::vec2{0.0f})
            : offset{offset}, width{width}, height{height}
        {
        }
        glm::vec2 offset{};
        float width{};
        float height{};
        bool isColliding{};
    };

    struct PlayerInputComponent final {
    };

    struct ScriptComponent {
        sol::function func{sol::lua_nil};
    };
} // namespace Engine

#endif
