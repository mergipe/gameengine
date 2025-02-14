#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <SDL.h>
#include <filesystem>
#include <string_view>
#include <unordered_map>

namespace Engine
{
    class ResourceManager final
    {
    public:
        explicit ResourceManager(const std::filesystem::path& resourcesBasePath);
        ~ResourceManager();
        void clearResources();
        void addTexture(std::string_view resourceId, const std::filesystem::path& relativeFilepath,
                        SDL_Renderer* renderer);
        SDL_Texture* getTexture(const std::string& resourceId) const { return m_textures.at(resourceId); };
        const std::filesystem::path& getResourcesBasePath() const { return m_resourcesBasePath; };
        std::filesystem::path getResourceAbsolutePath(const std::filesystem::path& relativeFilepath) const;

    private:
        std::unordered_map<std::string, SDL_Texture*> m_textures{};
        const std::filesystem::path m_resourcesBasePath{};
    };
} // namespace Engine

#endif
