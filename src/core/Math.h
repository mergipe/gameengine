#ifndef MATH_H
#define MATH_H

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Engine::Math
{
    constexpr glm::mat4 getRotationMatrix(const glm::vec3& rotationAngles)
    {
        return glm::mat4_cast(glm::quat(rotationAngles));
    }

    constexpr glm::mat4 getTranslationMatrix(const glm::vec3& position)
    {
        return glm::translate(glm::mat4{1.0f}, position);
    }

    constexpr glm::mat4 getScaleMatrix(const glm::vec3& scale) { return glm::scale(glm::mat4{1.0f}, scale); }

    constexpr glm::mat4 getTransformationMatrix(const glm::vec3& position, const glm::vec3& rotationAngles,
                                                const glm::vec3& scale)
    {
        return getTranslationMatrix(position) * getRotationMatrix(rotationAngles) * getScaleMatrix(scale);
    }
} // namespace Engine::Math

#endif
