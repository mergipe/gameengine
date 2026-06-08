#pragma once

#include "Shader.h"
#include "core/FileSystem.h"
#include "core/StringId.h"

#include <filesystem>
#include <memory>
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
        void Clear();
        const Shader& LoadShader(const StringId& shaderId,
                                 const std::filesystem::path& vertexShaderRelativePath,
                                 const std::filesystem::path& fragmentShaderRelativePath,
                                 const std::optional<std::filesystem::path>& geometryShaderRelativePath = {});
        const Shader& GetShader(const StringId& shaderId) const;

    private:
        static std::string ReadShaderFile(const std::filesystem::path& relativePath);
        static inline const std::filesystem::path s_shadersPath{FileSystem::GetAbsolutePath("shaders")};
        std::unordered_map<StringId, std::unique_ptr<Shader>> m_shaders{};
    };
} // namespace Engine
