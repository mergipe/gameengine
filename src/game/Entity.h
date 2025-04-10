#ifndef ENTITY_H
#define ENTITY_H

#include "core/StringId.h"
#include <entt/entt.hpp>

namespace Engine
{
    class Entity
    {
    public:
        Entity(entt::entity id, entt::registry* registry);
        const StringId& getId() const;
        template <typename T> T& getComponent();

    private:
        entt::entity m_id{};
        entt::registry* m_registry{};
    };

    template <typename T>
    T& Entity::getComponent()
    {
        return m_registry->get<T>(m_id);
    }
} // namespace Engine

#endif
