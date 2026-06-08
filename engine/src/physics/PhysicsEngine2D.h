#pragma once

#include "Physics2DDebugDraw.h"
#include "Physics2DTypes.h"

#include <box2d/box2d.h>
#include <entt/entity/entity.hpp>
#include <glm/glm.hpp>

namespace Engine
{
    class PhysicsEngine2D
    {
    public:
        void Init();
        void ShutDown();
        void Update(float timeStep);
        void DebugDraw();
        b2BodyEvents GetBodyEvents();
        b2BodyId CreateBody(const Body2DData& bodyData, glm::vec2 position, float rotation,
                            entt::entity entity);
        void CreateDefaultShape(b2BodyId);
        void CreateBoxShape(b2BodyId bodyId, const Shape2DData& shapeData, float width, float height,
                            float edgeRadius, glm::vec2 offset, float rotation);
        void CreateCircleShape(b2BodyId bodyId, const Shape2DData& shapeData, float radius, glm::vec2 offset);
        [[nodiscard]] glm::vec2 GetPosition(b2BodyId bodyId) const;
        [[nodiscard]] float GetRotationAngle(b2BodyId bodyId) const;
        [[nodiscard]] glm::vec2 GetLinearVelocity(b2BodyId bodyId) const;
        void SetTransform(b2BodyId bodyId, glm::vec2 position, float rotation);
        void SetLinearVelocity(b2BodyId bodyId, glm::vec2 velocity) const;
        void ApplyForceToCenter(b2BodyId bodyId, glm::vec2 force) const;
        void ApplyTorque(b2BodyId bodyId, float torque) const;

    private:
        constexpr static float s_gravityValue = 9.81f;
        constexpr static int s_subStepCount = 4;
        Physics2DDebugDraw m_physics2DDebugDraw{};
        b2WorldId m_worldId{};
    };
} // namespace Engine
