#ifndef INPUT_BINDING_H
#define INPUT_BINDING_H

#include "InputDevice.h"

#include <string>

namespace Engine
{
    struct InputBinding {
        std::string controlName{};
        int controlCode{};
        InputDevice::Type deviceType{};
    };
} // namespace Engine

#endif
