#include "ResourceManager.h"
#include "core/Logger.h"
#include <SDL_image.h>

namespace Engine
{
    ResourceManager::ResourceManager(const std::filesystem::path& resourcesBasePath)
        : m_resourcesBasePath{resourcesBasePath}
    {
        int flags{IMG_INIT_PNG};
        if (!(IMG_Init(flags) & flags)) {
            Logger::error("Failed to initialize SDL_image: {}", IMG_GetError());
        }
    };

    ResourceManager::~ResourceManager()
    {
        clearResources();
        IMG_Quit();
    };

    void ResourceManager::clearResources()
    {
        for (auto& texture : m_textures) {
            SDL_DestroyTexture(texture.second);
            texture.second = nullptr;
        }
        m_textures.clear();
    }

    void ResourceManager::addTexture(std::string_view resourceId,
                                     const std::filesystem::path& relativeFilepath, SDL_Renderer* renderer)
    {
        const std::filesystem::path fullPath{m_resourcesBasePath / relativeFilepath};
        SDL_Texture* texture{IMG_LoadTexture(renderer, fullPath.c_str())};
        if (!texture) {
            Logger::error("Failed to load texture from {}: {}", fullPath.c_str(), IMG_GetError());
        }
        m_textures.emplace(resourceId, texture);
        Logger::info("Texture {} loaded by the resource manager ({})", resourceId, relativeFilepath.c_str());
    }

    std::filesystem::path
    ResourceManager::getResourceAbsolutePath(const std::filesystem::path& relativeFilepath) const
    {
        return m_resourcesBasePath / relativeFilepath;
    }
} // namespace Engine
