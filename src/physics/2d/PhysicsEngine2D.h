#ifndef PHYSICSENGINE2D_H
#define PHYSICSENGINE2D_H

#include "Body2D.h"
#include <box2d/box2d.h>
#include <entt/entt.hpp>

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
        void update(float timeStep);
        b2BodyEvents getBodyEvents();
        Body2D createBody(const BodyData2D& bodyData, entt::entity entity);
        void createDefaultShape(b2BodyId);
        void createBoxShape(b2BodyId bodyId, const ShapeData2D& shapeData, float width, float height);
        void createCircleShape(b2BodyId bodyId, const ShapeData2D& shapeData, float radius);

    private:
        constexpr static float s_gravityValue = 9.81f;
        constexpr static int s_subStepCount = 4;
        b2WorldId m_worldId{};
    };
} // namespace Engine

#endif // PHYSICSENGINE2D_H
