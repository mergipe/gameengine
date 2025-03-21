#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include "Shader.h"
#include "core/Filesystem.h"
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

namespace Engine
{
    class ShaderManager final
    {
    public:
        ShaderManager() = default;
        ShaderManager(const ShaderManager&) = delete;
        ShaderManager(ShaderManager&&) = delete;
        ShaderManager& operator=(const ShaderManager&) = delete;
        ShaderManager& operator=(ShaderManager&&) = delete;
        ~ShaderManager() = default;
        void clear();
        const Shader& loadShader(std::string_view shaderId,
                                 const std::filesystem::path& vertexShaderRelativePath,
                                 const std::filesystem::path& fragmentShaderRelativePath,
                                 std::optional<std::filesystem::path> geometryShaderRelativePath = {});
        const Shader& getShader(std::string_view shaderId) const;

    private:
        std::unordered_map<std::string, std::unique_ptr<Shader>> m_shaders{};
        const std::filesystem::path m_shadersPath{Filesystem::getResourcesPath() / "shaders"};
    };
} // namespace Engine

#endif
