#include "Config.h"
#include "Filesystem.h"
#include "Logger.h"

namespace Engine::Config
{
    WindowConfig loadWindowConfig()
    {
        sol::state lua{};
        lua.open_libraries(sol::lib::base);
        auto filepath{Filesystem::getConfigPath() / "main.lua"};
        auto result{lua.script_file(filepath, sol::script_pass_on_error)};
        if (!result.valid()) {
            sol::error error{result};
            sol::call_status status{result.status()};
            Logger::error("Error loading config file ({}): {} error\n\t{}", filepath.c_str(),
                          sol::to_string(status), sol::error{result}.what());
            return {};
        }
        const auto configTable{lua["Config"]["Window"]};
        if (!configTable.valid()) {
            Logger::error("Error parsing window config from {}", filepath.c_str());
            return {};
        }
        WindowConfig config{};
        config.title = configTable["title"];
        config.width = configTable["width"];
        config.height = configTable["height"];
        config.isFullscreen = configTable["fullscreen"];
        config.isResizable = configTable["resizable"];
        config.isBorderless = configTable["borderless"];
        return config;
    }

} // namespace Engine::Config
