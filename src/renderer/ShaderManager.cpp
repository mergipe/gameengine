#include "ShaderManager.h"
#include "core/IO.h"
#include "core/Logger.h"

namespace Engine
{
    void ShaderManager::clear() { m_shaders.clear(); }

    const Shader& ShaderManager::loadShader(const StringId& shaderId,
                                            const std::filesystem::path& vertexShaderRelativePath,
                                            const std::filesystem::path& fragmentShaderRelativePath,
                                            std::optional<std::filesystem::path> geometryShaderRelativePath)
    {
        const std::string vertexShaderCode{IO::readStringFromFile(m_shadersPath / vertexShaderRelativePath)};
        const std::string fragmentShaderCode{
            IO::readStringFromFile(m_shadersPath / fragmentShaderRelativePath)};
        std::string geometryShaderCode{};
        if (geometryShaderRelativePath) {
            geometryShaderCode = IO::readStringFromFile(m_shadersPath / *geometryShaderRelativePath);
        }
        auto shader{std::make_unique<Shader>()};
        shader->compile(vertexShaderCode.c_str(), fragmentShaderCode.c_str(),
                        geometryShaderRelativePath ? geometryShaderCode.c_str() : nullptr);
        m_shaders.insert(std::make_pair(shaderId.id, std::move(shader)));
        Logger::info("'{}' shader loaded", shaderId.str.data());
        return *m_shaders.at(shaderId.id);
    }

    const Shader& ShaderManager::getShader(const StringId& shaderId) const
    {
        return *(m_shaders.at(shaderId.id));
    }
} // namespace Engine
