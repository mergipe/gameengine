#include "Renderer2D.h"
#include "ShaderManager.h"
#include "Shapes.h"
#include "core/Logger.h"
#include "core/Math.h"
#include "core/StringId.h"
#include <SDL3/SDL.h>
#include <glad/glad.h>

namespace Engine
{
    glm::mat4 getModelTransformation(const Rect& rect, const glm::vec3& rotation)
    {
        return Math::getTransformationMatrix(glm::vec3{rect.getCenter(), 0.0f}, rotation,
                                             glm::vec3{rect.width, rect.height, 1.0f});
    }

    glm::mat4 getTextureTransformation(const Texture2D& texture, const Rect& textureArea)
    {
        const float widthRatio{textureArea.width / static_cast<float>(texture.getWidth())};
        const float heightRatio{textureArea.height / static_cast<float>(texture.getHeight())};
        const glm::vec2 position{textureArea.getLeftX() / static_cast<float>(texture.getWidth()),
                                 textureArea.getTopY() / static_cast<float>(texture.getHeight())};
        return Math::getTransformationMatrix(glm::vec3{position, 0.0f}, glm::vec3{0.0f},
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
        Logger::info("2D Renderer destroyed");
    }

    void Renderer2D::init()
    {
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress))) {
            Logger::critical("Failed to initialize GLAD");
            std::abort();
        }
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        setViewport(0, 0, m_window->getWidth(), m_window->getHeight());
        setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
        m_shaderManager->loadShader(SID("sprite"), "sprite.vert", "sprite.frag")
            .use()
            .setUniform("textureSampler", 0);
        m_shaderManager->loadShader(SID("primitives"), "primitives.vert", "primitives.frag").use();
        Logger::info("2D Renderer initialized");
    }

    void Renderer2D::setViewport(int x, int y, int width, int height) { glViewport(x, y, width, height); }

    void Renderer2D::setClearColor(float red, float green, float blue, float alpha)
    {
        glClearColor(red, green, blue, alpha);
    }

    void Renderer2D::setupCamera(const Camera& camera)
    {
        m_cameraTransformation = camera.getCameraTransformation();
        m_projectionTransformation = camera.getProjectionTransformation();
    }

    void Renderer2D::drawRectangle(const Rect& rect, const glm::vec4& color, const glm::vec3& rotation)
    {
        m_shaderManager->getShader(SID("primitives"))
            .use()
            .setUniform("model", getModelTransformation(rect, rotation))
            .setUniform("camera", m_cameraTransformation)
            .setUniform("projection", m_projectionTransformation)
            .setUniform("color", color);
        glBindVertexArray(m_quadVao);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        glBindVertexArray(0);
    }

    void Renderer2D::drawSprite(const Rect& spriteGeometry, const glm::vec3& rotation,
                                const Texture2D& texture, const Rect& textureArea, const glm::vec3& color)
    {
        m_shaderManager->getShader(SID("sprite"))
            .use()
            .setUniform("model", getModelTransformation(spriteGeometry, rotation))
            .setUniform("camera", m_cameraTransformation)
            .setUniform("projection", m_projectionTransformation)
            .setUniform("color", color)
            .setUniform("textureTransform", getTextureTransformation(texture, textureArea));
        glActiveTexture(GL_TEXTURE0);
        texture.bind();
        glBindVertexArray(m_spriteVao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    void Renderer2D::clear() { glClear(GL_COLOR_BUFFER_BIT); }

    void Renderer2D::present() { SDL_GL_SwapWindow(m_window->getWindowPtr()); }
} // namespace Engine
