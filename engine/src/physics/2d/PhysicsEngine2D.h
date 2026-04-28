#ifndef PHYSICS_ENGINE_2D_H
#define PHYSICS_ENGINE_2D_H

#include "Body2D.h"

#include <box2d/box2d.h>
#include <entt/entity/registry.hpp>

namespace Engine
{
    class PhysicsEngine2D
    {
    public:
        PhysicsEngine2D();
        PhysicsEngine2D(const PhysicsEngine2D&) = delete;
        PhysicsEngine2D(PhysicsEngine2D&&) = delete;
        PhysicsEngine2D& operator=(const PhysicsEngine2D&) = delete;
        PhysicsEngine2D& operator=(PhysicsEngine2D&&) = delete;
        ~PhysicsEngine2D();
        void Update(float timeStep);
        b2BodyEvents GetBodyEvents();
        Body2D CreateBody(const BodyData2D& bodyData, entt::entity entity);
        void CreateDefaultShape(b2BodyId);
        void CreateBoxShape(b2BodyId bodyId, const ShapeData2D& shapeData, float width, float height);
        void CreateCircleShape(b2BodyId bodyId, const ShapeData2D& shapeData, float radius);

    private:
        constexpr static float s_gravityValue = 9.81f;
        constexpr static int s_subStepCount = 4;
        b2WorldId m_worldId{};
    };
} // namespace Engine

#endif // PHYSICSENGINE2D_H
