#ifndef EVENTS_H
#define EVENTS_H

#include "game/ECS.h"
#include <SDL_keycode.h>
#include <glm/glm.hpp>

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
        CollisionEvent(const Entity& entity, const Entity& otherEntity)
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

    struct ProjectileEmitEvent final : public Event {
        ProjectileEmitEvent(const Entity& entity, glm::vec2 velocity, int duration)
            : entity{entity}, velocity{velocity}, duration{duration}
        {
        }
        Entity entity;
        glm::vec2 velocity;
        int duration;
    };
} // namespace Engine

#endif
