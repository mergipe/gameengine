#ifndef SCENE_CONTEXT_H
#define SCENE_CONTEXT_H

#include "Systems.h"

#include <entt/entity/registry.hpp>

namespace Engine
{
    struct SceneContext {
        entt::registry registry{};
        std::unique_ptr<ScriptingSystem> scriptingSystem{};
    };
} // namespace Engine

#endif
