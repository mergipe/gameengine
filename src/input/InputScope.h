#ifndef INPUTSCOPE_H
#define INPUTSCOPE_H

#include "InputCommand.h"
#include "InputCommandMapping.h"
#include "InputDevice.h"
#include "core/StringId.h"
#include <vector>

namespace Engine
{
    class InputScope
    {
    public:
        explicit InputScope(const StringId& id)
            : m_id{id}
        {
        }
        InputScope(const StringId& id, const std::vector<InputCommand>& commands)
            : m_inputMapping{commands}, m_id{id}
        {
        }
        const StringId& getId() const { return m_id; }
        InputCommand* findCommand(InputDevice::Type deviceType, int controlCode);

    private:
        InputCommandMapping m_inputMapping{};
        StringId m_id{};
    };
} // namespace Engine

#endif
