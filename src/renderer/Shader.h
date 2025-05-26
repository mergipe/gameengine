#ifndef SHADER_H
#define SHADER_H

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
        GLuint getId() const { return m_id; }
        void compile(const char* vertexShaderCode, const char* fragmentShaderCode,
                     const char* geometryShaderCode = nullptr);
        const Shader& use() const;
        const Shader& setUniform(std::string_view name, bool value) const;
        const Shader& setUniform(std::string_view name, GLint value) const;
        const Shader& setUniform(std::string_view name, GLfloat value) const;
        const Shader& setUniform(std::string_view name, const glm::mat4& matrix) const;
        const Shader& setUniform(std::string_view name, const glm::vec3& vector) const;
        const Shader& setUniform(std::string_view name, const glm::vec4& vector) const;

    private:
        static void checkCompileErrors(GLuint id, std::string_view type);
        static constexpr GLsizei s_infoLogBufferMaxLength{1024};
        GLuint m_id{};
    };
} // namespace Engine

#endif
