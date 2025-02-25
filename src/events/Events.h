#ifndef EVENTS_H
#define EVENTS_H

#include "game/ECS.h"

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
    public:
        CollisionEvent(Entity entity, Entity otherEntity)
            : entity{entity}, otherEntity{otherEntity}
        {
        }
        Entity entity;
        Entity otherEntity;
    };
} // namespace Engine

#endif
