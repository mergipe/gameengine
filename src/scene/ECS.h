#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>

constexpr unsigned int MAX_COMPONENTS{32};
using Signature = std::bitset<MAX_COMPONENTS>;

class Component {};

class Entity {
private:
    int id{};

public:
    Entity(int id) : id{id} {}
    int getId() const { return id; }
};

class System {
private:
    Signature componentSignature{};
    std::vector<Entity> entities{};

public:
    System() = default;
    ~System() = default;
    std::vector<Entity> getEntities() const { return entities; }
    const Signature &getComponentSignature() const { return componentSignature; }
    void addEntity(Entity entity);
    void removeEntity(Entity entity);
};

class EntityManager {};

#endif
