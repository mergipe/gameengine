#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <filesystem>

namespace Engine
{
    class Filesystem
    {
    public:
        static std::filesystem::path getBasePath();
        static std::filesystem::path getRelativePath(std::string_view relativePath);
        static std::filesystem::path getLogsPath();
        static std::filesystem::path getConfigPath();
        static std::filesystem::path getResourcesPath();
    };
} // namespace Engine

#endif
