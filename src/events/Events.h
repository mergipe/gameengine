#ifndef EVENTS_H
#define EVENTS_H

#include "game/ECS.h"
#include <SDL_keycode.h>

namespace Engine
{
    class Event
    {
    public:
        Event(const Event&) = delete;
        Event(Event&&) = delete;
        Event& operator=(const Event&) = delete;
        Event& operator=(Event&&) = delete;
        virtual ~Event() = default;

    protected:
        Event() = default;
    };

    struct CollisionEvent final : public Event {
        CollisionEvent(Entity entity, Entity otherEntity)
            : entity{entity}, otherEntity{otherEntity}
        {
        }
        Entity entity;
        Entity otherEntity;
    };

    struct KeyPressedEvent final : public Event {
        KeyPressedEvent(SDL_Keycode keyCode)
            : keyCode{keyCode}
        {
        }
        SDL_Keycode keyCode;
    };
} // namespace Engine

#endif
