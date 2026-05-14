#ifndef SCRIPTING_API_H
#define SCRIPTING_API_H

#include "Entity.h"

#include <glm/glm.hpp>

namespace Engine::ScriptingApi
{
    // TODO: maybe implement these components classes and other things in lua using exposed c++ functions
    class Component
    {
    protected:
        explicit Component(Entity entity);
        Entity m_entity;
    };

    class Transform : public Component
    {
    public:
        explicit Transform(Entity entity);
        [[nodiscard]] glm::vec3 GetPosition() const;
        [[nodiscard]] glm::vec3 GetRotation() const;
        [[nodiscard]] glm::vec3 GetScale() const;
        [[nodiscard]] glm::vec3 GetRight() const;
        [[nodiscard]] glm::vec3 GetUp() const;
        [[nodiscard]] glm::vec3 GetForward() const;
        void SetPosition(glm::vec3 position) const;
        void SetRotation(glm::vec3 rotation) const;
        void SetScale(glm::vec3 scale) const;
    };

    class RigidBody2D : public Component
    {
    public:
        explicit RigidBody2D(Entity entity);
        [[nodiscard]] glm::vec2 GetPosition() const;
        [[nodiscard]] float GetRotationAngle() const;
        [[nodiscard]] glm::vec2 GetLinearVelocity() const;
        void SetLinearVelocity(glm::vec2 velocity) const;
        void ApplyForceToCenter(glm::vec2 force) const;
        void ApplyTorque(float torque) const;
    };
} // namespace Engine::ScriptingApi

#endif
