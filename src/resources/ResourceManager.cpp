#include <Logger.h>
#include <ResourceManager.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string>

void ResourceManager::clearResources() {
    for (auto texture : textures) {
        SDL_DestroyTexture(texture.second);
    }
    textures.clear();
}

void ResourceManager::addTexture(std::string_view resourceId, const std::string &relativeFilepath) {
    SDL_Texture *texture{IMG_LoadTexture(renderer, (resourcesBasePath + relativeFilepath).c_str())};
    textures.emplace(resourceId, texture);
    Logger::trace("Texture {} loaded by the resource manager ({})", resourceId, relativeFilepath);
}
