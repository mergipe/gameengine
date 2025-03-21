#ifndef SHAPES_H
#define SHAPES_H

#include <glm/glm.hpp>

namespace Engine
{
    struct Rect {
        glm::vec2 position{0.0f};
        float width{0.0f};
        float height{0.0f};
    };
} // namespace Engine

#endif
