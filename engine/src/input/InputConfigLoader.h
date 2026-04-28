#ifndef INPUT_CONFIG_LOADER_H
#define INPUT_CONFIG_LOADER_H

#include "InputConfig.h"

#include <filesystem>

namespace Engine
{
    class InputConfigLoader
    {
    public:
        InputConfigLoader() = delete;
        static InputConfig Load(const std::filesystem::path& gameInputConfigFilepath);
    };
} // namespace Engine

#endif
