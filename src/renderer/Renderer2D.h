#ifndef RENDERER2D_H
#define RENDERER2D_H

#include "Shader.h"
#include "ShaderManager.h"
#include "Shapes.h"
#include "core/Window.h"
#include "resources/Texture2D.h"
#include <SDL3/SDL.h>
#include <glm/glm.hpp>

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
        void init();
        void setViewport(int x, int y, int width, int height);
        void setClearColor(float red, float green, float blue, float alpha);
        void drawRectangle(const Rect& rect, const glm::vec4& color, float rotation);
        void drawSprite(const glm::vec2& position, float width, float height, float rotation,
                        const Texture2D& texture, const Rect& textureArea,
                        const glm::vec3& color = glm::vec3{1.0f});
        void clear();
        void present();

    private:
        std::unique_ptr<ShaderManager> m_shaderManager{};
        Window* m_window{};
        GLuint m_spriteVao{};
        GLuint m_quadVao{};
    };
} // namespace Engine

#endif
