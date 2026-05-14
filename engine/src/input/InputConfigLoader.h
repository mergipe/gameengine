#ifndef INPUT_CONFIG_LOADER_H
#define INPUT_CONFIG_LOADER_H

#include "InputConfig.h"
#include "core/ConfigManager.h"

#include <filesystem>

namespace Engine
{
    class InputConfigLoader
    {
    public:
        InputConfigLoader() = delete;
        static InputConfig Load(const std::filesystem::path& gameInputConfigFilePath);

    private:
        static const inline std::filesystem::path s_engineInputConfigFilePath{
            ConfigManager::GetEngineConfigPath() / "input.config"};
    };
} // namespace Engine

#endif
