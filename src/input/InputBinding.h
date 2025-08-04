#ifndef INPUTBINDING_H
#define INPUTBINDING_H

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
