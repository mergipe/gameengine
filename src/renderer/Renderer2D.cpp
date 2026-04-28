#include "Renderer2D.h"

#include "ShaderManager.h"
#include "Shapes.h"
#include "core/Locator.h"
#include "core/Math.h"

#include <SDL3/SDL.h>
#include <array>
#include <cstdlib>
#include <glad/glad.h>

namespace Engine
{
    glm::mat4 GetModelTransformation(const Rect& rect, const glm::vec3& rotation)
    {
        return Math::GetTransformationMatrix(glm::vec3{rect.GetCenter(), 0.0f}, rotation,
                                             glm::vec3{rect.width, rect.height, 1.0f});
    }

    glm::mat4 GetTextureTransformation(const Texture2D& texture, const Rect& textureArea)
    {
        const float widthRatio{textureArea.width / static_cast<float>(texture.GetWidth())};
        const float heightRatio{textureArea.height / static_cast<float>(texture.GetHeight())};
        const glm::vec2 position{textureArea.GetLeftX() / static_cast<float>(texture.GetWidth()),
                                 textureArea.GetTopY() / static_cast<float>(texture.GetHeight())};
        return Math::GetTransformationMatrix(glm::vec3{position, 0.0f}, glm::vec3{0.0f},
                                             glm::vec3{widthRatio, heightRatio, 1.0f});
    }

    Renderer2D::Renderer2D(Window* window)
        : m_window{window}
    {
    }

    Renderer2D::~Renderer2D()
    {
        glDeleteVertexArrays(1, &m_spriteVao);
        glDeleteVertexArrays(1, &m_quadVao);
        Locator::GetLogger()->Info("2D Renderer destroyed");
    }

    void Renderer2D::Init()
    {
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress))) {
            Locator::GetLogger()->Critical("Failed to initialize GLAD");
            std::abort();
        }
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        SetViewport(0, 0, m_window->GetWidth(), m_window->GetHeight());
        SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        constexpr std::array<GLfloat, 24> spriteVertexData{
            -0.5f, 0.5f,  0.0f, 1.0f, // top left
            0.5f,  -0.5f, 1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, // bottom left
            -0.5f, 0.5f,  0.0f, 1.0f, // top left
            0.5f,  0.5f,  1.0f, 1.0f, // top right
            0.5f,  -0.5f, 1.0f, 0.0f  // bottom right
        };
        glGenVertexArrays(1, &m_spriteVao);
        GLuint vbo{};
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(spriteVertexData), spriteVertexData.data(), GL_STATIC_DRAW);
        glBindVertexArray(m_spriteVao);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), static_cast<void*>(nullptr));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                              reinterpret_cast<void*>(2 * sizeof(GLfloat)));
        constexpr std::array<GLfloat, 8> quadVertices{
            -0.5f, -0.5f, // bottom left
            -0.5f, 0.5f,  // top left
            0.5f,  0.5f,  // top right
            0.5f,  -0.5f  // bottom right
        };
        glGenVertexArrays(1, &m_quadVao);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices.data(), GL_STATIC_DRAW);
        glBindVertexArray(m_quadVao);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void*>(nullptr));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        m_shaderManager = std::make_unique<ShaderManager>();
        m_shaderManager->LoadShader(m_spriteShaderId, "sprite.vert", "sprite.frag")
            .Use()
            .SetUniform("textureSampler", 0);
        m_shaderManager->LoadShader(m_primitivesShaderId, "primitives.vert", "primitives.frag").Use();
        Locator::GetLogger()->Info("2D Renderer initialized");
    }

    void Renderer2D::SetViewport(int x, int y, int width, int height) { glViewport(x, y, width, height); }

    void Renderer2D::SetViewportSize(int width, int height) { SetViewport(0, 0, width, height); }

    void Renderer2D::SetClearColor(float red, float green, float blue, float alpha)
    {
        glClearColor(red, green, blue, alpha);
    }

    void Renderer2D::SetupCamera(const Camera& camera)
    {
        m_cameraTransformation = camera.GetCameraTransformation();
        m_projectionTransformation = camera.GetProjectionTransformation();
    }

    void Renderer2D::DrawRectangle(const Rect& rect, const glm::vec4& color, const glm::vec3& rotation)
    {
        m_shaderManager->GetShader(m_primitivesShaderId)
            .Use()
            .SetUniform("model", GetModelTransformation(rect, rotation))
            .SetUniform("camera", m_cameraTransformation)
            .SetUniform("projection", m_projectionTransformation)
            .SetUniform("color", color);
        glBindVertexArray(m_quadVao);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        glBindVertexArray(0);
    }

    void Renderer2D::DrawSprite(const Rect& spriteGeometry, const glm::vec3& rotation,
                                const Texture2D& texture, const Rect& textureArea, const glm::vec3& color)
    {
        m_shaderManager->GetShader(m_spriteShaderId)
            .Use()
            .SetUniform("model", GetModelTransformation(spriteGeometry, rotation))
            .SetUniform("camera", m_cameraTransformation)
            .SetUniform("projection", m_projectionTransformation)
            .SetUniform("color", color)
            .SetUniform("textureTransform", GetTextureTransformation(texture, textureArea));
        glActiveTexture(GL_TEXTURE0);
        texture.Bind();
        glBindVertexArray(m_spriteVao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    void Renderer2D::Clear() { glClear(GL_COLOR_BUFFER_BIT); }

    void Renderer2D::Present() { SDL_GL_SwapWindow(m_window->GetWindowHandle()); }
} // namespace Engine
