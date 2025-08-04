#ifndef INPUTCONFIG_H
#define INPUTCONFIG_H

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
