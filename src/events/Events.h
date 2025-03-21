#ifndef EVENTS_H
#define EVENTS_H

#include <SDL3/SDL.h>
#include <entt/entt.hpp>
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
        CollisionEvent(const entt::entity& entity, const entt::entity& otherEntity)
            : entity{entity}, otherEntity{otherEntity}
        {
        }
        entt::entity entity;
        entt::entity otherEntity;
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
