#ifndef MATH_H
#define MATH_H

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Engine::Math
{
    constexpr glm::mat4 GetRotationMatrix(glm::vec3 rotationAngles)
    {
        return glm::mat4_cast(glm::quat(rotationAngles));
    }

    constexpr glm::mat4 GetTranslationMatrix(glm::vec3 position)
    {
        return glm::translate(glm::mat4{1.0f}, position);
    }

    constexpr glm::mat4 GetScaleMatrix(glm::vec3 scale) { return glm::scale(glm::mat4{1.0f}, scale); }

    constexpr glm::mat4 GetTransformMatrix(glm::vec3 position, glm::vec3 rotationAngles, glm::vec3 scale,
                                           glm::vec3 pivotPoint = glm::vec3{0.0f})
    {
        return GetTranslationMatrix(position) * GetRotationMatrix(rotationAngles) * GetScaleMatrix(scale) *
               GetTranslationMatrix(-pivotPoint);
    }

    constexpr glm::mat3 GetTransformMatrix(glm::vec2 position, float rotationInRadians, glm::vec2 scale)
    {
        const float cos{glm::cos(rotationInRadians)};
        const float sin{glm::sin(rotationInRadians)};
        return glm::mat3{
            scale.x * cos, -scale.y * sin, position.x, //
            scale.x * sin, scale.y * cos,  position.y, //
            0.0f,          0.0f,           1.0f        //
        };
    }
} // namespace Engine::Math

#endif
