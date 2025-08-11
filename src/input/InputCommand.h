#ifndef INPUTCOMMAND_H
#define INPUTCOMMAND_H

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
        const StringId& getName() const { return m_name; }
        std::vector<InputBinding> getBindings() const { return m_bindings; }
        Type getType() const { return m_type; }
        bool isEngineCommand() const { return m_isEngineCommand; }

    private:
        StringId m_name{};
        std::vector<InputBinding> m_bindings{};
        Type m_type{};
        bool m_isEngineCommand{false};
    };

    constexpr std::optional<InputCommand::Type> parseInputCommandType(std::string_view inputCommandTypeStr)
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
