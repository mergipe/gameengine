#include "InputScope.h"
#include "InputCommand.h"

namespace Engine
{
    InputCommand* InputScope::findCommand(InputDevice::Type deviceType, int controlCode)
    {
        return m_inputMapping.findCommand(deviceType, controlCode);
    }
} // namespace Engine
