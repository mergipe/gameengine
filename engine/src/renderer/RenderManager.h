#pragma once

#include "Camera.h"
#include "DebugRenderer.h"
#include "RenderContext.h"
#include "core/StringId.h"
#include "core/Window.h"
#include "resources/Texture2D.h"

#include <glm/glm.hpp>
#include <memory>

namespace Engine
{
    struct SpriteData {
        glm::mat4 transform{};
        glm::vec2 subTextureNormalizedUv{};
        glm::vec2 subTextureNormalizedSize{};
        int textureIndex{};
        RGBA8 color{};
    };

    class RenderManager
    {
    public:
        void Init();
        void ShutDown();
        void SetViewport(int x, int y, int width, int height);
        void OnViewportResize(int width, int height);
        void SetClearColor(float red, float green, float blue, float alpha);
        void SetCamera(Camera& camera);
        void AddSprite(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec2 pivotPoint,
                       const Texture2D* texture, glm::vec2 subTextureUvTopLeft, glm::vec2 subTextureSize,
                       RGBA8 color);
        void Clear();
        void Flush();
        void Present(const Window& window);
        void RenderDevGui();

    private:
        void FlushSprites();

        constexpr static StringId s_spriteShaderId{SID("sprite-shader")};
        constexpr static int s_spritesBatchSize{1024};
        constexpr static U32 s_textureUnitsCount{16};

        std::array<const Texture2D*, s_textureUnitsCount> m_spriteTextures{};
        RenderContext m_renderContext{};
        VertexArray m_spriteVertexArray{};
        std::vector<SpriteData> m_sprites{};
        std::unique_ptr<DebugRenderer> m_debugRenderer{};
        U32 m_spriteTextureCount{0};
    };
} // namespace Engine
