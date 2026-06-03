#ifndef SHADER_H
#define SHADER_H

#include "core/StringId.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string_view>

namespace Engine
{
    class Shader
    {
    public:
        Shader() = default;
        Shader(const Shader&) = delete;
        Shader(Shader&&) = delete;
        Shader& operator=(const Shader&) = delete;
        Shader& operator=(Shader&&) = delete;
        ~Shader();
        [[nodiscard]] GLuint GetId() const { return m_id; }
        void Create(const char* vertexShaderCode, const char* fragmentShaderCode,
                    const char* geometryShaderCode = nullptr);
        const Shader& BindUniformBlock(std::string_view blockName, GLuint bindingPoint) const;
        const Shader& Use() const;
        const Shader& SetUniform(std::string_view name, bool value) const;
        const Shader& SetUniform(std::string_view name, GLint value) const;
        const Shader& SetUniform(std::string_view name, U32 value) const;
        const Shader& SetUniform(std::string_view name, GLfloat value) const;
        const Shader& SetUniform(std::string_view name, const glm::mat4& matrix) const;
        const Shader& SetUniform(std::string_view name, glm::vec3 vector) const;
        const Shader& SetUniform(std::string_view name, const glm::vec4& vector) const;

    private:
        static void CheckCompileErrors(GLuint id, const StringId& type);
        static constexpr GLsizei s_infoLogBufferMaxLength{1024};
        GLuint m_id{};
    };
} // namespace Engine

#endif
