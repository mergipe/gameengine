#pragma once

#include "InputScope.h"

#include <unordered_map>

namespace Engine
{
    struct InputConfig {
        InputCommandMapping engineInputMapping{};
        std::unordered_map<StringId, InputScope> inputScopes{};
    };
} // namespace Engine
