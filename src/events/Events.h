#ifndef EVENTS_H
#define EVENTS_H

#include "core/StringId.h"
#include "input/InputDevice.h"
#include "input/InputValue.h"
#include <entt/entt.hpp>

namespace Engine
{
    class Event
    {
    public:
        virtual ~Event() = default;

    protected:
        Event() = default;
    };

    struct CollisionEvent final : public Event {
        CollisionEvent(const entt::entity& entity, const entt::entity& otherEntity)
            : entity{entity}, otherEntity{otherEntity}
        {
        }
        entt::entity entity;
        entt::entity otherEntity;
    };

    struct InputEvent final : public Event {
        InputEvent() = default;
        InputEvent(const StringId& commandId, const InputDevice::Id& inputDeviceId,
                   const InputValue& inputValue, bool isEngineCommand)
            : commandId{commandId}
            , inputDeviceId{inputDeviceId}
            , inputValue{inputValue}
            , isEngineCommand{isEngineCommand}
        {
        }
        StringId commandId{};
        InputDevice::Id inputDeviceId{};
        InputValue inputValue{};
        bool isEngineCommand{false};
    };
} // namespace Engine

#endif
