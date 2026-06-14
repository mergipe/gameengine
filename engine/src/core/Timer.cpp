#include "Timer.h"

#include <SDL3/SDL.h>

namespace Engine::Timer
{
    Ticks GetMilliseconds() { return SDL_GetTicks(); }

    Ticks GetNanoseconds() { return SDL_GetTicksNS(); }
} // namespace Engine::Timer
