#ifndef ASSETSTORE_H
#define ASSETSTORE_H

#include <SDL.h>
#include <string>
#include <string_view>
#include <unordered_map>

class AssetStore {
private:
    std::unordered_map<std::string_view, SDL_Texture *> textures{};
    const std::string assetsBasePath{};
    SDL_Renderer *renderer{};

public:
    AssetStore(std::string_view assetsBasePath, SDL_Renderer *renderer)
        : assetsBasePath{assetsBasePath}, renderer{renderer} {};
    AssetStore(const AssetStore &);
    ~AssetStore() { clearAssets(); };
    AssetStore &operator=(const AssetStore &);
    void clearAssets();
    void addTexture(std::string_view assetId, const std::string &relativeFilepath);
    SDL_Texture *getTexture(std::string_view assetId) const { return textures.at(assetId); };
};

#endif
