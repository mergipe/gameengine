#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "renderer/Texture.h"
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
        void addTexture(std::string_view resourceId, std::unique_ptr<Texture> texture);
        const Texture& getTexture(const std::string& resourceId) const
        {
            return *(m_textures.at(resourceId));
        };
        const std::filesystem::path& getResourcesPath() const { return m_resourcesPath; };
        std::filesystem::path getResourcePath(const std::filesystem::path& relativeFilepath) const
        {
            return m_resourcesPath / relativeFilepath;
        }

    private:
        std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures{};
        const std::filesystem::path m_resourcesPath{};
    };
} // namespace Engine

#endif
