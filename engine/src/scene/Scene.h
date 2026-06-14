#pragma once

#include "events/EventBus.h"
#include "events/Events.h"

namespace Engine
{
    class Entity;

    class Scene final
    {
    public:
        explicit Scene(entt::registry* registry);
        Scene(const Scene&) = delete;
        Scene(Scene&&) = delete;
        Scene& operator=(const Scene&) = delete;
        Scene& operator=(Scene&&) = delete;
        ~Scene();

        void Start();
        void Update(float timeStep);
        void Render(float frameExtrapolationTimeStep);
        void OnViewportResize(int width, int height);

        Entity& CreateEntity();
        Entity& CreateEntity(entt::handle entityTemplate);
        void DestroyEntity(const Entity& entity);
        template <typename T> void AddComponent(Entity& entity);
        template <typename T> void RemoveComponent(Entity& entity);

    private:
        struct EntityComponent {
            Entity* entity{};
            entt::id_type componentId{};
        };

        void CreateEntities();
        void DestroyEntities();
        void AddComponents();
        void RemoveComponents();

        void OnAddRigidBody2DComponent(entt::registry& registry, entt::entity entity);
        void OnAddBoxCollider2DComponent(entt::registry& registry, entt::entity entity);
        void OnAddCircleCollider2DComponent(entt::registry& registry, entt::entity entity);
        void OnRemoveRigidBody2DComponent(entt::registry& registry, entt::entity entity);
        void OnRemoveBoxCollider2DComponent(entt::registry& registry, entt::entity entity);
        void OnRemoveCircleCollider2DComponent(entt::registry& registry, entt::entity entity);

        void StartPhysics2D();
        void StartPlayerInput();
        void StartScripts();

        void OnInputCommand(const InputEvent& event);

        void UpdatePlayerInput(EventBus* eventBus);
        void UpdatePhysics2D(float timeStep);
        void UpdateScripts(float timeStep);

        entt::registry m_stagingRegistry{};

        std::unordered_map<StringId, Entity> m_entityById{};

        std::vector<Entity*> m_entitiesToCreate{};
        std::vector<Entity> m_entitiesToDestroy{};
        std::vector<EntityComponent> m_componentsToAdd{};
        std::vector<EntityComponent> m_componentsToRemove{};

        entt::registry* m_mainRegistry{};
    };

    template <typename T> void Scene::AddComponent(Entity& entity)
    {
        m_componentsToAdd.emplace_back(&entity, m_mainRegistry->storage<T>().info().hash());
    }

    template <typename T> void Scene::RemoveComponent(Entity& entity)
    {
        m_componentsToRemove.emplace_back(&entity, m_mainRegistry->storage<T>().info().hash());
    }
} // namespace Engine
