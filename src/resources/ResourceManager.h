#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <SDL.h>
#include <filesystem>
#include <string_view>
#include <unordered_map>

namespace Engine
{
    class ResourceManager
    {
    public:
        ResourceManager(const std::filesystem::path& resourcesBasePath, SDL_Renderer& renderer);
        ~ResourceManager();
        void clearResources();
        void addTexture(std::string_view resourceId, const std::filesystem::path& relativeFilepath);
        SDL_Texture* getTexture(std::string_view resourceId) const { return m_textures.at(resourceId); };
        const std::filesystem::path& getResourcesBasePath() const { return m_resourcesBasePath; };
        std::filesystem::path getResourceAbsolutePath(const std::filesystem::path& relativeFilepath) const;

    private:
        std::unordered_map<std::string_view, SDL_Texture*> m_textures{};
        const std::filesystem::path m_resourcesBasePath{};
        SDL_Renderer& m_renderer;
    };
} // namespace Engine

#endif
