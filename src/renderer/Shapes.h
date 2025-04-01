#ifndef SHAPES_H
#define SHAPES_H

#include <glm/glm.hpp>

namespace Engine
{
    struct Rect {
        float getLeftX() const { return position.x - width * pivotPoint.x; }
        float getRightX() const { return position.x + width * (1 - pivotPoint.x); }
        float getBottomY() const { return position.y - height * pivotPoint.y; }
        float getTopY() const { return position.y + height * (1 - pivotPoint.y); }
        glm::vec2 getCenter() const
        {
            return glm::vec2{position.x + width * (0.5 - pivotPoint.x),
                             position.y + height * (0.5 - pivotPoint.y)};
        }

        glm::vec2 position{0.0f};
        float width{0.0f};
        float height{0.0f};
        glm::vec2 pivotPoint{0.5f, 0.5f};
    };
} // namespace Engine

#endif
