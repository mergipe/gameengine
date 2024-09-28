#include <AssetStore.h>
#include <Logger.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string>

void AssetStore::clearAssets() {
    for (auto texture : textures) {
        SDL_DestroyTexture(texture.second);
    }
    textures.clear();
}

void AssetStore::addTexture(std::string_view assetId, const std::string &relativeFilepath) {
    SDL_Texture *texture{IMG_LoadTexture(renderer, (assetsBasePath + relativeFilepath).c_str())};
    textures.emplace(assetId, texture);
    Logger::trace("Texture {} added to asset store ({})", assetId, relativeFilepath);
}
