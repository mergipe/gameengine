#pragma once

#include "Scene.h"
#include "core/StringId.h"

#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace Engine
{
    class ScriptClass;
    class ScriptInstance;

    class Entity
    {
    public:
        explicit Entity(entt::handle handle, Scene* scene);
        void SetHandle(entt::handle handle);
        [[nodiscard]] entt::handle GetHandle() const;
        [[nodiscard]] bool IsValid() const;
        [[nodiscard]] const StringId& GetId() const;
        [[nodiscard]] const StringId& GetTag() const;
        template <typename T> [[nodiscard]] bool HasComponent() const;
        template <typename T> [[nodiscard]] T& GetComponent() const;
        template <typename T> [[nodiscard]] T& GetOrAddComponent();
        void AddComponent(entt::id_type componentId);
        void RemoveComponent(entt::id_type componentId);
        template <typename T> void AddComponentOnNextStep();
        template <typename T> void RemoveComponentOnNextStep();
        [[nodiscard]] bool HasScript(const StringId& scriptClassId) const;
        [[nodiscard]] ScriptInstance* GetScript(const StringId& scriptClassId) const;
        void AddScript(ScriptInstance&& scriptInstance);
        void RemoveScript(const StringId& scriptClassId);
        std::optional<ScriptHandle> AddScriptOnNextStep(const StringId& scriptClassId);
        void RemoveScriptOnNextStep(const StringId& scriptClassId);

    private:
        entt::handle m_handle{};
        Scene* m_scene{};
    };

    template <typename T> bool Entity::HasComponent() const { return m_handle.all_of<T>(); }

    template <typename T> T& Entity::GetComponent() const { return m_handle.get<T>(); }

    template <typename T> T& Entity::GetOrAddComponent() { return m_handle.get_or_emplace<T>(); }

    template <typename T> void Entity::AddComponentOnNextStep() { m_scene->AddComponentOnNextStep<T>(*this); }

    template <typename T> void Entity::RemoveComponentOnNextStep()
    {
        m_scene->RemoveComponentOnNextStep<T>(*this);
    }
} // namespace Engine
