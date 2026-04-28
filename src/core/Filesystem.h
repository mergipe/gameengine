#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <filesystem>

namespace Engine
{
    class Filesystem
    {
    public:
        Filesystem() = delete;
        static std::filesystem::path GetBasePath();
        static std::filesystem::path GetRelativePath(std::string_view relativePath);
        static std::filesystem::path GetLogsPath();
        static std::filesystem::path GetConfigPath();
        static std::filesystem::path GetResourcesPath();
        static std::filesystem::path GetModulesPath();
        static std::filesystem::path GetScriptingLibraryPath();
    };
} // namespace Engine

#endif
