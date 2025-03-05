#include "Timer.h"
#include <SDL3/SDL.h>

namespace Engine::Timer
{
    std::uint64_t getTicks() { return SDL_GetTicks(); }
} // namespace Engine::Timer
