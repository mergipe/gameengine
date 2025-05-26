#include "Shader.h"
#include "core/Logger.h"
#include <glm/gtc/type_ptr.hpp>

namespace Engine
{
    Shader::~Shader() { glDeleteProgram(m_id); }

    void Shader::compile(const char* vertexShaderCode, const char* fragmentShaderCode,
                         const char* geometryShaderCode)
    {
        const GLuint vertexShader{glCreateShader(GL_VERTEX_SHADER)};
        glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
        glCompileShader(vertexShader);
        checkCompileErrors(vertexShader, "vertex");
        const GLuint fragmentShader{glCreateShader(GL_FRAGMENT_SHADER)};
        glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);
        glCompileShader(fragmentShader);
        checkCompileErrors(vertexShader, "fragment");
        GLuint geometryShader{};
        if (geometryShaderCode) {
            geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometryShader, 1, &geometryShaderCode, nullptr);
            glCompileShader(geometryShader);
            checkCompileErrors(geometryShader, "geometry");
        }
        m_id = glCreateProgram();
        glAttachShader(m_id, vertexShader);
        glAttachShader(m_id, fragmentShader);
        if (geometryShaderCode) {
            glAttachShader(m_id, geometryShader);
        }
        glLinkProgram(m_id);
        checkCompileErrors(m_id, "program");
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        if (geometryShaderCode) {
            glDeleteShader(geometryShader);
        }
    }

    const Shader& Shader::use() const
    {
        glUseProgram(m_id);
        return *this;
    }

    const Shader& Shader::setUniform(std::string_view name, bool value) const
    {
        glUniform1i(glGetUniformLocation(m_id, name.data()), static_cast<GLint>(value));
        return *this;
    }

    const Shader& Shader::setUniform(std::string_view name, GLint value) const
    {
        glUniform1i(glGetUniformLocation(m_id, name.data()), value);
        return *this;
    }

    const Shader& Shader::setUniform(std::string_view name, GLfloat value) const
    {
        glUniform1f(glGetUniformLocation(m_id, name.data()), value);
        return *this;
    }

    const Shader& Shader::setUniform(std::string_view name, const glm::mat4& matrix) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_id, name.data()), 1, GL_FALSE, glm::value_ptr(matrix));
        return *this;
    }

    const Shader& Shader::setUniform(std::string_view name, const glm::vec3& vector) const
    {
        glUniform3f(glGetUniformLocation(m_id, name.data()), vector.x, vector.y, vector.z);
        return *this;
    }

    const Shader& Shader::setUniform(std::string_view name, const glm::vec4& vector) const
    {
        glUniform4f(glGetUniformLocation(m_id, name.data()), vector.x, vector.y, vector.z, vector.w);
        return *this;
    }

    void Shader::checkCompileErrors(GLuint id, std::string_view type)
    {
        GLint success{};
        std::array<GLchar, s_infoLogBufferMaxLength> infoLog{};
        if (type == "program") {
            glGetProgramiv(id, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(id, s_infoLogBufferMaxLength, nullptr, infoLog.data());
                Logger::error("Error linking shader program: {}", infoLog.data());
            }
        } else {
            glGetShaderiv(id, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(id, s_infoLogBufferMaxLength, nullptr, infoLog.data());
                Logger::error("Error compiling {} shader: {}", type, infoLog.data());
            }
        }
    }
} // namespace Engine
