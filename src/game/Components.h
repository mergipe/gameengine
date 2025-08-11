#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "ScriptInstance.h"
#include "core/Math.h"
#include "core/StringId.h"
#include "core/Timer.h"
#include "input/InputCallback.h"
#include "input/InputDevice.h"
#include "physics/2d/Body2D.h"
#include "renderer/Camera.h"
#include "renderer/Shapes.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace Engine
{
    struct IdComponent final {
        StringId sid{};
    };

    struct TagComponent final {
        StringId name{};
    };

    struct TransformComponent final {
        glm::mat4 getTransformationMatrix() const
        {
            return Math::getTransformationMatrix(position, rotation, scale);
        }
        glm::vec3 getRight() const
        {
            return Math::getRotationMatrix(rotation) * glm::vec4{1.0f, 0.0f, 0.0f, 1.0f};
        }
        glm::vec3 getUp() const
        {
            return Math::getRotationMatrix(rotation) * glm::vec4{0.0f, 1.0f, 0.0f, 1.0f};
        }
        glm::vec3 getForward() const
        {
            return Math::getRotationMatrix(rotation) * glm::vec4{0.0f, 0.0f, 1.0f, 1.0f};
        }

        glm::vec3 position{0.0f};
        glm::vec3 scale{1.0f};
        glm::vec3 rotation{0.0f};
    };

    struct RigidBody2DComponent final {
        BodyData2D bodyData{};
        Body2D body{};
    };

    struct SpriteComponent final {
        StringId textureId{};
        Rect textureArea{};
        glm::vec3 color{1.0f};
        int zIndex{0};
    };

    struct SpriteAnimationComponent final {
        explicit SpriteAnimationComponent(int framesCount = 1, int framesPerSecond = 1,
                                          bool shouldLoop = true)
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
        ShapeData2D shapeData{};
        float width{};
        float height{};
    };

    struct CircleCollider2DComponent final {
        ShapeData2D shapeData{};
        float radius{};
    };

    struct PlayerInputComponent final {
        InputCallbackMapping callbackMapping{};
        StringId defaultInputScope{};
        InputDevice::Id inputDeviceId{};
    };

    struct ScriptComponent final {
        std::vector<ScriptInstance> scriptInstances{};
    };

    struct CameraComponent final {
        std::unique_ptr<Camera> camera{};
    };
} // namespace Engine

#endif
