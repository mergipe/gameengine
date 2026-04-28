#ifndef INPUT_CONFIG_H
#define INPUT_CONFIG_H

#include "InputScope.h"

#include <unordered_map>

namespace Engine
{
    struct InputConfig {
        InputCommandMapping engineInputMapping{};
        std::unordered_map<StringId, InputScope> inputScopes{};
    };
} // namespace Engine

#endif // INPUTCONFIG_H
