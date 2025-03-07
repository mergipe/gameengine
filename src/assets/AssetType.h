#ifndef ASSETTYPE_H
#define ASSETTYPE_H

#include <optional>
#include <string_view>

namespace Engine
{
    enum class AssetType { texture, font };

    constexpr std::optional<AssetType> getAssetType(std::string_view assetTypeStr)
    {
        using enum AssetType;
        if (assetTypeStr == "texture")
            return texture;
        if (assetTypeStr == "font")
            return font;
        return {};
    }
} // namespace Engine

#endif
