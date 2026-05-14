#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <filesystem>

namespace Engine
{
    class FileSystem
    {
    public:
        FileSystem() = delete;
        static std::filesystem::path GetBasePath();
        static std::filesystem::path GetAbsolutePath(std::string_view relativePath);
        static bool IsDirectory(const std::filesystem::path& path);
        static bool IsFile(const std::filesystem::path& path);
        static std::string ReadStringFromFile(const std::filesystem::path& filePath);
    };
} // namespace Engine

#endif
