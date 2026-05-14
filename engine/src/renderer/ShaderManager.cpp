#include "ShaderManager.h"

#include "core/FileSystem.h"
#include "core/Locator.h"

namespace Engine
{
    std::string ShaderManager::ReadShaderFile(const std::filesystem::path& relativePath)
    {
        return FileSystem::ReadStringFromFile(s_shadersPath / relativePath);
    }

    void ShaderManager::Clear() { m_shaders.clear(); }

    const Shader&
    ShaderManager::LoadShader(const StringId& shaderId, const std::filesystem::path& vertexShaderRelativePath,
                              const std::filesystem::path& fragmentShaderRelativePath,
                              const std::optional<std::filesystem::path>& geometryShaderRelativePath)
    {
        const std::string vertexShaderCode{ReadShaderFile(vertexShaderRelativePath)};
        const std::string fragmentShaderCode{ReadShaderFile(fragmentShaderRelativePath)};
        std::string geometryShaderCode{};
        if (geometryShaderRelativePath) {
            geometryShaderCode = ReadShaderFile(*geometryShaderRelativePath);
        }
        auto shader{std::make_unique<Shader>()};
        shader->Compile(vertexShaderCode.c_str(), fragmentShaderCode.c_str(),
                        geometryShaderRelativePath ? geometryShaderCode.c_str() : nullptr);
        m_shaders.insert(std::make_pair(shaderId, std::move(shader)));
        Locator::GetLogger()->Info("'{}' shader loaded", shaderId.GetString());
        return *m_shaders.at(shaderId);
    }

    const Shader& ShaderManager::GetShader(const StringId& shaderId) const { return *m_shaders.at(shaderId); }
} // namespace Engine
