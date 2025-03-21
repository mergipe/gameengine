#include "Timer.h"
#include <SDL3/SDL.h>

namespace Engine::Timer
{
    Ticks getTicks() { return SDL_GetTicks(); }
} // namespace Engine::Timer
