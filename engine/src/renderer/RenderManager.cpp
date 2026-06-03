#include "RenderManager.h"

#include "ShaderManager.h"
#include "core/Locator.h"
#include "core/Math.h"

#include <SDL3/SDL.h>
#include <array>
#include <cstdlib>
#include <glm/gtc/type_ptr.hpp>

namespace Engine
{
    constexpr std::string_view GetOpenGLDebugSourceName(GLenum source)
    {
        switch (source) {
        case GL_DEBUG_SOURCE_API:
            return "API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            return "WINDOW_SYSTEM";
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            return "SHADER_COMPILER";
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            return "THIRD_PARTY";
        case GL_DEBUG_SOURCE_APPLICATION:
            return "APPLICATION";
        case GL_DEBUG_SOURCE_OTHER:
            return "OTHER";
        default:
            return "UNKNOWN";
        }
    }

    constexpr std::string_view GetOpenGLDebugTypeName(GLenum type)
    {
        switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            return "ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            return "DEPRECATED_BEHAVIOR";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            return "UNDEFINED_BEHAVIOR";
        case GL_DEBUG_TYPE_PORTABILITY:
            return "PORTABILITY";
        case GL_DEBUG_TYPE_PERFORMANCE:
            return "PERFORMANCE";
        case GL_DEBUG_TYPE_MARKER:
            return "MARKER";
        case GL_DEBUG_TYPE_PUSH_GROUP:
            return "PUSH_GROUP";
        case GL_DEBUG_TYPE_POP_GROUP:
            return "POP_GROUP";
        case GL_DEBUG_TYPE_OTHER:
            return "OTHER";
        default:
            return "UNKNOWN";
        }
    }

    constexpr std::string_view GetOpenGLDebugSeverityName(GLenum severity)
    {
        switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            return "HIGH";
        case GL_DEBUG_SEVERITY_MEDIUM:
            return "MEDIUM";
        case GL_DEBUG_SEVERITY_LOW:
            return "LOW";
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            return "NOTIFICATION";
        default:
            return "UNKNOWN";
        }
    }

    void APIENTRY GLDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
                                [[maybe_unused]] GLsizei length, const GLchar* message,
                                [[maybe_unused]] const void* userParam)
    {
        auto loggerLevel{Logger::Level::warn};
        if (type == GL_DEBUG_TYPE_ERROR || type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR) {
            loggerLevel = Logger::Level::error;
        } else if (severity == GL_DEBUG_SEVERITY_LOW) {
            loggerLevel = Logger::Level::debug;
        } else if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
            loggerLevel = Logger::Level::trace;
        }
        Locator::GetLogger()->Log(loggerLevel, "[OpenGL] {}: {} | Source: {} | Type: {} | Severity: {}", id,
                                  message, GetOpenGLDebugSourceName(source), GetOpenGLDebugTypeName(type),
                                  GetOpenGLDebugSeverityName(severity));
    }

    void RenderManager::Init()
    {
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress))) {
            Locator::GetLogger()->Critical("Failed to initialize GLAD");
            std::abort();
        }
        int contextFlags{};
        glGetIntegerv(GL_CONTEXT_FLAGS, &contextFlags);
        if (contextFlags & GL_CONTEXT_FLAG_DEBUG_BIT) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(GLDebugOutput, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }
        SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        constexpr std::array spriteVertices{
            0.5f,  0.5f,  1.0f, 1.0f, // top right
            0.5f,  -0.5f, 1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, // bottom left
            -0.5f, 0.5f,  0.0f, 1.0f, // top left
        };
        constexpr std::array spriteVertexIndices{0, 1, 3, 1, 2, 3};
        m_spriteVertexArray.Create();
        VertexBuffer spriteVertexBuffer{};
        spriteVertexBuffer.Create(sizeof(spriteVertices), spriteVertices.data());
        spriteVertexBuffer.AddAttribute(VertexAttribute{0, GL_FLOAT, 2, false});
        spriteVertexBuffer.AddAttribute(VertexAttribute{1, GL_FLOAT, 2, false});
        m_spriteVertexArray.AddVertexBuffer(std::move(spriteVertexBuffer));
        VertexBuffer spriteInstanceBuffer{};
        spriteInstanceBuffer.Create(s_spritesBatchSize * sizeof(SpriteData), true);
        spriteInstanceBuffer.AddAttribute(VertexAttribute{2, GL_FLOAT, 4, false});
        spriteInstanceBuffer.AddAttribute(VertexAttribute{3, GL_FLOAT, 4, false});
        spriteInstanceBuffer.AddAttribute(VertexAttribute{4, GL_FLOAT, 4, false});
        spriteInstanceBuffer.AddAttribute(VertexAttribute{5, GL_FLOAT, 4, false});
        spriteInstanceBuffer.AddAttribute(VertexAttribute{6, GL_FLOAT, 2, false});
        spriteInstanceBuffer.AddAttribute(VertexAttribute{7, GL_FLOAT, 2, false});
        spriteInstanceBuffer.AddAttribute(VertexAttribute{8, GL_INT, 1, false});
        spriteInstanceBuffer.AddAttribute(VertexAttribute{9, GL_UNSIGNED_BYTE, 4, true});
        m_spriteVertexArray.AddVertexBuffer(std::move(spriteInstanceBuffer));
        m_spriteVertexArray.ConfigureVertexAttributes();
        ElementBuffer spriteElementBuffer{};
        spriteElementBuffer.Create(sizeof(spriteVertexIndices), spriteVertexIndices.data());
        m_spriteVertexArray.SetElementBuffer(std::move(spriteElementBuffer));

        m_renderContext.cameraUniformBuffer.Create(sizeof(glm::mat4), 0);
        m_renderContext.shaderManager = std::make_unique<ShaderManager>();
        m_renderContext.shaderManager->LoadShader(s_spriteShaderId, "sprite.vert", "sprite.frag")
            .Use()
            .BindUniformBlock("Camera", m_renderContext.cameraUniformBuffer.GetBindingPoint())
            .SetUniform("textureSampler", 0);
        m_debugRenderer = std::make_unique<DebugRenderer>(&m_renderContext);
        m_debugRenderer->Init();
        Locator::Provide(m_debugRenderer.get());
        Locator::GetLogger()->Info("Render manager initialized");
    }

    void RenderManager::ShutDown()
    {
        m_spriteVertexArray.Destroy();
        m_renderContext.cameraUniformBuffer.Destroy();
        m_debugRenderer->ShutDown();
        Locator::GetLogger()->Info("Render manager shut down");
    }

    void RenderManager::SetViewport(int x, int y, int width, int height)
    {
        glViewport(x, y, width, height);
        m_renderContext.viewportWidth = width;
        m_renderContext.viewportHeight = height;
    }

    void RenderManager::OnViewportResize(int width, int height) { SetViewport(0, 0, width, height); }

    void RenderManager::SetClearColor(float red, float green, float blue, float alpha)
    {
        glClearColor(red, green, blue, alpha);
    }

    void RenderManager::SetCamera(Camera& camera)
    {
        const auto viewProjectionMatrix{camera.GetProjectionMatrix() * camera.GetViewMatrix()};
        m_renderContext.cameraUniformBuffer.UpdateData(0, sizeof(viewProjectionMatrix),
                                                       glm::value_ptr(viewProjectionMatrix));
        m_renderContext.camera = &camera;
    }

    void RenderManager::AddSprite(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale,
                                  glm::vec2 pivotPoint, const Texture2D* texture,
                                  glm::vec2 subTextureUvTopLeft, glm::vec2 subTextureSize, RGBA8 color)
    {
        U32 textureIndex{m_spriteTextureCount};
        for (U32 i{0}; i < m_spriteTextureCount; ++i) {
            if (*m_spriteTextures[i] == *texture) {
                textureIndex = i;
            }
        }
        if (textureIndex == m_spriteTextureCount) {
            if (m_spriteTextureCount == s_textureUnitsCount) {
                // TODO: how to handle this?
            } else {
                m_spriteTextures[m_spriteTextureCount++] = texture;
            }
        }
        const float textureWidth{static_cast<float>(texture->GetWidth())};
        const float textureHeight{static_cast<float>(texture->GetHeight())};
        glm::vec3 ndcPivotPoint{pivotPoint.x - 0.5f, pivotPoint.y - 0.5f, 0.0f};
        glm::mat4 transform{Math::GetTransformMatrix(glm::vec3{position}, rotation, scale, ndcPivotPoint)};
        m_sprites.emplace_back(transform,
                               glm::vec2{subTextureUvTopLeft.x / textureWidth,
                                         1 - (subTextureUvTopLeft.y + subTextureSize.y) / textureHeight},
                               glm::vec2{subTextureSize.x / textureWidth, subTextureSize.y / textureHeight},
                               textureIndex, color);
    }

    void RenderManager::Clear() { glClear(GL_COLOR_BUFFER_BIT); }

    void RenderManager::Flush()
    {
        FlushSprites();
        m_debugRenderer->Flush();
    }

    void RenderManager::Present(const Window& window) { SDL_GL_SwapWindow(window.GetWindowHandle()); }

    void RenderManager::RenderDevGui() { m_debugRenderer->RenderDevGui(); }

    void RenderManager::FlushSprites()
    {
        int count{static_cast<int>(m_sprites.size())};
        if (count == 0) {
            return;
        }
        const auto* elementBuffer{m_spriteVertexArray.GetElementBuffer()};
        auto& shader{m_renderContext.shaderManager->GetShader(s_spriteShaderId)};
        shader.Use();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        for (U32 i{0}; i < m_spriteTextureCount; ++i) {
            m_spriteTextures[i]->Bind(i);
            shader.SetUniform("tex_sprites[" + std::to_string(i) + "]", static_cast<int>(i));
        }
        m_spriteVertexArray.Bind();
        auto* buffer{m_spriteVertexArray.GetVertexBuffer(1)};
        std::size_t offset{0};
        while (count > 0) {
            const int batchSize{std::min(count, s_spritesBatchSize)};
            buffer->UpdateData(0, static_cast<U32>(batchSize) * sizeof(SpriteData), &m_sprites[offset]);
            glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(elementBuffer->GetSize()),
                                    GL_UNSIGNED_INT, nullptr, batchSize);
            count -= s_spritesBatchSize;
            offset += s_spritesBatchSize;
        }
        m_spriteVertexArray.Unbind();
        glDisable(GL_BLEND);
        m_sprites.clear();
    }
} // namespace Engine
