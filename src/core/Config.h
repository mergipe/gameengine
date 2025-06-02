#ifndef CONFIG_H
#define CONFIG_H

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
    };

    VideoConfig loadVideoConfig();
} // namespace Engine::Config

#endif
