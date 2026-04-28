#include "Timer.h"

#include <SDL3/SDL.h>

namespace Engine::Timer
{
    Ticks GetTicks() { return SDL_GetTicks(); }
} // namespace Engine::Timer
