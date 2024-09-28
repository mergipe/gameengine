#ifndef SYSTEMS_H
#define SYSTEMS_H

#include <Components.h>
#include <ECS.h>

class MovementSystem : public System {
public:
    MovementSystem(Registry *registry) : System(registry) {
        requireComponent<TransformComponent>();
        requireComponent<RigidBodyComponent>();
    }

    void update(float timeStep) {
        for (auto entity : getEntities()) {
            auto &transform{registry->getComponent<TransformComponent>(entity)};
            const auto rigidBody{registry->getComponent<RigidBodyComponent>(entity)};
            transform.position += rigidBody.velocity * timeStep;
        }
    }
};

#endif
