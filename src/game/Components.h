#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "Script.h"
#include "core/Math.h"
#include "core/StringId.h"
#include "core/Timer.h"
#include "renderer/Camera.h"
#include "renderer/Shapes.h"
#include <glm/glm.hpp>
#include <sol/sol.hpp>

namespace Engine
{
    struct IdComponent final {
        StringId sid{};
    };

    struct TagComponent final {
        StringId name{};
    };

    struct TransformComponent final {
        glm::mat4 getTransformation() const
        {
            return Math::getTransformationMatrix(position, rotation, scale);
        }

        glm::vec3 position{0.0f};
        glm::vec3 scale{1.0f};
        glm::vec3 rotation{0.0f};
    };

    struct RigidBody2DComponent final {
        glm::vec3 velocity{0.0f};
    };

    struct SpriteComponent final {
        StringId textureId{};
        Rect textureArea{};
        glm::vec3 color{1.0f};
        int zIndex{0};
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

    struct ScriptComponent final {
        std::vector<Script> scripts{};
    };

    struct CameraComponent final {
        std::unique_ptr<Camera> camera{};
    };
} // namespace Engine

#endif
