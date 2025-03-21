#ifndef CONFIG_H
#define CONFIG_H

#include "Window.h"
#include <sol/sol.hpp>

namespace Engine::Config
{
    WindowConfig loadWindowConfig();
} // namespace Engine::Config

#endif
