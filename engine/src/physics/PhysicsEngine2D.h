#pragma once

#include "Physics2DDebugDraw.h"
#include "Physics2DTypes.h"
#include "scene/Entity.h"

#include <box2d/box2d.h>
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

        BodyEvents2D GetBodyEvents();
        CollisionEvents2D GetCollisionEvents();
        TriggerEvents2D GetTriggerEvents();

        Body2DId CreateBody(const Body2DData& bodyData, glm::vec2 position, float rotation, Entity& entity,
                            std::string_view name);
        void DestroyBody(Body2DId bodyId);
        void CreateDefaultShape(Body2DId, Entity& entity);
        Shape2DId CreateBoxShape(Body2DId bodyId, const Shape2DData& shapeData, float width, float height,
                                 float edgeRadius, glm::vec2 offset, float rotation, Entity& entity);
        Shape2DId CreateCircleShape(Body2DId bodyId, const Shape2DData& shapeData, float radius,
                                    glm::vec2 offset, Entity& entity);
        void DestroyShape(Shape2DId shapeId, bool updateBodyMass);

        bool IsValid(Shape2DId shapeId);

        Entity* GetEntity(Body2DId bodyId);
        Entity* GetEntity(Shape2DId shapeId);
        [[nodiscard]] glm::vec2 GetPosition(Body2DId bodyId) const;
        [[nodiscard]] float GetRotationAngle(Body2DId bodyId) const;
        [[nodiscard]] glm::vec2 GetLinearVelocity(Body2DId bodyId) const;
        [[nodiscard]] float GetAngularVelocity(Body2DId bodyId) const;

        void SetTransform(Body2DId bodyId, glm::vec2 position, float rotation);
        void SetLinearVelocity(Body2DId bodyId, glm::vec2 velocity) const;
        void SetAngularVelocity(Body2DId bodyId, float velocity) const;
        void ApplyForce(Body2DId bodyId, glm::vec2 force) const;
        void ApplyTorque(Body2DId bodyId, float torque) const;

    private:
        constexpr static float s_gravityValue = 9.81f;
        constexpr static int s_subStepCount = 4;

        Physics2DDebugDraw m_physics2DDebugDraw{};
        b2WorldId m_worldId{};
    };
} // namespace Engine
