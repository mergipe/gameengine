#ifndef SCRIPTING_API_H
#define SCRIPTING_API_H

#include "Entity.h"

namespace Engine::ScriptingApi
{
    template <typename T>
    T& GetComponent(Entity& entity)
    {
        return entity.GetComponent<T>();
    }
} // namespace Engine::ScriptingApi

#endif
