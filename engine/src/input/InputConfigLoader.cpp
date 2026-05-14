#include "InputConfigLoader.h"

#include "InputBinding.h"
#include "InputCommand.h"
#include "core/ConfigManager.h"
#include "core/Locator.h"
#include "core/Yaml.h"

#include <SDL3/SDL.h>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Engine
{
    std::vector<InputBinding> ParseInputBindings(const YAML::Node& bindingsNode)
    {
        std::vector<InputBinding> inputBindings{};
        if (bindingsNode.IsDefined()) {
            for (auto it{bindingsNode.begin()}; it != bindingsNode.end(); ++it) {
                const YAML::Node& bindingNode{*it};
                const std::string deviceTypeString{bindingNode["device"].as<std::string>()};
                const StringId typeSid{StringId::Intern(deviceTypeString)};
                const InputDevice::Type deviceType{ParseInputDeviceType(typeSid).value()};
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

    std::vector<InputCommand> ParseInputCommands(const YAML::Node& commandsNode, bool areEngineCommands)
    {
        std::vector<InputCommand> inputCommands{};
        if (commandsNode.IsDefined()) {
            for (auto it{commandsNode.begin()}; it != commandsNode.end(); ++it) {
                const YAML::Node commandNode{*it};
                const std::string name{commandNode["name"].as<std::string>()};
                const std::string typeString{commandNode["type"].as<std::string>()};
                const StringId typeSid{StringId::Intern(typeString)};
                const InputCommand::Type type{ParseInputCommandType(typeSid).value()};
                const std::vector bindings{ParseInputBindings(commandNode["bindings"])};
                inputCommands.emplace_back(StringId::Intern(name), type, bindings, areEngineCommands);
            }
        }
        return inputCommands;
    }

    std::unordered_map<StringId, InputScope> ParseInputScopes(const YAML::Node& scopesNode)
    {
        std::unordered_map<StringId, InputScope> inputScopes{};
        if (scopesNode.IsDefined()) {
            for (auto it{scopesNode.begin()}; it != scopesNode.end(); ++it) {
                const YAML::Node scopeNode{*it};
                const std::string name{scopeNode["name"].as<std::string>()};
                const std::vector commands{ParseInputCommands(scopeNode["commands"], false)};
                const StringId scopeId{StringId::Intern(name)};
                inputScopes.insert(std::make_pair(scopeId, InputScope{scopeId, commands}));
            }
        }
        return inputScopes;
    }

    InputConfig InputConfigLoader::Load(const std::filesystem::path& gameInputConfigFilePath)
    {
        // TODO: think of how to improve this InputConfig
        if (!FileSystem::IsFile(s_engineInputConfigFilePath)) {
            Locator::GetLogger()->Error("Engine input config file not found!");
            std::abort();
        }
        InputConfig inputConfig{};
        const YAML::Node engineInputRootNode{YAML::LoadFile(s_engineInputConfigFilePath)};
        inputConfig.engineInputMapping =
            InputCommandMapping{ParseInputCommands(engineInputRootNode["commands"], true)};
        if (FileSystem::IsFile(gameInputConfigFilePath)) {
            const YAML::Node gameInputRootNode{YAML::LoadFile(gameInputConfigFilePath)};
            inputConfig.inputScopes = ParseInputScopes(gameInputRootNode["scopes"]);
        }
        return inputConfig;
    }
} // namespace Engine
