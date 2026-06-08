#pragma once

#include "core/StringId.h"

#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace Engine
{
    class Entity
    {
    public:
        explicit Entity(entt::handle handle);
        [[nodiscard]] const StringId& GetId() const;
        template <typename T> T& GetComponent() const;

    private:
        entt::handle m_handle{};
    };

    template <typename T>
    T& Entity::GetComponent() const
    {
        return m_handle.get<T>();
    }
} // namespace Engine
