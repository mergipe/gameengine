#ifndef CONFIG_H
#define CONFIG_H

#include <glm/glm.hpp>
#include <string>

namespace Engine::Config
{
    struct WindowConfig {
        std::string title{"Untitled"};
        int width{800};
        int height{600};
        bool isFullscreen{false};
        bool isResizable{true};
        bool isBorderless{false};
        bool isMaximized{false};
    };

    struct VideoConfig {
        WindowConfig windowConfig;
        bool vsync{true};
    };

    struct Physics2DConfig {
        glm::vec2 gravity{0.0f, -9.81f};
        float maxLinearSpeed{400.0f};
    };

    VideoConfig loadVideoConfig();
    Physics2DConfig loadPhysics2DConfig();
} // namespace Engine::Config

#endif
