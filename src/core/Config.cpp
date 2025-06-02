#include "Config.h"
#include "Filesystem.h"
#include "Logger.h"
#include <yaml-cpp/yaml.h>

namespace Engine::Config
{
    VideoConfig loadVideoConfig()
    {
        const auto filepath{Filesystem::getConfigPath() / "video.yaml"};
        const YAML::Node rootNode{YAML::LoadFile(filepath)};
        WindowConfig windowConfig{};
        const YAML::Node windowNode{rootNode["window"]};
        if (windowNode["title"]) {
            windowConfig.title = windowNode["title"].as<std::string>();
        }
        if (windowNode["width"]) {
            windowConfig.width = windowNode["width"].as<int>();
        }
        if (windowNode["height"]) {
            windowConfig.height = windowNode["height"].as<int>();
        }
        if (windowNode["fullscreen"]) {
            windowConfig.isFullscreen = windowNode["fullscreen"].as<bool>();
        }
        if (windowNode["resizable"]) {
            windowConfig.isResizable = windowNode["resizable"].as<bool>();
        }
        if (windowNode["borderless"]) {
            windowConfig.isBorderless = windowNode["borderless"].as<bool>();
        }
        if (windowNode["maximized"]) {
            windowConfig.isMaximized = windowNode["maximized"].as<bool>();
        }
        return VideoConfig{windowConfig};
    }
} // namespace Engine::Config
