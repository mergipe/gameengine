#include "Config.h"
#include "Filesystem.h"
#include "Logger.h"
#include <sol/sol.hpp>

namespace Engine::Config
{
    WindowConfig loadWindowConfig()
    {
        sol::state lua{};
        lua.open_libraries(sol::lib::base);
        const auto filepath{Filesystem::getConfigPath() / "main.lua"};
        const auto result{lua.script_file(filepath, sol::script_pass_on_error)};
        if (!result.valid()) {
            const sol::error error{result};
            const sol::call_status status{result.status()};
            Logger::error("Error loading config file ({}): {} error\n\t{}", filepath.c_str(),
                          sol::to_string(status), sol::error{result}.what());
            return {};
        }
        const sol::optional<sol::table> maybeWindowConfig{lua["Config"]["Window"]};
        if (!maybeWindowConfig) {
            return {};
        }
        const sol::table& windowConfig{maybeWindowConfig.value()};
        WindowConfig config{};
        config.title = windowConfig["title"].get_or(std::string{"Untitled"});
        config.width = windowConfig["width"].get_or(800);
        config.height = windowConfig["height"].get_or(600);
        config.isFullscreen = windowConfig["fullscreen"].get_or(false);
        config.isResizable = windowConfig["resizable"].get_or(true);
        config.isBorderless = windowConfig["borderless"].get_or(false);
        config.isMaximized = windowConfig["maximized"].get_or(false);
        return config;
    }

} // namespace Engine::Config
