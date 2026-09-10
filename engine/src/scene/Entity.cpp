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

    bool Entity::IsValid() const { return m_handle.valid(); }

    const StringId& Entity::GetId() const { return GetComponent<IdComponent>().value; }

    const StringId& Entity::GetTag() const { return GetComponent<TagComponent>().value; }

    void Entity::AddComponent(entt::id_type componentId)
    {
        m_handle.registry()->storage(componentId)->push(m_handle.entity());
    }

    void Entity::RemoveComponent(entt::id_type componentId)
    {
        m_handle.registry()->storage(componentId)->remove(m_handle.entity());
    }

    bool Entity::HasScript(const StringId& scriptClassId) const
    {
        return m_handle.registry()
            ->storage<ScriptInstanceComponent>(scriptClassId.GetId())
            .contains(m_handle.entity());
    }

    ScriptInstance* Entity::GetScript(const StringId& scriptClassId) const
    {
        auto& storage{m_handle.registry()->storage<ScriptInstanceComponent>(scriptClassId.GetId())};
        if (storage.contains(m_handle.entity())) {
            return &storage.get(m_handle.entity()).instance;
        }
        return nullptr;
    }

    void Entity::AddScript(ScriptInstance&& scriptInstance)
    {
        const auto scriptClassId{scriptInstance.GetClass().GetId()}; // should be by copy!
        const auto scriptInstanceComponent{ScriptInstanceComponent{std::move(scriptInstance)}};
        auto& storage{m_handle.registry()->storage<ScriptInstanceComponent>(scriptClassId.GetId())};
        if (storage.push(m_handle.entity(), &scriptInstanceComponent).index() != storage.end().index()) {
            GetOrAddComponent<ScriptBaseComponent>().classIds.push_back(scriptClassId);
        }
    }

    void Entity::RemoveScript(const StringId& scriptClassId)
    {
        auto& storage{m_handle.registry()->storage<ScriptInstanceComponent>(scriptClassId.GetId())};
        if (storage.remove(m_handle.entity())) {
            auto& scriptBaseComponent{GetComponent<ScriptBaseComponent>()};
            std::erase_if(scriptBaseComponent.classIds, [scriptClassId](const StringId& sid) {
                return sid.GetId() == scriptClassId.GetId();
            });
        }
    }

    std::optional<ScriptHandle> Entity::AddScriptOnNextStep(const StringId& scriptClassId)
    {
        return m_scene->AddScriptOnNextStep(*this, scriptClassId);
    }

    void Entity::RemoveScriptOnNextStep(const StringId& scriptClassId)
    {
        m_scene->RemoveScriptOnNextStep(*this, scriptClassId);
    }
} // namespace Engine
