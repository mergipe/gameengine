#ifndef INPUT_DEVICE_H
#define INPUT_DEVICE_H

#include "core/StringId.h"
#include "core/Types.h"

#include <optional>

namespace Engine
{
    struct InputDevice {
        enum class Type { keyboard };
        struct Id {
            bool operator==(const Id& other) const { return type == other.type && id == other.id; }
            Type type;
            U32 id;
        };
        bool operator==(const InputDevice& other) const { return id == other.id; }

        Id id{};
        bool inUse{false};
    };

    constexpr std::optional<InputDevice::Type> ParseInputDeviceType(const StringId& deviceTypeName)
    {
        using enum InputDevice::Type;
        if (deviceTypeName == SID("keyboard"))
            return keyboard;
        return {};
    }
} // namespace Engine

#endif
