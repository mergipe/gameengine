#include "Filesystem.h"
#include <SDL3/SDL_filesystem.h>

namespace Engine::Filesystem
{
    std::filesystem::path getBasePath()
    {
        return std::filesystem::canonical(SDL_GetBasePath()).parent_path();
    }

    std::filesystem::path getRelativePath(std::string_view relativePath)
    {
        return getBasePath() / relativePath;
    }
} // namespace Engine::Filesystem
