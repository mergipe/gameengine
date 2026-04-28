#include "InputScope.h"

#include "InputCommand.h"

namespace Engine
{
    InputCommand* InputScope::FindCommand(InputDevice::Type deviceType, int controlCode)
    {
        return m_inputMapping.FindCommand(deviceType, controlCode);
    }
} // namespace Engine
