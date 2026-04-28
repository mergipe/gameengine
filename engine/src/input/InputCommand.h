#ifndef INPUT_COMMAND_H
#define INPUT_COMMAND_H

#include "InputBinding.h"
#include "core/StringId.h"

#include <optional>
#include <string_view>
#include <vector>

namespace Engine
{
    class InputCommand
    {
    public:
        enum class Type { control_down, control_state };

        InputCommand(const StringId& name, Type type, const std::vector<InputBinding>& bindings,
                     bool isEngineCommand)
            : m_name{name}, m_bindings{bindings}, m_type{type}, m_isEngineCommand{isEngineCommand}
        {
        }
        const StringId& GetName() const { return m_name; }
        std::vector<InputBinding> GetBindings() const { return m_bindings; }
        Type GetType() const { return m_type; }
        bool IsEngineCommand() const { return m_isEngineCommand; }

    private:
        StringId m_name{};
        std::vector<InputBinding> m_bindings{};
        Type m_type{};
        bool m_isEngineCommand{false};
    };

    constexpr std::optional<InputCommand::Type> ParseInputCommandType(std::string_view inputCommandTypeStr)
    {
        using enum InputCommand::Type;
        if (inputCommandTypeStr == "control_down")
            return control_down;
        if (inputCommandTypeStr == "control_state")
            return control_state;
        return {};
    }
} // namespace Engine

#endif
