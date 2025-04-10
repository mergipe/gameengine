#ifndef SCRIPTINGAPI_H
#define SCRIPTINGAPI_H

#include "Entity.h"
#include <entt/entt.hpp>

namespace Engine::ScriptingApi
{
    template <typename T>
    T& getComponent(Entity& entity)
    {
        return entity.getComponent<T>();
    }
} // namespace Engine::ScriptingApi

#endif
