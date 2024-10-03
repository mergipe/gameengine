#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <SDL.h>
#include <string>
#include <string_view>
#include <unordered_map>

namespace Engine {

class ResourceManager {
private:
    std::unordered_map<std::string_view, SDL_Texture *> textures{};
    const std::string resourcesBasePath{};
    SDL_Renderer *renderer{};

public:
    ResourceManager(std::string_view resourcesBasePath, SDL_Renderer *renderer)
        : resourcesBasePath{resourcesBasePath}, renderer{renderer} {};
    ResourceManager(const ResourceManager &);
    ~ResourceManager() { clearResources(); };
    ResourceManager &operator=(const ResourceManager &);
    void clearResources();
    void addTexture(std::string_view resourceId, const std::string &relativeFilepath);
    SDL_Texture *getTexture(std::string_view resourceId) const { return textures.at(resourceId); };
    std::string_view getResourcesBasePath() const { return resourcesBasePath; };
};

} // namespace Engine

#endif
