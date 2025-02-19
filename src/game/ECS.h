#ifndef ECS_H
#define ECS_H

#include "core/Logger.h"
#include <bitset>
#include <deque>
#include <memory>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace Engine
{
    constexpr size_t MAX_COMPONENTS{32};
    constexpr size_t DEFAULT_POOL_SIZE{100};
    using Signature = std::bitset<MAX_COMPONENTS>;

    struct IComponent {
    public:
        IComponent(const IComponent&) = delete;
        IComponent(IComponent&&) = delete;
        IComponent& operator=(const IComponent&) = delete;
        IComponent& operator=(IComponent&&) = delete;
        virtual ~IComponent() = default;

    protected:
        static inline size_t s_nextId{0};
    };

    template <typename TComponent>
    class Component final : public IComponent
    {
    public:
        static size_t getId();
    };

    template <typename TComponent>
    size_t Component<TComponent>::getId()
    {
        const static auto s_id{s_nextId++};
        return s_id;
    }

    class Registry;

    class Entity final
    {
    public:
        Entity(size_t id, Registry* registry)
            : m_id{id}, m_registry{registry}
        {
        }
        bool operator==(const Entity& e) const { return m_id == e.m_id; };
        bool operator<(const Entity& e) const { return m_id < e.m_id; };
        size_t getId() const { return m_id; }
        void kill();
        template <typename TComponent, typename... TArgs> void addComponent(TArgs&&... args);
        template <typename TComponent> void removeComponent();
        template <typename TComponent> bool hasComponent() const;
        template <typename TComponent> TComponent& getComponent() const;

    private:
        size_t m_id{};
        Registry* m_registry{};
    };

    class System
    {
    public:
        System(const System&) = delete;
        System(System&&) = delete;
        System& operator=(const System&) = delete;
        System& operator=(System&&) = delete;
        virtual ~System() = default;
        std::vector<Entity> getEntities() const { return m_entities; }
        const Signature& getComponentSignature() const { return m_componentSignature; }
        void addEntity(Entity entity) { m_entities.push_back(entity); };
        void removeEntity(Entity entity) { std::erase(m_entities, entity); };

    protected:
        System() = default;
        template <typename TComponent> void requireComponent();

    private:
        std::vector<Entity> m_entities{};
        Signature m_componentSignature{};
    };

    template <typename TComponent>
    void System::requireComponent()
    {
        m_componentSignature.set(Component<TComponent>::getId());
    }

    class IPool
    {
    public:
        IPool(const IPool&) = delete;
        IPool(IPool&&) = delete;
        IPool& operator=(const IPool&) = delete;
        IPool& operator=(IPool&&) = delete;
        virtual ~IPool() = default;

    protected:
        IPool() = default;
    };

    template <typename T>
    class Pool final : public IPool
    {
    public:
        explicit Pool(size_t size = DEFAULT_POOL_SIZE) { m_objects.resize(size); }
        T& operator[](size_t index) { return m_objects[index]; }
        bool isEmpty() const { return m_objects.empty(); }
        size_t getSize() const { return m_objects.size(); }
        void resize(size_t size) { m_objects.resize(size); }
        void clear() { m_objects.clear(); }
        void add(const T& object) { m_objects.push_back(object); }
        void set(size_t index, const T& object) { m_objects[index] = object; }
        T& get(size_t index) { return m_objects[index]; }

    private:
        std::vector<T> m_objects{};
    };

    class Registry final
    {
    public:
        void update();
        Entity createEntity();
        void killEntity(Entity entity);
        void addEntityToSystems(Entity entity);
        void removeEntityFromSystems(Entity entity);
        template <typename TComponent, typename... TArgs> void addComponent(Entity entity, TArgs&&... args);
        template <typename TComponent> void removeComponent(Entity entity);
        template <typename TComponent> bool hasComponent(Entity entity) const;
        template <typename TComponent> TComponent& getComponent(Entity entity) const;
        template <typename TSystem, typename... TArgs> void addSystem(TArgs&&... args);
        template <typename TSystem> void removeSystem();
        template <typename TSystem> bool hasSystem() const;
        template <typename TSystem> TSystem& getSystem() const;

    private:
        std::deque<size_t> m_freeEntityIds{};
        std::unordered_map<std::type_index, std::shared_ptr<System>> m_systems{};
        std::set<Entity> m_entitiesToBeAdded{};
        std::set<Entity> m_entitiesToBeKilled{};
        std::vector<std::shared_ptr<IPool>> m_componentPools{};
        std::vector<Signature> m_entityComponentSignatures{};
        size_t m_entitiesCount{0};
    };

    template <typename TComponent, typename... TArgs>
    void Entity::addComponent(TArgs&&... args)
    {
        m_registry->addComponent<TComponent>(*this, std::forward<TArgs>(args)...);
    }

    template <typename TComponent>
    void Entity::removeComponent()
    {
        m_registry->removeComponent<TComponent>(*this);
    }

    template <typename TComponent>
    bool Entity::hasComponent() const
    {
        return m_registry->hasComponent<TComponent>(*this);
    }

    template <typename TComponent>
    TComponent& Entity::getComponent() const
    {
        return m_registry->getComponent<TComponent>(*this);
    }

    template <typename TComponent, typename... TArgs>
    void Registry::addComponent(Entity entity, TArgs&&... args)
    {
        const auto componentId{Component<TComponent>::getId()};
        if (componentId >= m_componentPools.size()) {
            m_componentPools.resize(componentId + 1, nullptr);
        }
        if (!m_componentPools[componentId]) {
            m_componentPools[componentId] = std::make_shared<Pool<TComponent>>();
        }
        const std::shared_ptr<Pool<TComponent>> componentPool{
            std::static_pointer_cast<Pool<TComponent>>(m_componentPools[componentId])};
        const auto entityId{entity.getId()};
        if (entityId >= componentPool->getSize()) {
            componentPool->resize(m_entitiesCount);
        }
        const TComponent newComponent{std::forward<TArgs>(args)...};
        componentPool->set(entityId, newComponent);
        m_entityComponentSignatures[entityId].set(componentId);
        Logger::trace("Component {} added to entity {}", componentId, entityId);
    }

    template <typename TComponent>
    void Registry::removeComponent(Entity entity)
    {
        const auto componentId{Component<TComponent>::getId()};
        const auto entityId{entity.getId()};
        m_entityComponentSignatures[entityId].set(componentId, false);
        Logger::trace("Component {} removed from entity {}", componentId, entityId);
    }

    template <typename TComponent>
    bool Registry::hasComponent(Entity entity) const
    {
        return m_entityComponentSignatures[entity.getId()].test(Component<TComponent>::getId());
    }

    template <typename TComponent>
    TComponent& Registry::getComponent(Entity entity) const
    {
        const auto componentPool{
            std::static_pointer_cast<Pool<TComponent>>(m_componentPools[Component<TComponent>::getId()])};
        return componentPool->get(entity.getId());
    }

    template <typename TSystem, typename... TArgs>
    void Registry::addSystem(TArgs&&... args)
    {
        const std::shared_ptr<TSystem> newSystem{std::make_shared<TSystem>(std::forward<TArgs>(args)...)};
        const auto systemId{std::type_index(typeid(TSystem))};
        m_systems.insert(std::make_pair(systemId, newSystem));
        Logger::trace("System {} added to registry", systemId.name());
    }

    template <typename TSystem>
    void Registry::removeSystem()
    {
        const auto systemId{std::type_index(typeid(TSystem))};
        const auto system{m_systems.find(systemId)};
        m_systems.erase(system);
        Logger::trace("System {} removed from registry", systemId.name());
    }

    template <typename TSystem>
    bool Registry::hasSystem() const
    {
        return m_systems.find(std::type_index(typeid(TSystem))) != m_systems.end();
    }

    template <typename TSystem>
    TSystem& Registry::getSystem() const
    {
        const auto system{m_systems.find(std::type_index(typeid(TSystem)))};
        return *(std::static_pointer_cast<TSystem>(system->second));
    }
} // namespace Engine

#endif
