#pragma once

#include "core/StringId.h"
#include "physics/Physics2DTypes.h"
#include "scene/Entity.h"

#include <glm/glm.hpp>

namespace Engine::ScriptingApi
{
    class Component;

    class Entity
    {
    public:
        explicit Entity(Engine::Entity* entityPtr);
        Engine::Entity* GetEntityPtr() const;
        [[nodiscard]] const StringId& GetId() const;
        [[nodiscard]] const StringId& GetTag() const;

    private:
        Engine::Entity* m_entityPtr{};
    };

    class Component
    {
    public:
        virtual ~Component() = default;
        Entity GetEntity() const;

    protected:
        explicit Component(Engine::Entity* entity);
        Engine::Entity* m_entity{};
    };

    class Transform : public Component
    {
    public:
        explicit Transform(Engine::Entity* entity);
        [[nodiscard]] glm::vec3 GetPosition() const;
        [[nodiscard]] glm::vec3 GetRotation() const;
        [[nodiscard]] glm::vec3 GetScale() const;
        [[nodiscard]] glm::vec3 GetUp() const;
        [[nodiscard]] glm::vec3 GetRight() const;
        [[nodiscard]] glm::vec3 GetForward() const;
        void SetPosition(glm::vec3 position) const;
        void SetRotation(glm::vec3 rotation) const;
        void SetScale(glm::vec3 scale) const;
    };

    class RigidBody2D : public Component
    {
    public:
        explicit RigidBody2D(Engine::Entity* entity);
        [[nodiscard]] glm::vec2 GetLinearVelocity() const;
        [[nodiscard]] float GetAngularVelocity() const;
        void SetLinearVelocity(glm::vec2 velocity) const;
        void SetAngularVelocity(float velocity) const;
        void ApplyForce(glm::vec2 force) const;
        void ApplyTorque(float torque) const;
    };

    class Collider2D : public Component
    {
    public:
        explicit Collider2D(Engine::Entity* entity);
        void SetShapeId(Shape2DId shapeId);

    private:
        Shape2DId m_shapeId{};
    };
} // namespace Engine::ScriptingApi
