#include "PhysicsEngine2D.h"

#include "core/ConfigManager.h"
#include "core/Locator.h"

#include <glm/glm.hpp>

namespace Engine
{
    b2Vec2 To_b2Vec2(glm::vec2 v) { return b2Vec2{v.x, v.y}; }

    glm::vec2 From_b2Vec2(b2Vec2 v) { return glm::vec2{v.x, v.y}; }

    b2ShapeDef CreateShapeDef(const Shape2DData& shapeData, Entity& entity)
    {
        b2ShapeDef shapeDef{b2DefaultShapeDef()};
        shapeDef.density = shapeData.density;
        shapeDef.material.friction = shapeData.materialData.friction;
        shapeDef.material.restitution = shapeData.materialData.bounciness;
        shapeDef.material.rollingResistance = shapeData.materialData.rollingResistance;
        shapeDef.material.tangentSpeed = shapeData.materialData.tangentSpeed;
        shapeDef.isSensor = shapeData.isTrigger;
        shapeDef.updateBodyMass = !shapeData.isTrigger;
        shapeDef.enableContactEvents = shapeData.enableEvents && !shapeData.isTrigger;
        shapeDef.enableSensorEvents = shapeData.enableEvents && shapeData.isTrigger;
        shapeDef.userData = static_cast<void*>(&entity);
        return shapeDef;
    }

    void PhysicsEngine2D::Init()
    {
        b2WorldDef worldDef{b2DefaultWorldDef()};
        const Physics2DConfig& config{ConfigManager::GetPhysics2DConfig()};
        worldDef.gravity = To_b2Vec2(config.gravity);
        worldDef.maximumLinearSpeed = config.maxLinearSpeed;
        m_worldId = b2CreateWorld(&worldDef);
        Locator::GetLogger()->Info("Physics engine 2D initialized");
    }

    void PhysicsEngine2D::ShutDown()
    {
        b2DestroyWorld(m_worldId);
        Locator::GetLogger()->Info("Physics engine 2D shut down");
    }

    void PhysicsEngine2D::Update(float timeStep) { b2World_Step(m_worldId, timeStep, s_subStepCount); }

    void PhysicsEngine2D::DebugDraw()
    {
        m_physics2DDebugDraw.Draw(m_worldId);
        m_physics2DDebugDraw.RenderDevGui();
    }

    b2BodyEvents PhysicsEngine2D::GetBodyEvents() { return b2World_GetBodyEvents(m_worldId); }

    b2ContactEvents PhysicsEngine2D::GetCollisionEvents() { return b2World_GetContactEvents(m_worldId); }

    b2SensorEvents PhysicsEngine2D::GetTriggerEvents() { return b2World_GetSensorEvents(m_worldId); }

    Body2DId PhysicsEngine2D::CreateBody(const Body2DData& bodyData, glm::vec2 position, float rotation,
                                         Entity& entity, std::string_view name)
    {
        b2BodyDef bodyDef{b2DefaultBodyDef()};
        bodyDef.position = To_b2Vec2(position);
        bodyDef.rotation = b2MakeRot(glm::radians(rotation));
        bodyDef.type = bodyData.type;
        bodyDef.gravityScale = bodyData.gravityScale;
        bodyDef.angularDamping = bodyData.angularDamping;
        bodyDef.angularVelocity = bodyData.angularVelocity;
        bodyDef.linearDamping = bodyData.linearDamping;
        bodyDef.linearVelocity = To_b2Vec2(bodyData.linearVelocity);
        bodyDef.isBullet = bodyData.preciseCollisions;
        bodyDef.name = name.data();
        bodyDef.userData = static_cast<void*>(&entity);
        return b2CreateBody(m_worldId, &bodyDef);
    }

    void PhysicsEngine2D::DestroyBody(Body2DId bodyId)
    {
        if (b2Body_IsValid(bodyId)) {
            b2DestroyBody(bodyId);
        }
    }

