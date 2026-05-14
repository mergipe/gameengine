#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

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
    class RenderManager
    {
    public:
        explicit RenderManager(Window* window);
        void Init();
        void ShutDown();
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
        StringId m_spriteShaderId{SID("sprite-shader")};
        StringId m_primitivesShaderId{SID("primitives-shader")};
        std::unique_ptr<ShaderManager> m_shaderManager{};
        Window* m_window{};
        GLuint m_spriteVao{};
        GLuint m_quadVao{};
    };
} // namespace Engine

#endif
