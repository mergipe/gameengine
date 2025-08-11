#ifndef BODY2D_H
#define BODY2D_H

#include <box2d/box2d.h>
#include <glm/glm.hpp>
#include <optional>
#include <string_view>

namespace Engine
{
    constexpr std::optional<b2BodyType> parseBodyType2D(std::string_view bodyTypeStr)
    {
        if (bodyTypeStr == "static") {
            return b2_staticBody;
        }
        if (bodyTypeStr == "dynamic") {
            return b2_dynamicBody;
        }
        if (bodyTypeStr == "kinematic") {
            return b2_kinematicBody;
        }
        return {};
    }

    struct MaterialData2D {
        float friction{0.6f};
        float restitution{0.0f};
        float rollingResistance{0.0f};
        float tangentSpeed{0.0f};
    };

    struct ShapeData2D {
        MaterialData2D materialData{};
        float density{1.0f};
        bool isTrigger{false};
    };

    struct BodyData2D {
        glm::vec2 position{};
        b2BodyType type{b2_staticBody};
        float rotation{0.0f};
        float gravityScale{0.0f};
        float angularDamping{0.0f};
        float linearDamping{0.0f};
    };

    class Body2D
    {
    public:
        Body2D() = default;
        explicit Body2D(b2BodyId id)
            : m_id{id}
        {
        }
        b2BodyId getId() const { return m_id; }
        void applyForceToCenter(const glm::vec2& force);
        void applyTorque(float torque);
        glm::vec2 getLinearVelocity() const;
        glm::vec2 getPosition() const;
        float getRotationAngle() const;
        void setLinearVelocity(const glm::vec2& velocity);

    private:
        b2BodyId m_id{};
    };
} // namespace Engine

#endif
