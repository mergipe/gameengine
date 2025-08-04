#include "InputConfigLoader.h"
#include "InputBinding.h"
#include "InputCommand.h"
#include "core/Filesystem.h"
#include <SDL3/SDL.h>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <yaml-cpp/yaml.h>

namespace Engine
{
    std::vector<InputBinding> parseInputBindings(const YAML::Node& bindingsNode)
    {
        std::vector<InputBinding> inputBindings{};
        if (bindingsNode.IsDefined()) {
            for (YAML::const_iterator it{bindingsNode.begin()}; it != bindingsNode.end(); ++it) {
                const YAML::Node& bindingNode{*it};
                const std::string deviceTypeString{bindingNode["device"].as<std::string>()};
                const InputDevice::Type deviceType{getInputDeviceType(deviceTypeString).value()};
                const std::string controlName{bindingNode["control"].as<std::string>()};
                int controlCode{};
                switch (deviceType) {
                case InputDevice::Type::keyboard:
                    controlCode = SDL_GetScancodeFromName(controlName.c_str());
                    break;
                }
                inputBindings.emplace_back(controlName, controlCode, deviceType);
            }
        }
        return inputBindings;
    }

    std::vector<InputCommand> parseInputCommands(const YAML::Node& commandsNode, bool areEngineCommands)
    {
        std::vector<InputCommand> inputCommands{};
        if (commandsNode.IsDefined()) {
            for (YAML::const_iterator it{commandsNode.begin()}; it != commandsNode.end(); ++it) {
                const YAML::Node commandNode{*it};
                const std::string name{commandNode["name"].as<std::string>()};
                const std::string typeString{commandNode["type"].as<std::string>()};
                const InputCommand::Type type{getInputCommandType(typeString).value()};
                const std::vector<InputBinding> bindings{parseInputBindings(commandNode["bindings"])};
                inputCommands.emplace_back(StringId{name}, type, bindings, areEngineCommands);
            }
        }
        return inputCommands;
    }

    std::unordered_map<StringId, InputScope> parseInputScopes(const YAML::Node& scopesNode)
    {
        std::unordered_map<StringId, InputScope> inputScopes{};
        if (scopesNode.IsDefined()) {
            for (YAML::const_iterator it{scopesNode.begin()}; it != scopesNode.end(); ++it) {
                const YAML::Node scopeNode{*it};
                const std::string name{scopeNode["name"].as<std::string>()};
                const std::vector<InputCommand> commands{parseInputCommands(scopeNode["commands"], false)};
                const StringId scopeId{name};
                inputScopes.insert(std::make_pair(scopeId, InputScope{scopeId, commands}));
            }
        }
        return inputScopes;
    }

    InputConfig InputConfigLoader::load(const std::filesystem::path& gameInputConfigFilepath)
    {
        const auto engineInputConfigFilepath{Filesystem::getConfigPath() / "engine_input.yaml"};
        const YAML::Node engineInputRootNode{YAML::LoadFile(engineInputConfigFilepath)};
        const auto engineInputMapping{
            InputCommandMapping{parseInputCommands(engineInputRootNode["commands"], true)}};
        const YAML::Node gameInputRootNode{YAML::LoadFile(gameInputConfigFilepath)};
        const auto inputScopes{parseInputScopes(gameInputRootNode["scopes"])};
        return InputConfig{engineInputMapping, inputScopes};
    }
} // namespace Engine
