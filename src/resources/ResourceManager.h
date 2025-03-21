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
    class ResourceManager final
    {
    public:
        explicit ResourceManager(const std::filesystem::path& resourcesPath)
            : m_resourcesPath{resourcesPath}
        {
        }
        ResourceManager(const ResourceManager&) = delete;
        ResourceManager(ResourceManager&&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;
        ResourceManager& operator=(ResourceManager&&) = delete;
        ~ResourceManager() = default;
        std::filesystem::path getResourcePath(const std::filesystem::path& relativePath) const;
        const std::filesystem::path& getResourcesPath() const { return m_resourcesPath; };
        void clear();
        void loadTexture(std::string_view textureId, const std::filesystem::path& relativePath,
                         const TextureConfig& textureConfig);
        const Texture2D& getTexture(std::string_view textureId) const;

    private:
        std::unordered_map<std::string, std::unique_ptr<Texture2D>> m_textures{};
        const std::filesystem::path m_resourcesPath{};
    };
} // namespace Engine

#endif
