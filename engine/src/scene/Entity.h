#ifndef ENTITY_H
#define ENTITY_H

#include "core/StringId.h"

#include <entt/entity/registry.hpp>

namespace Engine
{
    class Entity
    {
    public:
        Entity(entt::entity id, entt::registry* registry);
        const StringId& GetId() const;
        template <typename T> T& GetComponent();

    private:
        entt::entity m_id{};
        entt::registry* m_registry{};
    };

    template <typename T>
    T& Entity::GetComponent()
    {
        return m_registry->get<T>(m_id);
    }
} // namespace Engine

#endif
