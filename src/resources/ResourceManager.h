#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "core/Renderer.h"
#include <filesystem>
#include <string>
#include <string_view>
#include <unordered_map>

namespace Engine
{
    class ResourceManager final
    {
    public:
        explicit ResourceManager(const std::filesystem::path& resourcesBasePath);
        ResourceManager(const ResourceManager&) = delete;
        ResourceManager(ResourceManager&&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;
        ResourceManager& operator=(ResourceManager&&) = delete;
        ~ResourceManager();
        void addTexture(std::string_view resourceId, const std::filesystem::path& relativeFilepath,
                        Renderer& renderer);
        const Texture& getTexture(const std::string& resourceId) const { return m_textures.at(resourceId); };
        const std::filesystem::path& getResourcesBasePath() const { return m_resourcesBasePath; };
        std::filesystem::path getResourceAbsolutePath(const std::filesystem::path& relativeFilepath) const;

    private:
        std::unordered_map<std::string, Texture> m_textures{};
        const std::filesystem::path m_resourcesBasePath{};
    };
} // namespace Engine

#endif
