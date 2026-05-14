#include "Entity.h"

#include "Components.h"

namespace Engine
{
    Entity::Entity(entt::handle handle)
        : m_handle{handle}
    {
    }

    const StringId& Entity::GetId() const { return m_handle.get<IdComponent>().value; }
} // namespace Engine
