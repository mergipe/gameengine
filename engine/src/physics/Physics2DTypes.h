#pragma once

#include "core/StringId.h"

#include <box2d/box2d.h>
#include <optional>

namespace Engine
{
    constexpr std::optional<b2BodyType> ParseBodyType2D(const StringId& bodyTypeName)
    {
        if (bodyTypeName == SID("static")) {
            return b2_staticBody;
        }
        if (bodyTypeName == SID("dynamic")) {
            return b2_dynamicBody;
        }
        if (bodyTypeName == SID("kinematic")) {
            return b2_kinematicBody;
        }
        return {};
    }

    struct Material2DData {
        float friction{0.6f};
        float restitution{0.0f};
        float rollingResistance{0.0f};
        float tangentSpeed{0.0f};
    };

    struct Shape2DData {
        Material2DData materialData{};
        float density{1.0f};
        bool isTrigger{false};
    };

    struct Body2DData {
        b2BodyType type{b2_staticBody};
        float gravityScale{0.0f};
        float angularDamping{0.0f};
        float linearDamping{0.0f};
    };
} // namespace Engine
