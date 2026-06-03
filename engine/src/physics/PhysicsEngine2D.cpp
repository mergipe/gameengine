#include "PhysicsEngine2D.h"

#include "core/ConfigManager.h"
#include "core/Locator.h"

#include <glm/glm.hpp>

namespace Engine
{
    void PhysicsEngine2D::Init()
    {
        b2WorldDef worldDef{b2DefaultWorldDef()};
        const Physics2DConfig& config{ConfigManager::GetPhysics2DConfig()};
        worldDef.gravity = b2Vec2{config.gravity.x, config.gravity.y};
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

    b2BodyId PhysicsEngine2D::CreateBody(const Body2DData& bodyData, glm::vec2 position, float rotation,
                                         entt::entity entity)
    {
        b2BodyDef bodyDef{b2DefaultBodyDef()};
        bodyDef.position = b2Vec2{position.x, position.y};
        bodyDef.rotation = b2MakeRot(glm::radians(rotation));
        bodyDef.type = bodyData.type;
        bodyDef.gravityScale = bodyData.gravityScale;
        bodyDef.angularDamping = bodyData.angularDamping;
        bodyDef.linearDamping = bodyData.linearDamping;
        bodyDef.userData = reinterpret_cast<void*>(entity);
        return b2CreateBody(m_worldId, &bodyDef);
    }

    void PhysicsEngine2D::CreateDefaultShape(b2BodyId bodyId)
    {
        b2ShapeDef shapeDef{b2DefaultShapeDef()};
        shapeDef.filter.maskBits = 0; // no collision
        const b2Circle circle{b2Vec2_zero, 1.0f};
        b2CreateCircleShape(bodyId, &shapeDef, &circle);
    }

    b2ShapeDef CreateShapeDef(const Shape2DData& shapeData)
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

    void PhysicsEngine2D::CreateBoxShape(b2BodyId bodyId, const Shape2DData& shapeData, float width,
                                         float height, float edgeRadius)
    {
        const b2ShapeDef shapeDef{CreateShapeDef(shapeData)};
        const b2Polygon box{b2MakeRoundedBox(width / 2.0f, height / 2.0f, edgeRadius)};
        b2CreatePolygonShape(bodyId, &shapeDef, &box);
    }

    void PhysicsEngine2D::CreateCircleShape(b2BodyId bodyId, const Shape2DData& shapeData, float radius)
    {
        const b2ShapeDef shapeDef{CreateShapeDef(shapeData)};
        const b2Circle circle{b2Vec2_zero, radius};
        b2CreateCircleShape(bodyId, &shapeDef, &circle);
    }

    glm::vec2 PhysicsEngine2D::GetPosition(b2BodyId bodyId) const
    {
        const auto position{b2Body_GetPosition(bodyId)};
        return glm::vec2{position.x, position.y};
    }

    float PhysicsEngine2D::GetRotationAngle(b2BodyId bodyId) const
    {
        return b2Rot_GetAngle(b2Body_GetRotation(bodyId));
    }

    glm::vec2 PhysicsEngine2D::GetLinearVelocity(b2BodyId bodyId) const
    {
        const auto linearVelocity{b2Body_GetLinearVelocity(bodyId)};
        return glm::vec2{linearVelocity.x, linearVelocity.y};
    }

    void PhysicsEngine2D::SetTransform(b2BodyId bodyId, glm::vec2 position, float rotation)
    {
        b2Body_SetTransform(bodyId, b2Vec2{position.x, position.y}, b2MakeRot(rotation));
    }

    void PhysicsEngine2D::SetLinearVelocity(b2BodyId bodyId, glm::vec2 velocity) const
    {
        b2Body_SetLinearVelocity(bodyId, b2Vec2{velocity.x, velocity.y});
    }

    void PhysicsEngine2D::ApplyForceToCenter(b2BodyId bodyId, glm::vec2 force) const
    {
        b2Body_ApplyForceToCenter(bodyId, b2Vec2{force.x, force.y}, true);
    }

    void PhysicsEngine2D::ApplyTorque(b2BodyId bodyId, float torque) const
    {
        b2Body_ApplyTorque(bodyId, torque, true);
    }
} // namespace Engine
