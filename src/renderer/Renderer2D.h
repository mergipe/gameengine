#ifndef RENDERER_2D_H
#define RENDERER_2D_H

#include "Camera.h"
#include "ShaderManager.h"
#include "Shapes.h"
#include "core/StringId.h"
#include "core/Window.h"
#include "resources/Texture2D.h"

#include <glm/glm.hpp>
#include <memory>

namespace Engine
{
    class Renderer2D
    {
    public:
        explicit Renderer2D(Window* window);
        Renderer2D(const Renderer2D&) = delete;
        Renderer2D(Renderer2D&&) = delete;
        Renderer2D& operator=(const Renderer2D&) = delete;
        Renderer2D& operator=(Renderer2D&&) = delete;
        ~Renderer2D();
        void Init();
        void SetViewport(int x, int y, int width, int height);
        void SetViewportSize(int width, int height);
        void SetClearColor(float red, float green, float blue, float alpha);
        void SetupCamera(const Camera& camera);
        void DrawRectangle(const Rect& rect, const glm::vec4& color, const glm::vec3& rotation);
        void DrawSprite(const Rect& spriteGeometry, const glm::vec3& rotation, const Texture2D& texture,
                        const Rect& textureArea, const glm::vec3& color = glm::vec3{1.0f});
        void Clear();
        void Present();

    private:
        glm::mat4 m_cameraTransformation{};
        glm::mat4 m_projectionTransformation{};
        StringId m_spriteShaderId{"sprite-shader"};
        StringId m_primitivesShaderId{"primitives-shader"};
        std::unique_ptr<ShaderManager> m_shaderManager{};
        Window* m_window{};
        GLuint m_spriteVao{};
        GLuint m_quadVao{};
    };
} // namespace Engine

#endif
