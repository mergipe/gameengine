#ifndef INPUTDEVICE_H
#define INPUTDEVICE_H

#include <cstdint>
#include <optional>
#include <string_view>

namespace Engine
{
    struct InputDevice {
        enum class Type { keyboard };
        struct Id {
            bool operator==(const Id& other) const { return type == other.type && id == other.id; }
            Type type;
            std::uint32_t id;
        };
        bool operator==(const InputDevice& other) const { return id == other.id; }

        Id id{};
        bool inUse{false};
    };

    constexpr std::optional<InputDevice::Type> getInputDeviceType(std::string_view inputDeviceTypeStr)
    {
        using enum InputDevice::Type;
        if (inputDeviceTypeStr == "keyboard")
            return keyboard;
        return {};
    }
} // namespace Engine

#endif
