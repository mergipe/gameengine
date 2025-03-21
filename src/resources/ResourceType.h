#ifndef RESOURCETYPE_H
#define RESOURCETYPE_H

#include <optional>
#include <string_view>

namespace Engine
{
    enum class ResourceType { texture, font };

    constexpr std::optional<ResourceType> getResourceType(std::string_view resourceTypeStr)
    {
        using enum ResourceType;
        if (resourceTypeStr == "texture")
            return texture;
        if (resourceTypeStr == "font")
            return font;
        return {};
    }
} // namespace Engine

#endif
