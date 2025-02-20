#ifndef EVENTS_H
#define EVENTS_H

#include "game/ECS.h"
#include <functional>
#include <list>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <utility>

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

    class IEventCallback
    {
    public:
        IEventCallback(const IEventCallback&) = delete;
        IEventCallback(IEventCallback&&) = delete;
        IEventCallback& operator=(const IEventCallback&) = delete;
        IEventCallback& operator=(IEventCallback&&) = delete;
        virtual ~IEventCallback() = default;
        void execute(Event& e) { call(e); }

    private:
        virtual void call(Event& e) = 0;
    };

    template <typename TOwner, typename TEvent>
    class EventCallback final : public IEventCallback
    {
    public:
        using CallbackFunction = std::function<void(const TOwner&, TEvent&)>;
        EventCallback(TOwner* ownerInstance, const CallbackFunction& callbackFunction)
            : m_callbackFunction{callbackFunction}, m_ownerInstance{ownerInstance}
        {
        }

    private:
        void call(Event& e) override;
        CallbackFunction m_callbackFunction{};
        TOwner* m_ownerInstance{};
    };

    template <typename TOwner, typename TEvent>
    void EventCallback<TOwner, TEvent>::call(Event& e)
    {
        std::invoke(m_callbackFunction, m_ownerInstance, static_cast<TEvent&>(e));
    }

    class EventBus final
    {
    public:
        using HandlerList = std::list<std::unique_ptr<IEventCallback>>;
        template <typename TOwner, typename TEvent>
        void subscribeToEvent(TOwner* ownerInstance,
                              const std::function<void(const TOwner&, TEvent&)>& callbackFunction);
        template <typename TEvent, typename... TArgs> void dispatchEvent(TArgs&&... args);

    private:
        std::unordered_map<std::type_index, std::unique_ptr<HandlerList>> m_subscribers;
    };

    template <typename TOwner, typename TEvent>
    void EventBus::subscribeToEvent(TOwner* ownerInstance,
                                    const std::function<void(const TOwner&, TEvent&)>& callbackFunction)
    {
        if (!m_subscribers[typeid(TEvent)].get()) {
            m_subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
        }
        const auto subscriber{
            std::make_unique<EventCallback<TOwner, TEvent>>(ownerInstance, callbackFunction)};
        m_subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
    }

    template <typename TEvent, typename... TArgs>
    void EventBus::dispatchEvent(TArgs&&... args)
    {
        const auto handlers{m_subscribers[typeid(TEvent)].get()};
        const TEvent event{std::forward<TArgs>(args)...};
        if (handlers) {
            for (auto it{handlers->begin()}; it != handlers->end(); ++it) {
                const auto handler{it->get()};
                handler->execute(event);
            }
        }
    }

    class CollisionEvent final : public Event
    {
    public:
        CollisionEvent(Entity entity, Entity otherEntity)
            : m_entity{entity}, m_otherEntity{otherEntity}
        {
        }
        Entity m_entity;
        Entity m_otherEntity;
    };
} // namespace Engine

#endif
