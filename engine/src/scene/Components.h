#pragma once

#include "core/StringId.h"
#include "input/InputCallback.h"
#include "input/InputDevice.h"
#include "physics/Physics2DTypes.h"
#include "renderer/Camera.h"
#include "scripting/ScriptInstance.h"
#include "scripting/ScriptSystem.h"

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
        std::optional<Body2DId> bodyId{};
    };

    struct BoxCollider2DComponent final {
        Shape2DData shapeData{};
        std::optional<Shape2DId> shapeId{};
        glm::vec2 offset{0.0f};
        float width{1.0f};
        float height{1.0f};
        float edgeRadius{0.0f};
        float rotation{0.0f};
    };

    struct CircleCollider2DComponent final {
        Shape2DData shapeData{};
        std::optional<Shape2DId> shapeId{};
        glm::vec2 offset{0.0f};
        float radius{1.0f};
    };

    struct ScriptComponent final {
        std::vector<ScriptData> scriptDatas{};
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

    template <typename... T>
    struct ComponentTypes {
    };

    using AllComponentTypes =
        ComponentTypes<IdComponent, TagComponent, TransformComponent, SpriteComponent,
                       SpriteAnimationComponent, RigidBody2DComponent, BoxCollider2DComponent,
                       CircleCollider2DComponent, ScriptComponent, PlayerInputComponent, CameraComponent>;
} // namespace Engine
