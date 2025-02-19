#include "Timer.h"
#include <SDL_timer.h>

namespace Engine::Timer
{
    std::uint64_t getTicks() { return SDL_GetTicks64(); }
} // namespace Engine::Timer
