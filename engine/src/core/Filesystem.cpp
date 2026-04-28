#include "Filesystem.h"

namespace Engine
{
    std::filesystem::path Filesystem::GetBasePath()
    {
        static std::filesystem::path s_basePath{std::filesystem::canonical(ENGINE_BASE_DIR)};
        return s_basePath;
    }

    std::filesystem::path Filesystem::GetRelativePath(std::string_view relativePath)
    {
        return GetBasePath() / relativePath;
    }

    std::filesystem::path Filesystem::GetLogsPath()
    {
        static std::filesystem::path s_logsPath{GetBasePath() / "logs"};
        return s_logsPath;
    }

    std::filesystem::path Filesystem::GetConfigPath()
    {
        static std::filesystem::path s_configPath{GetBasePath() / "config"};
        return s_configPath;
    }

    std::filesystem::path Filesystem::GetShadersPath()
    {
        static std::filesystem::path s_shadersPath{GetBasePath() / "shaders"};
        return s_shadersPath;
    }

    std::filesystem::path Filesystem::GetResourcesPath()
    {
        static std::filesystem::path s_resourcesPath{GetBasePath() / "resources"};
        return s_resourcesPath;
    }

    std::filesystem::path Filesystem::GetModulesPath()
    {
        static std::filesystem::path s_modulesPath{GetBasePath() / "modules"};
        return s_modulesPath;
    }

    std::filesystem::path Filesystem::GetScriptingLibraryPath()
    {
        static std::filesystem::path s_scriptsPath{GetModulesPath() / "lua_scripting"};
        return s_scriptsPath;
    }
} // namespace Engine
