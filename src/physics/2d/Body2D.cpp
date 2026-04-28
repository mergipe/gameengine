#include "Body2D.h"

#include <box2d/box2d.h>

namespace Engine
{
    void Body2D::ApplyForceToCenter(const glm::vec2& force)
    {
        b2Body_ApplyForceToCenter(m_id, b2Vec2(force.x, force.y), true);
    }

    void Body2D::ApplyTorque(float torque) { b2Body_ApplyTorque(m_id, torque, true); }

    glm::vec2 Body2D::GetLinearVelocity() const
    {
        const b2Vec2 velocity{b2Body_GetLinearVelocity(m_id)};
        return glm::vec2{velocity.x, velocity.y};
    }

    glm::vec2 Body2D::GetPosition() const
    {
        const b2Vec2 position{b2Body_GetPosition(m_id)};
        return glm::vec2{position.x, position.y};
    }

    float Body2D::GetRotationAngle() const { return b2Rot_GetAngle(b2Body_GetRotation(m_id)); }

    void Body2D::SetLinearVelocity(const glm::vec2& velocity)
    {
        b2Body_SetLinearVelocity(m_id, b2Vec2{velocity.x, velocity.y});
    }
} // namespace Engine
