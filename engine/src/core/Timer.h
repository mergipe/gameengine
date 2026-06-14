#pragma once

#include "Types.h"

namespace Engine::Timer
{
    using Ticks = U64;

    Ticks GetMilliseconds();
    Ticks GetNanoseconds();
} // namespace Engine::Timer
