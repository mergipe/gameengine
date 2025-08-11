#include "PhysicsEngine2D.h"
#include "core/Config.h"
#include <entt/entt.hpp>
#include <glm/glm.hpp>

namespace Engine
{
    PhysicsEngine2D::PhysicsEngine2D()
    {
        b2WorldDef worldDef{b2DefaultWorldDef()};
        Config::Physics2DConfig config{Config::loadPhysics2DConfig()};
        worldDef.gravity = b2Vec2{config.gravity.x, config.gravity.y};
        worldDef.maximumLinearSpeed = config.maxLinearSpeed;
        m_worldId = b2CreateWorld(&worldDef);
    }

    PhysicsEngine2D::~PhysicsEngine2D() { b2DestroyWorld(m_worldId); }

    void PhysicsEngine2D::update(float timeStep) { b2World_Step(m_worldId, timeStep, s_subStepCount); }

    b2BodyEvents PhysicsEngine2D::getBodyEvents() { return b2World_GetBodyEvents(m_worldId); }

    Body2D PhysicsEngine2D::createBody(const BodyData2D& bodyData, entt::entity entity)
    {
        b2BodyDef bodyDef{b2DefaultBodyDef()};
        bodyDef.position = b2Vec2{bodyData.position.x, bodyData.position.y};
        bodyDef.rotation = b2MakeRot(glm::radians(bodyData.rotation));
        bodyDef.type = bodyData.type;
        bodyDef.gravityScale = bodyData.gravityScale;
        bodyDef.angularDamping = bodyData.angularDamping;
        bodyDef.linearDamping = bodyData.linearDamping;
        bodyDef.userData = reinterpret_cast<void*>(entity);
        b2BodyId bodyId{b2CreateBody(m_worldId, &bodyDef)};
        return Body2D{bodyId};
    }

    void PhysicsEngine2D::createDefaultShape(b2BodyId bodyId)
    {
        b2ShapeDef shapeDef{b2DefaultShapeDef()};
        shapeDef.filter.maskBits = 0; // no collision
        b2Circle circle{b2Vec2_zero, 1.0f};
        b2CreateCircleShape(bodyId, &shapeDef, &circle);
    }

    b2ShapeDef createShapeDef(const ShapeData2D& shapeData)
    {
        b2ShapeDef shapeDef{b2DefaultShapeDef()};
        shapeDef.density = shapeData.density;
        shapeDef.material.friction = shapeData.materialData.friction;
        shapeDef.material.restitution = shapeData.materialData.restitution;
        shapeDef.material.rollingResistance = shapeData.materialData.rollingResistance;
        shapeDef.material.tangentSpeed = shapeData.materialData.tangentSpeed;
        shapeDef.isSensor = shapeData.isTrigger;
        return shapeDef;
    }

    void PhysicsEngine2D::createBoxShape(b2BodyId bodyId, const ShapeData2D& shapeData, float width,
                                         float height)
    {
        b2ShapeDef shapeDef{createShapeDef(shapeData)};
        b2Polygon box{b2MakeBox(width / 2.0f, height / 2.0f)};
        b2CreatePolygonShape(bodyId, &shapeDef, &box);
    }

    void PhysicsEngine2D::createCircleShape(b2BodyId bodyId, const ShapeData2D& shapeData, float radius)
    {
        b2ShapeDef shapeDef{createShapeDef(shapeData)};
        b2Circle circle{b2Vec2_zero, radius};
        b2CreateCircleShape(bodyId, &shapeDef, &circle);
    }
} // namespace Engine
