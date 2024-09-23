#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>

constexpr unsigned int MAX_COMPONENTS{32};
using Signature = std::bitset<MAX_COMPONENTS>;

struct BaseComponent {
protected:
    static inline int nextId{};
};

template <typename T> class Component : public BaseComponent {
    static int getId() {
        const static auto id{nextId++};
        return id;
    };
};

class Entity {
private:
    int id{};

public:
    Entity(int id) : id{id} {}
    bool operator==(const Entity &);
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
    template <typename T> void requireComponent();
};

class EntityManager {};

template <typename T> void System::requireComponent() {
    const auto componentId{Component<T>::getId()};
    componentSignature.set(componentId);
}

#endif
