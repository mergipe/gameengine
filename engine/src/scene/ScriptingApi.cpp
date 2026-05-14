#include "ScriptingApi.h"

#include "Components.h"
#include "core/Locator.h"

namespace Engine::ScriptingApi
{
    Component::Component(Entity entity)
        : m_entity{entity}
    {
    }

    Transform::Transform(Entity entity)
        : Component{entity}
    {
    }

    glm::vec3 Transform::GetPosition() const { return m_entity.GetComponent<TransformComponent>().position; }

    glm::vec3 Transform::GetRotation() const { return m_entity.GetComponent<TransformComponent>().rotation; }

    glm::vec3 Transform::GetScale() const { return m_entity.GetComponent<TransformComponent>().scale; }

    glm::vec3 Transform::GetRight() const
    {
        const auto& transformComponent = m_entity.GetComponent<TransformComponent>();
        return Math::GetRotationMatrix(transformComponent.rotation) * glm::vec4{1.0f, 0.0f, 0.0f, 1.0f};
    }

    glm::vec3 Transform::GetUp() const
    {
        const auto& transformComponent = m_entity.GetComponent<TransformComponent>();
        return Math::GetRotationMatrix(transformComponent.rotation) * glm::vec4{0.0f, 1.0f, 0.0f, 1.0f};
    }

    glm::vec3 Transform::GetForward() const
    {
        const auto& transformComponent = m_entity.GetComponent<TransformComponent>();
        return Math::GetRotationMatrix(transformComponent.rotation) * glm::vec4{0.0f, 0.0f, 1.0f, 1.0f};
    }

    void Transform::SetPosition(glm::vec3 position) const
    {
        m_entity.GetComponent<TransformComponent>().position = position;
    }

    void Transform::SetRotation(glm::vec3 rotation) const
    {
        m_entity.GetComponent<TransformComponent>().rotation = rotation;
    }

    void Transform::SetScale(glm::vec3 scale) const
    {
        m_entity.GetComponent<TransformComponent>().scale = scale;
    }

    RigidBody2D::RigidBody2D(Entity entity)
        : Component{entity}
    {
    }

    glm::vec2 RigidBody2D::GetPosition() const
    {
        const auto& runtimeComponent = m_entity.GetComponent<RigidBody2DRuntimeComponent>();
        return Locator::GetPhysicsEngine2D()->GetPosition(runtimeComponent.bodyId);
    }

    float RigidBody2D::GetRotationAngle() const
    {
        const auto& runtimeComponent = m_entity.GetComponent<RigidBody2DRuntimeComponent>();
        return Locator::GetPhysicsEngine2D()->GetRotationAngle(runtimeComponent.bodyId);
    }

    glm::vec2 RigidBody2D::GetLinearVelocity() const
    {
        const auto& runtimeComponent = m_entity.GetComponent<RigidBody2DRuntimeComponent>();
        return Locator::GetPhysicsEngine2D()->GetLinearVelocity(runtimeComponent.bodyId);
    }

    void RigidBody2D::SetLinearVelocity(glm::vec2 velocity) const
    {
        const auto& runtimeComponent = m_entity.GetComponent<RigidBody2DRuntimeComponent>();
        return Locator::GetPhysicsEngine2D()->SetLinearVelocity(runtimeComponent.bodyId, velocity);
    }

    void RigidBody2D::ApplyForceToCenter(glm::vec2 force) const
    {
        const auto& runtimeComponent = m_entity.GetComponent<RigidBody2DRuntimeComponent>();
        return Locator::GetPhysicsEngine2D()->ApplyForceToCenter(runtimeComponent.bodyId, force);
    }

    void RigidBody2D::ApplyTorque(float torque) const
    {
        const auto& runtimeComponent = m_entity.GetComponent<RigidBody2DRuntimeComponent>();
        return Locator::GetPhysicsEngine2D()->ApplyTorque(runtimeComponent.bodyId, torque);
    }
} // namespace Engine::ScriptingApi
