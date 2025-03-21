#ifndef CONFIG_H
#define CONFIG_H

#include "Filesystem.h"
#include "Logger.h"
#include "Window.h"
#include <sol/sol.hpp>

namespace Engine::Config
{
    WindowConfig loadWindowConfig();
} // namespace Engine::Config

#endif
