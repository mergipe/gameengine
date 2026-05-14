#include "FileSystem.h"

#include "Locator.h"

#include <fstream>
#include <sstream>

namespace Engine
{
    std::filesystem::path FileSystem::GetBasePath()
    {
        static std::filesystem::path s_basePath{std::filesystem::canonical(ENGINE_BASE_DIR)};
        return s_basePath;
    }

    std::filesystem::path FileSystem::GetAbsolutePath(std::string_view relativePath)
    {
        return GetBasePath() / relativePath;
    }

    bool FileSystem::IsDirectory(const std::filesystem::path& path)
    {
        return std::filesystem::is_directory(path);
    }

    bool FileSystem::IsFile(const std::filesystem::path& path)
    {
        return std::filesystem::is_regular_file(path);
    }

    std::string FileSystem::ReadStringFromFile(const std::filesystem::path& filePath)
    {
        std::ifstream fileStream{filePath};
        if (!fileStream.is_open()) {
            Locator::GetLogger()->Error("File {} not found", filePath.c_str());
            return {};
        }
        std::stringstream buffer{};
        buffer << fileStream.rdbuf();
        fileStream.close();
        return buffer.str();
    }
} // namespace Engine
