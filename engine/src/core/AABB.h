#ifndef AABB_H
#define AABB_H

#include <glm/glm.hpp>

namespace Engine
{
    struct AABB {
        float GetLeftX() const { return center.x - width * 0.5f; }
        float GetRightX() const { return center.x + width * 0.5f; }
        float GetBottomY() const { return center.y - height * 0.5f; }
        float GetTopY() const { return center.y + height * 0.5f; }

        glm::vec2 center{0.0f};
        float width{0.0f};
        float height{0.0f};
    };
} // namespace Engine

#endif
