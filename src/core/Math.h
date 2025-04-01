#ifndef MATH_H
#define MATH_H

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Engine::Math
{
    constexpr glm::mat4 getTransformationMatrix(const glm::vec3& position, const glm::vec3& rotation,
                                                const glm::vec3& scale)
    {
        return glm::translate(glm::mat4{1.0f}, position) * glm::mat4_cast(glm::quat(rotation)) *
               glm::scale(glm::mat4{1.0f}, scale);
    }
} // namespace Engine::Math

#endif
