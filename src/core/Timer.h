#ifndef TIME_H
#define TIME_H

#include <cstdint>

namespace Engine::Timer
{
    using Ticks = std::uint64_t;
    Ticks GetTicks();
} // namespace Engine::Timer

#endif
