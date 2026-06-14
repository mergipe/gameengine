#pragma once

#include "Scene.h"
#include "core/StringId.h"

#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace Engine
{

    class Entity
    {
    public:
        explicit Entity(entt::handle handle, Scene* scene);
        void SetHandle(entt::handle handle);
        [[nodiscard]] entt::handle GetHandle() const;
        [[nodiscard]] const StringId& GetId() const;
        [[nodiscard]] const StringId& GetTag() const;
        template <typename T> [[nodiscard]] bool HasComponent() const;
        template <typename T> [[nodiscard]] T& GetComponent() const;
        template <typename T> void AddComponent();
        template <typename T> void RemoveComponent();

    private:
        entt::handle m_handle{};
        Scene* m_scene{};
    };

    template <typename T> bool Entity::HasComponent() const { return m_handle.all_of<T>(); }

    template <typename T> T& Entity::GetComponent() const { return m_handle.get<T>(); }

    template <typename T> void Entity::AddComponent() { m_scene->AddComponent<T>(*this); }

    template <typename T> void Entity::RemoveComponent() { m_scene->RemoveComponent<T>(*this); }
} // namespace Engine
