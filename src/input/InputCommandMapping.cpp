#include "InputCommandMapping.h"

namespace Engine
{
    InputCommandMapping::InputCommandMapping(const std::vector<InputCommand>& commands)
        : m_commands{commands}
    {
        for (size_t i{0}; i < m_commands.size(); ++i) {
            for (const auto& binding : m_commands[i].getBindings()) {
                m_mappingByDevice[binding.deviceType][binding.controlCode] = i;
            }
        }
    }

    InputCommand* InputCommandMapping::findCommand(InputDevice::Type deviceType, int controlCode)
    {
        InputCommand* command{nullptr};
        const auto mappingIt{m_mappingByDevice.find(deviceType)};
        if (mappingIt != m_mappingByDevice.end()) {
            const auto commandIt{mappingIt->second.find(controlCode)};
            if (commandIt != mappingIt->second.end()) {
                command = &m_commands[commandIt->second];
            }
        }
        return command;
    }
} // namespace Engine
