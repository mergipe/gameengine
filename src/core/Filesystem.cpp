#include "Filesystem.h"
#include <SDL3/SDL_filesystem.h>

namespace Engine
{
    std::filesystem::path Filesystem::getBasePath()
    {
        static std::filesystem::path s_basePath{std::filesystem::canonical(SDL_GetBasePath()).parent_path()};
        return s_basePath;
    }

    std::filesystem::path Filesystem::getRelativePath(std::string_view relativePath)
    {
        return getBasePath() / relativePath;
    }

    std::filesystem::path Filesystem::getLogsPath()
    {
        static std::filesystem::path s_logsPath{getBasePath() / "logs"};
        return s_logsPath;
    }

    std::filesystem::path Filesystem::getConfigPath()
    {
        static std::filesystem::path s_configPath{getBasePath() / "config"};
        return s_configPath;
    }

    std::filesystem::path Filesystem::getResourcesPath()
    {
        static std::filesystem::path s_resourcesPath{getBasePath() / "resources"};
        return s_resourcesPath;
    }
} // namespace Engine
