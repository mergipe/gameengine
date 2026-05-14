#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include "FileSystem.h"
#include "StringId.h"
#include "resources/ResourceManager.h"

#include <filesystem>
#include <glm/glm.hpp>
#include <string>

namespace Engine
{
    struct WindowConfig {
        std::string title{"Game Engine"};
        int width{800};
        int height{600};
        bool isFullscreen{false};
        bool isResizable{true};
        bool isBorderless{false};
        bool isMaximized{false};
    };

    struct VideoConfig {
        WindowConfig windowConfig{};
        bool vsync{true};
    };

    struct Physics2DConfig {
        glm::vec2 gravity{0.0f, -9.81f};
        float maxLinearSpeed{400.0f};
    };

    struct GameConfig {
        StringId initialScene{};
    };

    class ConfigManager
    {
    public:
        ConfigManager() = delete;
        static void Init();
        static const VideoConfig& GetVideoConfig() { return s_videoConfig; }
        static const Physics2DConfig& GetPhysics2DConfig() { return s_physics2DConfig; }
        static const GameConfig& GetGameConfig() { return s_gameConfig; }
        static const std::filesystem::path& GetEngineConfigPath() { return s_engineConfigPath; }

    private:
        static void ParseVideoConfig();
        static void ParsePhysics2DConfig();
        static void ParseGameConfig();
        static inline VideoConfig s_videoConfig{};
        static inline Physics2DConfig s_physics2DConfig{};
        static inline GameConfig s_gameConfig{};
        static inline const std::filesystem::path s_engineConfigPath{FileSystem::GetAbsolutePath("config")};
        static inline const std::filesystem::path s_videoConfigPath{
            ResourceManager::GetResourcePath("video.config")};
        static inline const std::filesystem::path s_physics2DConfigPath{
            ResourceManager::GetResourcePath("physics2d.config")};
        static inline const std::filesystem::path s_gameConfigPath{
            ResourceManager::GetResourcePath("game.config")};
    };
} // namespace Engine

#endif
