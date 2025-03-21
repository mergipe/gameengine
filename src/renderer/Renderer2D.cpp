#include "Renderer2D.h"
#include "core/Filesystem.h"
#include "core/Logger.h"
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
    glm::mat4 getModelMatrix(const glm::vec2& position, float width, float height, float rotation)
    {
        glm::mat4 model{1.0f};
        model = glm::translate(model, glm::vec3{position, 0.0f});
        model = glm::translate(model, glm::vec3{0.5f * width, 0.5f * height, 0.0f});
        model = glm::rotate(model, glm::radians(rotation), glm::vec3{0.0f, 0.0f, 1.0f});
        model = glm::translate(model, glm::vec3{-0.5f * width, -0.5f * height, 0.0f});
        model = glm::scale(model, glm::vec3{width, height, 1.0f});
        return model;
    }

    glm::mat4 getTextureTransform(const Texture2D& texture, const Rect& textureArea)
    {
        float widthRatio{textureArea.width / static_cast<float>(texture.getWidth())};
        float heightRatio{textureArea.height / static_cast<float>(texture.getHeight())};
        glm::vec2 translate{textureArea.position.x / static_cast<float>(texture.getWidth()),
                            textureArea.position.y / static_cast<float>(texture.getHeight())};
        glm::mat4 transform{1.0f};
        transform = glm::translate(transform, glm::vec3{translate, 0.0f});
        transform = glm::scale(transform, glm::vec3(widthRatio, heightRatio, 1.0f));
        return transform;
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
        if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
            Logger::critical("Failed to initialize GLAD");
            std::abort();
        }
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        setViewport(0, 0, m_window->getWidth(), m_window->getHeight());
        setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        std::array<GLfloat, 24> spriteVertexData{0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                                                 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                                                 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f};
        glGenVertexArrays(1, &m_spriteVao);
        GLuint vbo{};
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(spriteVertexData), spriteVertexData.data(), GL_STATIC_DRAW);
        glBindVertexArray(m_spriteVao);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*) 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));
        std::array<GLfloat, 8> quadVertices{0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f};
        glGenVertexArrays(1, &m_quadVao);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices.data(), GL_STATIC_DRAW);
        glBindVertexArray(m_quadVao);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*) 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glm::mat4 projection{glm::ortho(0.0f, static_cast<float>(m_window->getWidth()),
                                        static_cast<float>(m_window->getHeight()), 0.0f, -1.0f, 1.0f)};
        m_shaderManager = std::make_unique<ShaderManager>();
        m_shaderManager->loadShader("sprite", "sprite.vert", "sprite.frag")
            .use()
            .setUniform("textureSampler", 0)
            .setUniform("projection", projection);
        m_shaderManager->loadShader("primitives", "primitives.vert", "primitives.frag")
            .use()
            .setUniform("projection", projection);
        Logger::info("2D Renderer initialized");
    }

    void Renderer2D::setViewport(int x, int y, int width, int height) { glViewport(x, y, width, height); }

    void Renderer2D::setClearColor(float red, float green, float blue, float alpha)
    {
        glClearColor(red, green, blue, alpha);
    }

    void Renderer2D::drawRectangle(const Rect& rect, const glm::vec4& color, float rotation)
    {
        m_shaderManager->getShader("primitives")
            .use()
            .setUniform("model", getModelMatrix(rect.position, rect.width, rect.height, rotation))
            .setUniform("color", color);
        glBindVertexArray(m_quadVao);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        glBindVertexArray(0);
    }

    void Renderer2D::drawSprite(const glm::vec2& position, float width, float height, float rotation,
                                const Texture2D& texture, const Rect& textureArea, const glm::vec3& color)
    {
        m_shaderManager->getShader("sprite")
            .use()
            .setUniform("model", getModelMatrix(position, width, height, rotation))
            .setUniform("color", color)
            .setUniform("textureTransform", getTextureTransform(texture, textureArea));
        glActiveTexture(GL_TEXTURE0);
        texture.bind();
        glBindVertexArray(m_spriteVao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    void Renderer2D::clear() { glClear(GL_COLOR_BUFFER_BIT); }

    void Renderer2D::present() { SDL_GL_SwapWindow(m_window->getWindowPtr()); }
} // namespace Engine
