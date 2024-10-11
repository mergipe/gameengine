#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <SDL.h>
#include <string>
#include <string_view>
#include <unordered_map>

namespace Engine
{
    class ResourceManager
    {
    private:
        std::unordered_map<std::string_view, SDL_Texture*> m_textures{};
        const std::string m_resourcesBasePath{};
        SDL_Renderer& m_renderer;

    public:
        ResourceManager(std::string_view resourcesBasePath, SDL_Renderer& renderer);
        ~ResourceManager();
        void clearResources();
        void addTexture(std::string_view resourceId, const std::string& relativeFilepath);
        SDL_Texture* getTexture(std::string_view resourceId) const { return m_textures.at(resourceId); };
        std::string_view getResourcesBasePath() const { return m_resourcesBasePath; };
    };
} // namespace Engine

#endif
