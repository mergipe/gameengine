#ifndef ENTITY_LOADER_H
#define ENTITY_LOADER_H

#include "core/Yaml.h"

#include <entt/entity/registry.hpp>

namespace Engine
{
    class EntityLoader
    {
    public:
        static entt::entity Load(entt::registry& registry, const YAML::Node& entityNode);
    };
} // namespace Engine

#endif
