#pragma once

#include "Components.h"

namespace Engine::ScriptingApi
{
    struct CollisionPoint2D {
        glm::vec2 anchorA{};
        glm::vec2 anchorB{};
        float normalImpulse{};
        float normalVelocity{};
        float separation{};
        float tangentImpulse{};
        float totalNormalImpulse{};
        U16 id{};
        bool persisted{};
    };

    struct Collision2DData {
        std::array<CollisionPoint2D, 2> points{};
        glm::vec2 normal{};
        int pointCount{};
    };
} // namespace Engine::ScriptingApi
