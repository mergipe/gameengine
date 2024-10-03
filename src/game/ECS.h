#ifndef ECS_H
#define ECS_H

#include <Logger.h>
#include <bitset>
#include <cstddef>
#include <deque>
#include <memory>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace Engine {

constexpr size_t MAX_COMPONENTS{32};
using Signature = std::bitset<MAX_COMPONENTS>;

struct IComponent {
protected:
    static inline size_t s_nextId{0};
};

template <typename T> class Component : public IComponent {
public:
    static size_t getId() {
        const static auto s_id{s_nextId++};
        return s_id;
    };
};

class Entity {
private:
    size_t m_id{};

public:
    Entity();
    Entity(size_t id) : m_id{id} {}
    bool operator==(const Entity &e) const { return m_id == e.m_id; };
    bool operator<(const Entity &e) const { return m_id < e.m_id; };
    size_t getId() const { return m_id; }
};

class System {
private:
    Signature m_componentSignature{};
    std::vector<Entity> m_entities{};

protected:
    class Registry *m_registry{};
    template <typename T> void requireComponent();

public:
    System(Registry *registry) : m_registry{registry} {}
    System(const System &);
    ~System() = default;
    System &operator=(const System &);
    std::vector<Entity> getEntities() const { return m_entities; }
    const Signature &getComponentSignature() const { return m_componentSignature; }
    void addEntity(Entity entity) { m_entities.push_back(entity); };
    void removeEntity(Entity entity) { std::erase(m_entities, entity); };
};

class IPool {
public:
    virtual ~IPool() {}
};

template <typename T> class Pool : public IPool {
private:
    std::vector<T> m_objects{};

public:
    Pool(size_t size = 100) { m_objects.resize(size); }
    virtual ~Pool() = default;
    bool isEmpty() const { return m_objects.empty(); }
    size_t getSize() const { return m_objects.size(); }
    void resize(size_t size) { m_objects.resize(size); }
    void clear() { m_objects.clear(); }
    void add(T object) { m_objects.push_back(object); }
    void set(size_t index, T object) { m_objects[index] = object; }
    T &get(size_t index) { return static_cast<T &>(m_objects[index]); }
    T &operator[](size_t index) { return m_objects[index]; }
};

class Registry {
private:
    size_t m_entitiesCount{0};
    std::set<Entity> m_entitiesToBeAdded{};
    std::set<Entity> m_entitiesToBeKilled{};
    std::vector<std::shared_ptr<IPool>> m_componentPools{};
    std::vector<Signature> m_entityComponentSignatures{};
    std::unordered_map<std::type_index, std::shared_ptr<System>> m_systems{};
    std::deque<size_t> m_freeEntityIds{};

public:
    Registry() = default;
    void update();
    Entity createEntity();
    void killEntity(Entity entity);
    void addEntityToSystems(Entity entity);
    void removeEntityFromSystems(Entity entity);
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
    m_componentSignature.set(componentId);
}

template <typename T, typename... Args> void Registry::addComponent(Entity entity, Args &&...args) {
    const auto componentId{Component<T>::getId()};
    if (componentId >= m_componentPools.size()) {
        m_componentPools.resize(componentId + 1, nullptr);
    }
    if (!m_componentPools[componentId]) {
        m_componentPools[componentId] = std::make_shared<Pool<T>>();
    }
    std::shared_ptr<Pool<T>> componentPool{std::static_pointer_cast<Pool<T>>(m_componentPools[componentId])};
    const auto entityId{entity.getId()};
    if (entityId >= componentPool->getSize()) {
        componentPool->resize(m_entitiesCount);
    }
    T newComponent{std::forward<Args>(args)...};
    componentPool->set(entityId, newComponent);
    m_entityComponentSignatures[entityId].set(componentId);
    Logger::trace("Component {} added to entity {}", componentId, entityId);
}

template <typename T> void Registry::removeComponent(Entity entity) {
    const auto componentId{Component<T>::getId()};
    const auto entityId{entity.getId()};
    m_entityComponentSignatures[entityId].set(componentId, false);
    Logger::trace("Component {} removed from entity {}", componentId, entityId);
}

template <typename T> bool Registry::hasComponent(Entity entity) const {
    return m_entityComponentSignatures[entity.getId()].test(Component<T>::getId());
}

template <typename T> T &Registry::getComponent(Entity entity) const {
    auto componentPool{std::static_pointer_cast<Pool<T>>(m_componentPools[Component<T>::getId()])};
    return componentPool->get(entity.getId());
}

template <typename T, typename... Args> void Registry::addSystem(Args &&...args) {
    std::shared_ptr<T> newSystem{std::make_shared<T>(this, args...)};
    const auto systemId{std::type_index(typeid(T))};
    m_systems.insert(std::make_pair(systemId, newSystem));
    Logger::trace("System {} added to registry", systemId.name());
}

template <typename T> void Registry::removeSystem() {
    const auto systemId{std::type_index(typeid(T))};
    auto system{m_systems.find(systemId)};
    m_systems.erase(system);
    Logger::trace("System {} removed from registry", systemId.name());
}

template <typename T> bool Registry::hasSystem() const {
    return m_systems.find(std::type_index(typeid(T))) != m_systems.end();
}

template <typename T> T &Registry::getSystem() const {
    auto system{m_systems.find(std::type_index(typeid(T)))};
    return *(std::static_pointer_cast<T>(system->second));
}

} // namespace Engine

#endif
