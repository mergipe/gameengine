#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <glm/glm.hpp>

struct TransformComponent {
    glm::vec2 position{};
    glm::vec2 scale{};
    double rotation{};

    TransformComponent(glm::vec2 position = glm::vec2(0, 0), glm::vec2 scale = glm::vec2(1, 1),
                       double rotation = 0.0)
        : position{position}, scale{scale}, rotation{rotation} {}
};

struct RigidBodyComponent {
    glm::vec2 velocity{};

    RigidBodyComponent(glm::vec2 velocity = glm::vec2(0, 0)) : velocity{velocity} {}
};

#endif
