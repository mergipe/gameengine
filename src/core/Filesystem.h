#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <filesystem>

namespace Engine::Filesystem
{
    std::filesystem::path getBasePath();
    std::filesystem::path getRelativePath(std::string_view relativePath);
    std::filesystem::path getLogsPath();
    std::filesystem::path getConfigPath();
    std::filesystem::path getResourcesPath();
} // namespace Engine::Filesystem

#endif
