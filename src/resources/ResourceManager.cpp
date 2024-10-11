#include "ResourceManager.h"
#include "core/Logger.h"
#include <SDL.h>
#include <SDL_image.h>
#include <string>

namespace Engine
{
    ResourceManager::ResourceManager(std::string_view resourcesBasePath, SDL_Renderer& renderer)
        : m_resourcesBasePath{resourcesBasePath}, m_renderer{renderer}
    {
        int flags = IMG_INIT_PNG;
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

    void ResourceManager::addTexture(std::string_view resourceId, const std::string& relativeFilepath)
    {
        const std::string fullPath{(m_resourcesBasePath + relativeFilepath)};
        SDL_Texture* texture{IMG_LoadTexture(&m_renderer, fullPath.c_str())};
        if (!texture) {
            Logger::error("Failed to load texture from {}: {}", fullPath, IMG_GetError());
        }
        m_textures.emplace(resourceId, texture);
        Logger::trace("Texture {} loaded by the resource manager ({})", resourceId, relativeFilepath);
    }
} // namespace Engine
