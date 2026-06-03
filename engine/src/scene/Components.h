#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "core/Math.h"
#include "core/StringId.h"
#include "input/InputCallback.h"
#include "input/InputDevice.h"
#include "physics/Physics2DTypes.h"
#include "renderer/Camera.h"
#include "scripting/ScriptInstance.h"

#include <vector>

namespace Engine
{
    struct IdComponent final {
        StringId value{};
    };

    struct TagComponent final {
        StringId value{};
    };

    struct TransformComponent final {
        glm::vec3 position{0.0f};
        glm::vec3 scale{1.0f};
        glm::vec3 rotation{0.0f};
    };

    struct SpriteComponent final {
        StringId textureId{};
        StringId spriteId{};
        glm::vec2 pivotPoint{0.5f, 0.5f};
        RGBA8 color{255, 255, 255, 255};
        int zIndex{0};
    };

    struct SpriteAnimationComponent final {
    };

    struct RigidBody2DComponent final {
        Body2DData bodyData{};
    };

    struct RigidBody2DRuntimeComponent final {
        b2BodyId bodyId{};
    };

    struct BoxCollider2DComponent final {
        Shape2DData shapeData{};
        float width{1.0f};
        float height{1.0f};
        float edgeRadius{0.0f};
    };

    struct CircleCollider2DComponent final {
        Shape2DData shapeData{};
        float radius{1.0f};
    };

    struct ScriptDef final {
        std::string filePath{};
        std::string className{};
    };

    struct ScriptComponent final {
        std::vector<ScriptDef> scriptDefs{};
    };

    struct ScriptRuntimeComponent final {
        std::vector<ScriptInstance> scriptInstances{};
    };

    struct InputCallbackDef final {
        StringId scriptId{};
        std::string callbackName{};
    };

    struct PlayerInputComponent final {
        std::unordered_map<StringId, InputCallbackDef> callbackDefs{};
        StringId defaultInputScope{};
    };

    struct PlayerInputRuntimeComponent final {
        InputCallbackMapping callbackMapping{};
        InputDevice::Id inputDeviceId{};
    };

    struct CameraComponent final {
        Camera camera{};
    };
} // namespace Engine

#endif