    void PhysicsEngine2D::CreateDefaultShape(Body2DId bodyId, Entity& entity)
    {
        b2ShapeDef shapeDef{b2DefaultShapeDef()};
        shapeDef.filter.maskBits = 0; // no collision
        shapeDef.userData = static_cast<void*>(&entity);
        const b2Circle circle{b2Vec2_zero, 1.0f};
        b2CreateCircleShape(bodyId, &shapeDef, &circle);
    }

    Shape2DId PhysicsEngine2D::CreateBoxShape(Body2DId bodyId, const Shape2DData& shapeData, float width,
                                              float height, float edgeRadius, glm::vec2 offset,
                                              float rotation, Entity& entity)
    {
        const b2ShapeDef shapeDef{CreateShapeDef(shapeData, entity)};
        const b2Polygon box{b2MakeOffsetRoundedBox(width / 2.0f, height / 2.0f, To_b2Vec2(offset),
                                                   b2MakeRot(rotation), edgeRadius)};
        return b2CreatePolygonShape(bodyId, &shapeDef, &box);
    }

    Shape2DId PhysicsEngine2D::CreateCircleShape(Body2DId bodyId, const Shape2DData& shapeData, float radius,
                                                 glm::vec2 offset, Entity& entity)
    {
        const b2ShapeDef shapeDef{CreateShapeDef(shapeData, entity)};
        const b2Circle circle{To_b2Vec2(offset), radius};
        return b2CreateCircleShape(bodyId, &shapeDef, &circle);
    }

    void PhysicsEngine2D::DestroyShape(Shape2DId shapeId, bool updateBodyMass)
    {
        if (b2Shape_IsValid(shapeId)) {
            b2DestroyShape(shapeId, updateBodyMass);
        }
    }

    bool PhysicsEngine2D::IsValid(Shape2DId shapeId) { return b2Shape_IsValid(shapeId); }

    Entity* PhysicsEngine2D::GetEntity(Body2DId bodyId)
    {
        return static_cast<Entity*>(b2Body_GetUserData(bodyId));
    }

    Entity* PhysicsEngine2D::GetEntity(Shape2DId shapeId)
    {
        return static_cast<Entity*>(b2Shape_GetUserData(shapeId));
    }

    glm::vec2 PhysicsEngine2D::GetPosition(Body2DId bodyId) const
    {
        return From_b2Vec2(b2Body_GetPosition(bodyId));
    }

    float PhysicsEngine2D::GetRotationAngle(Body2DId bodyId) const
    {
        return b2Rot_GetAngle(b2Body_GetRotation(bodyId));
    }

    glm::vec2 PhysicsEngine2D::GetLinearVelocity(Body2DId bodyId) const
    {
        return From_b2Vec2(b2Body_GetLinearVelocity(bodyId));
    }

    float PhysicsEngine2D::GetAngularVelocity(Body2DId bodyId) const
    {
        return b2Body_GetAngularVelocity(bodyId);
    }

    void PhysicsEngine2D::SetTransform(Body2DId bodyId, glm::vec2 position, float rotation)
    {
        b2Body_SetTransform(bodyId, To_b2Vec2(position), b2MakeRot(rotation));
    }

    void PhysicsEngine2D::SetLinearVelocity(Body2DId bodyId, glm::vec2 velocity) const
    {
        b2Body_SetLinearVelocity(bodyId, To_b2Vec2(velocity));
    }

    void PhysicsEngine2D::SetAngularVelocity(Body2DId bodyId, float velocity) const
    {
        b2Body_SetAngularVelocity(bodyId, velocity);
    }

    void PhysicsEngine2D::ApplyForce(Body2DId bodyId, glm::vec2 force) const
    {
        b2Body_ApplyForceToCenter(bodyId, To_b2Vec2(force), true);
    }

    void PhysicsEngine2D::ApplyTorque(Body2DId bodyId, float torque) const
    {
        b2Body_ApplyTorque(bodyId, torque, true);
    }
} // namespace Engine
