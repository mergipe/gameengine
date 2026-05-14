#ifndef TIME_H
#define TIME_H

#include "Types.h"

namespace Engine::Timer
{
    using Ticks = U64;

    Ticks GetTicks();
} // namespace Engine::Timer

#endif
