#include "Config.h"
#include "Filesystem.h"
#include "Logger.h"
#include <yaml-cpp/yaml.h>

namespace Engine::Config
{
    VideoConfig parseVideoConfig()
    {
        const auto filepath{Filesystem::getConfigPath() / "video.yaml"};
        const YAML::Node rootNode{YAML::LoadFile(filepath)};
        VideoConfig config{};
        if (rootNode["vsync"]) {
            config.vsync = rootNode["vsync"].as<bool>();
        }
        config.windowConfig = WindowConfig{};
        const YAML::Node windowNode{rootNode["window"]};
        if (windowNode["title"]) {
            config.windowConfig.title = windowNode["title"].as<std::string>();
        }
        if (windowNode["width"]) {
            config.windowConfig.width = windowNode["width"].as<int>();
        }
        if (windowNode["height"]) {
            config.windowConfig.height = windowNode["height"].as<int>();
        }
        if (windowNode["fullscreen"]) {
            config.windowConfig.isFullscreen = windowNode["fullscreen"].as<bool>();
        }
        if (windowNode["resizable"]) {
            config.windowConfig.isResizable = windowNode["resizable"].as<bool>();
        }
        if (windowNode["borderless"]) {
            config.windowConfig.isBorderless = windowNode["borderless"].as<bool>();
        }
        if (windowNode["maximized"]) {
            config.windowConfig.isMaximized = windowNode["maximized"].as<bool>();
        }
        return config;
    }

    Physics2DConfig parsePhysics2DConfig()
    {
        const auto filepath{Filesystem::getConfigPath() / "physics2d.yaml"};
        const YAML::Node rootNode{YAML::LoadFile(filepath)};
        Physics2DConfig config{};
        if (rootNode["gravity"]) {
            const YAML::Node gravityNode{rootNode["gravity"]};
            if (gravityNode["x"]) {
                config.gravity.x = gravityNode["x"].as<float>();
            }
            if (gravityNode["y"]) {
                config.gravity.y = gravityNode["y"].as<float>();
            }
        }
        if (rootNode["max_linear_speed"]) {
            config.maxLinearSpeed = rootNode["max_linear_speed"].as<float>();
        }
        return config;
    }
} // namespace Engine::Config
