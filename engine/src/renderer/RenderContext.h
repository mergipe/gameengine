#pragma once

#include "Buffer.h"
#include "Camera.h"
#include "ShaderManager.h"

namespace Engine
{
    struct RenderContext {
        UniformBuffer cameraUniformBuffer{};
        std::unique_ptr<ShaderManager> shaderManager{};
        const Camera* camera{};
        int viewportWidth{};
        int viewportHeight{};
    };
} // namespace Engine
