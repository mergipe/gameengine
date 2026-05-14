#include "ConfigManager.h"

#include "Locator.h"
#include "Yaml.h"

namespace Engine
{
    void ConfigManager::ParseVideoConfig()
    {
        if (!FileSystem::IsFile(s_videoConfigPath)) {
            return;
        }
        const YAML::Node rootNode{YAML::LoadFile(s_videoConfigPath)};
        if (rootNode["vsync"]) {
            s_videoConfig.vsync = rootNode["vsync"].as<bool>();
        }
        if (const YAML::Node windowNode{rootNode["window"]}) {
            if (windowNode["title"]) {
                s_videoConfig.windowConfig.title = windowNode["title"].as<std::string>();
            }
            if (windowNode["width"]) {
                s_videoConfig.windowConfig.width = windowNode["width"].as<int>();
            }
            if (windowNode["height"]) {
                s_videoConfig.windowConfig.height = windowNode["height"].as<int>();
            }
            if (windowNode["fullscreen"]) {
                s_videoConfig.windowConfig.isFullscreen = windowNode["fullscreen"].as<bool>();
            }
            if (windowNode["resizable"]) {
                s_videoConfig.windowConfig.isResizable = windowNode["resizable"].as<bool>();
            }
            if (windowNode["borderless"]) {
                s_videoConfig.windowConfig.isBorderless = windowNode["borderless"].as<bool>();
            }
            if (windowNode["maximized"]) {
                s_videoConfig.windowConfig.isMaximized = windowNode["maximized"].as<bool>();
            }
        }
    }

    void ConfigManager::ParsePhysics2DConfig()
    {
        if (!FileSystem::IsFile(s_physics2DConfigPath)) {
            return;
        }
        const YAML::Node rootNode{YAML::LoadFile(s_physics2DConfigPath)};
        if (const YAML::Node gravityNode{rootNode["gravity"]}) {
            if (gravityNode["x"]) {
                s_physics2DConfig.gravity.x = gravityNode["x"].as<float>();
            }
            if (gravityNode["y"]) {
                s_physics2DConfig.gravity.y = gravityNode["y"].as<float>();
            }
        }
        if (rootNode["max_linear_speed"]) {
            s_physics2DConfig.maxLinearSpeed = rootNode["max_linear_speed"].as<float>();
        }
    }

    void ConfigManager::ParseGameConfig()
    {
        if (!FileSystem::IsFile(s_gameConfigPath)) {
            return;
        }
        const YAML::Node rootNode{YAML::LoadFile(s_gameConfigPath)};
        if (rootNode["initial_scene"]) {
            s_gameConfig.initialScene = StringId::Intern(rootNode["initial_scene"].as<std::string>());
        }
    }

    void ConfigManager::Init()
    {
        ParseVideoConfig();
        ParsePhysics2DConfig();
        ParseGameConfig();
    }
} // namespace Engine
