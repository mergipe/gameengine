#ifndef INPUTCONFIGLOADER_H
#define INPUTCONFIGLOADER_H

#include "InputConfig.h"
#include <filesystem>

namespace Engine
{
    class InputConfigLoader
    {
    public:
        InputConfigLoader() = delete;
        static InputConfig load(const std::filesystem::path& gameInputConfigFilepath);
    };
} // namespace Engine

#endif
