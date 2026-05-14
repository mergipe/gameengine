#ifndef INPUT_COMMAND_H
#define INPUT_COMMAND_H

#include "InputBinding.h"
#include "core/StringId.h"

#include <optional>
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
        [[nodiscard]] const StringId& GetName() const { return m_name; }
        [[nodiscard]] std::vector<InputBinding> GetBindings() const { return m_bindings; }
        [[nodiscard]] Type GetType() const { return m_type; }
        [[nodiscard]] bool IsEngineCommand() const { return m_isEngineCommand; }

    private:
        StringId m_name{};
        std::vector<InputBinding> m_bindings{};
        Type m_type{};
        bool m_isEngineCommand{false};
    };

    constexpr std::optional<InputCommand::Type> ParseInputCommandType(const StringId& commandTypeName)
    {
        using enum InputCommand::Type;
        if (commandTypeName == SID("control_down"))
            return control_down;
        if (commandTypeName == SID("control_state"))
            return control_state;
        return {};
    }
} // namespace Engine

#endif
