#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <filesystem>

namespace Engine::Filesystem
{
    std::filesystem::path getBasePath();
    std::filesystem::path getRelativePath(std::string_view relativePath);
} // namespace Engine::Filesystem

#endif
