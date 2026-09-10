#include "Components.h"

#include "core/Locator.h"
#include "core/Math.h"
#include "scene/Components.h"

namespace Engine::ScriptingApi
{
    Entity::Entity(Engine::Entity* entityPtr)
        : m_entityPtr{entityPtr}
    {
    }

    Engine::Entity* Entity::GetEntityPtr() const { return m_entityPtr; }

    const StringId& Entity::GetId() const { return m_entityPtr->GetId(); }

    const StringId& Entity::GetTag() const { return m_entityPtr->GetTag(); }

    Entity Component::GetEntity() const { return Entity{m_entity}; }

    Component::Component(Engine::Entity* entity)
        : m_entity{entity}
    {
    }

    Transform::Transform(Engine::Entity* entity)
        : Component{entity}
    {
    }

    glm::vec3 Transform::GetPosition() const { return m_entity->GetComponent<TransformComponent>().position; }

    glm::vec3 Transform::GetRotation() const { return m_entity->GetComponent<TransformComponent>().rotation; }

    glm::vec3 Transform::GetScale() const { return m_entity->GetComponent<TransformComponent>().scale; }

    glm::vec3 Transform::GetUp() const
    {
        return Math::CalculateUpVector(m_entity->GetComponent<TransformComponent>().rotation);
    }

    glm::vec3 Transform::GetRight() const
    {
        return Math::CalculateRightVector(m_entity->GetComponent<TransformComponent>().rotation);
    }

    glm::vec3 Transform::GetForward() const
    {
        return Math::CalculateForwardVector(m_entity->GetComponent<TransformComponent>().rotation);
    }

    void Transform::SetPosition(glm::vec3 position) const
    {
        m_entity->GetComponent<TransformComponent>().position = position;
    }

    void Transform::SetRotation(glm::vec3 rotation) const
    {
        m_entity->GetComponent<TransformComponent>().rotation = rotation;
    }

    void Transform::SetScale(glm::vec3 scale) const
    {
        m_entity->GetComponent<TransformComponent>().scale = scale;
    }

    RigidBody2D::RigidBody2D(Engine::Entity* entity)
        : Component{entity}
    {
    }

    glm::vec2 RigidBody2D::GetLinearVelocity() const
    {
        const auto& rigidBody{m_entity->GetComponent<RigidBody2DComponent>()};
        if (rigidBody.bodyId) {
            return Locator::GetPhysicsEngine2D()->GetLinearVelocity(rigidBody.bodyId.value());
        }
        return rigidBody.bodyData.linearVelocity;
    }

    float RigidBody2D::GetAngularVelocity() const
    {
        const auto& rigidBody{m_entity->GetComponent<RigidBody2DComponent>()};
        if (rigidBody.bodyId) {
            return Locator::GetPhysicsEngine2D()->GetAngularVelocity(rigidBody.bodyId.value());
        }
        return rigidBody.bodyData.angularVelocity;
    }

    void RigidBody2D::SetLinearVelocity(glm::vec2 velocity) const
    {
        auto& rigidBody{m_entity->GetComponent<RigidBody2DComponent>()};
        rigidBody.bodyData.linearVelocity = velocity;
        if (rigidBody.bodyId) {
            Locator::GetPhysicsEngine2D()->SetLinearVelocity(rigidBody.bodyId.value(), velocity);
        }
    }

    void RigidBody2D::SetAngularVelocity(float velocity) const
    {
        auto& rigidBody{m_entity->GetComponent<RigidBody2DComponent>()};
        rigidBody.bodyData.angularVelocity = velocity;
        if (rigidBody.bodyId) {
            Locator::GetPhysicsEngine2D()->SetAngularVelocity(rigidBody.bodyId.value(), velocity);
        }
    }

    void RigidBody2D::ApplyForce(glm::vec2 force) const
    {
        const auto& rigidBody{m_entity->GetComponent<RigidBody2DComponent>()};
        if (rigidBody.bodyId) {
            Locator::GetPhysicsEngine2D()->ApplyForce(rigidBody.bodyId.value(), force);
        }
    }

    void RigidBody2D::ApplyTorque(float torque) const
    {
        const auto& rigidBody{m_entity->GetComponent<RigidBody2DComponent>()};
        if (rigidBody.bodyId) {
            Locator::GetPhysicsEngine2D()->ApplyTorque(rigidBody.bodyId.value(), torque);
        }
    }

    Collider2D::Collider2D(Engine::Entity* entity)
        : Component{entity}
    {
    }

    void Collider2D::SetShapeId(Shape2DId shapeId) { m_shapeId = shapeId; }
} // namespace Engine::ScriptingApi
