#ifndef ECS_H
#define ECS_H

#include <Logger.h>
#include <bitset>
#include <cstddef>
#include <memory>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <vector>

constexpr size_t MAX_COMPONENTS{32};
using Signature = std::bitset<MAX_COMPONENTS>;

struct IComponent {
protected:
    static inline size_t nextId{0};
};

template <typename T> class Component : public IComponent {
public:
    static size_t getId() {
        const static auto id{nextId++};
        return id;
    };
};

class Entity {
private:
    size_t id{};

public:
    Entity(size_t id) : id{id} {}
    bool operator==(const Entity &e) const { return id != e.id; };
    bool operator<(const Entity &e) const { return id < e.id; };
    size_t getId() const { return id; }
};

class System {
private:
    Signature componentSignature{};
    std::vector<Entity> entities{};

protected:
    class Registry *registry{};
    template <typename T> void requireComponent();

public:
    System(Registry *registry) : registry{registry} {}
    System(const System &);
    ~System() = default;
    System &operator=(const System &);
    std::vector<Entity> getEntities() const { return entities; }
    const Signature &getComponentSignature() const { return componentSignature; }
    void addEntity(Entity entity) { entities.push_back(entity); };
    void removeEntity(Entity entity) { std::erase(entities, entity); };
};

class IPool {
public:
    virtual ~IPool() {}
};

template <typename T> class Pool : public IPool {
private:
    std::vector<T> objects{};

public:
    Pool(size_t size = 100) { objects.resize(size); }
    virtual ~Pool() = default;
    bool isEmpty() const { return objects.empty(); }
    size_t getSize() const { return objects.size(); }
    void resize(size_t size) { objects.resize(size); }
    void clear() { objects.clear(); }
    void add(T object) { objects.push_back(object); }
    void set(size_t index, T object) { objects[index] = object; }
    T &get(size_t index) { return static_cast<T &>(objects[index]); }
    T &operator[](size_t index) { return objects[index]; }
};

class Registry {
private:
    size_t entitiesCount{0};
    std::set<Entity> entitiesToBeAdded{};
    std::set<Entity> entitiesToBeKilled{};
    std::vector<std::shared_ptr<IPool>> componentPools{};
    std::vector<Signature> entityComponentSignatures{};
    std::unordered_map<std::type_index, std::shared_ptr<System>> systems{};

public:
    Registry() = default;
    void update();
    Entity createEntity();
    void killEntity(Entity entity);
    void addEntityToSystems(Entity entity);
    template <typename T, typename... Args> void addComponent(Entity entity, Args &&...args);
    template <typename T> void removeComponent(Entity entity);
    template <typename T> bool hasComponent(Entity entity) const;
    template <typename T> T &getComponent(Entity entity) const;
    template <typename T, typename... Args> void addSystem(Args &&...args);
    template <typename T> void removeSystem();
    template <typename T> bool hasSystem() const;
    template <typename T> T &getSystem() const;
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
        componentPools[componentId] = std::make_shared<Pool<T>>();
    }
    std::shared_ptr<Pool<T>> componentPool{std::static_pointer_cast<Pool<T>>(componentPools[componentId])};
    const auto entityId{entity.getId()};
    if (entityId >= componentPool->getSize()) {
        componentPool->resize(entitiesCount);
    }
    T newComponent{std::forward<Args>(args)...};
    componentPool->set(entityId, newComponent);
    entityComponentSignatures[entityId].set(componentId);
    Logger::trace("Component {} added to entity {}", componentId, entityId);
}

template <typename T> void Registry::removeComponent(Entity entity) {
    const auto componentId{Component<T>::getId()};
    const auto entityId{entity.getId()};
    entityComponentSignatures[entityId].set(componentId, false);
    Logger::trace("Component {} removed from entity {}", componentId, entityId);
}

template <typename T> bool Registry::hasComponent(Entity entity) const {
    return entityComponentSignatures[entity.getId()].test(Component<T>::getId());
}

template <typename T> T &Registry::getComponent(Entity entity) const {
    auto componentPool{std::static_pointer_cast<Pool<T>>(componentPools[Component<T>::getId()])};
    return componentPool->get(entity.getId());
}

template <typename T, typename... Args> void Registry::addSystem(Args &&...args) {
    std::shared_ptr<T> newSystem{std::make_shared<T>(this, std::forward<T>(args)...)};
    const auto systemId{std::type_index(typeid(T))};
    systems.insert(std::make_pair(systemId, newSystem));
    Logger::trace("System {} added to registry", systemId.name());
}

template <typename T> void Registry::removeSystem() {
    const auto systemId{std::type_index(typeid(T))};
    auto system{systems.find(systemId)};
    systems.erase(system);
    Logger::trace("System {} removed from registry", systemId.name());
}

template <typename T> bool Registry::hasSystem() const {
    return systems.find(std::type_index(typeid(T))) != systems.end();
}

template <typename T> T &Registry::getSystem() const {
    auto system{systems.find(std::type_index(typeid(T)))};
    return *(std::static_pointer_cast<T>(system->second));
}

#endif
