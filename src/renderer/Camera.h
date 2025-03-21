#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

namespace Engine
{
    struct Camera {
        glm::vec2 position{};
        float width{};
        float height{};
    };
} // namespace Engine

#endif
