#pragma once

#include "Components.h"

#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace Engine::ECSUtils
{
    void CopyEntity(entt::handle source, entt::handle target);

    template <typename... T> void CreateStorages(ComponentTypes<T...>, entt::registry& registry)
    {
        (registry.storage<T>(), ...);
    }
} // namespace Engine::ECSUtils
