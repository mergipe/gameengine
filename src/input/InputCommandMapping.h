#ifndef INPUTCOMMANDMAPPING_H
#define INPUTCOMMANDMAPPING_H

#include "InputCommand.h"
#include "InputDevice.h"
#include <unordered_map>
#include <vector>

namespace Engine
{
    class InputCommandMapping
    {
    public:
        InputCommandMapping() = default;
        explicit InputCommandMapping(const std::vector<InputCommand>& commands);
        InputCommand* findCommand(InputDevice::Type deviceType, int controlCode);

    private:
        std::unordered_map<InputDevice::Type, std::unordered_map<int, size_t>> m_mappingByDevice{};
        std::vector<InputCommand> m_commands{};
    };
} // namespace Engine

#endif
