#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Texture2D.h"
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

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

    class ResourceManager final
    {
    public:
        ResourceManager();
        ResourceManager(const ResourceManager&) = delete;
        ResourceManager(ResourceManager&&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;
        ResourceManager& operator=(ResourceManager&&) = delete;
        ~ResourceManager() = default;
        std::filesystem::path getResourcePath(const std::filesystem::path& relativePath) const;
        void clear();
        void loadTexture(std::string_view textureId, const std::filesystem::path& relativePath,
                         const TextureConfig& textureConfig);
        const Texture2D& getTexture(std::string_view textureId) const;

    private:
        std::unordered_map<std::string, std::unique_ptr<Texture2D>> m_textures{};
        std::unique_ptr<Texture2D> m_fallbackTexture{};
    };
} // namespace Engine

#endif
