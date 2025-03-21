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
        sol::load_result loadResult{lua.load_file(filepath)};
        if (!loadResult.valid()) {
            Logger::error("Error loading {} config file: {}", filepath.c_str(),
                          sol::error{loadResult}.what());
            return {};
        }
        loadResult();
        const sol::table configTable{lua["Config"]["Window"]};
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
