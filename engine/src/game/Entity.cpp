#include "Entity.h"

#include "Components.h"

namespace Engine
{
    Entity::Entity(entt::entity id, entt::registry* registry)
        : m_id{id}, m_registry{registry}
    {
    }

    const StringId& Entity::GetId() const { return m_registry->get<IdComponent>(m_id).sid; }
} // namespace Engine
