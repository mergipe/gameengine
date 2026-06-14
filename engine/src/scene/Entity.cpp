#include "Entity.h"

#include "Components.h"

namespace Engine
{
    Entity::Entity(entt::handle handle, Scene* scene)
        : m_handle{handle}, m_scene{scene}
    {
    }

    void Entity::SetHandle(entt::handle handle) { m_handle = handle; }

    entt::handle Entity::GetHandle() const { return m_handle; }

    const StringId& Entity::GetId() const { return GetComponent<IdComponent>().value; }

    const StringId& Entity::GetTag() const { return GetComponent<TagComponent>().value; }
} // namespace Engine
