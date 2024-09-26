#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <vector>

constexpr unsigned int MAX_COMPONENTS{32};
using Signature = std::bitset<MAX_COMPONENTS>;

struct IComponent {
protected:
    static inline int nextId{};
};

template <typename T> class Component : public IComponent {
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

class IPool {
public:
    virtual ~IPool() {}
};

template <typename T> class Pool : public IPool {
private:
    std::vector<T> objects{};

public:
    Pool(int size = 100) { objects.resize(size); }
    virtual ~Pool() = default;
    bool isEmpty() const { return objects.empty(); }
    int getSize() const { return objects.size(); }
    void resize() { objects.resize(); }
    void clear() { objects.clear(); }
    void add(T object) { objects.push_back(object); }
    void set(int index, T object) { object[index] = object; }
    T &get(int index) { return static_cast<T &>(objects[index]); }
    T &operator[](unsigned int index) { return objects[index]; }
};

class Registry {
private:
    int entitiesCount{0};
    std::set<Entity> entitiesToBeAdded{};
    std::set<Entity> entitiesToBeKilled{};
    std::vector<IPool *> componentPools{};
    std::vector<Signature> entityComponentSignatures{};
    std::unordered_map<std::type_index, System *> systems{};

public:
    Registry() = default;
    void update();
    Entity createEntity();
    void killEntity(Entity entity);
    void addEntityToSystems(Entity entity);
    template <typename T, typename... Args> void addComponent(Entity entity, Args &&...args);
};

template <typename T> void System::requireComponent() {
    const auto componentId{Component<T>::getId()};
    componentSignature.set(componentId);
}

template <typename T, typename... Args> void Registry::addComponent(Entity entity, Args &&...args) {
    const auto componentId{Component<T>::getId()};
    if (componentId >= componentPools.size()) {
        componentPools.resize(componentId + 1, nullptr);
    }
    if (!componentPools[componentId]) {
        componentPools[componentId] = new Pool<T>();
    }
    Pool<T> *componentPool = componentPools[componentId];
    const auto entityId{entity.getId()};
    if (entityId >= componentPool->getSize()) {
        componentPool->resize(entitiesCount);
    }
    T newComponent{std::forward<Args>(args)...};
    componentPool->set(entityId, newComponent);
    entityComponentSignatures[entityId].set(componentId);
}

#endif
