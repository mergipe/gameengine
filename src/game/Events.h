#ifndef EVENTS_H
#define EVENTS_H

#include "ECS.h"
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
        Event() = default;
    };

    class IEventCallback
    {
    private:
        virtual void call(Event& e) = 0;

    public:
        virtual ~IEventCallback() = default;
        void execute(Event& e) { call(e); }
    };

    template <typename TOwner, typename TEvent>
    class EventCallback : public IEventCallback
    {
    private:
        using CallbackFunction = std::function<void(const TOwner&, TEvent&)>;
        TOwner* m_ownerInstance{};
        CallbackFunction m_callbackFunction{};
        virtual void call(Event& e) override;

    public:
        EventCallback(TOwner* ownerInstance, const CallbackFunction& callbackFunction)
            : m_ownerInstance{ownerInstance}, m_callbackFunction{callbackFunction}
        {
        }
        virtual ~EventCallback() override = default;
    };

    template <typename TOwner, typename TEvent>
    void EventCallback<TOwner, TEvent>::call(Event& e)
    {
        std::invoke(m_callbackFunction, m_ownerInstance, static_cast<TEvent&>(e));
    }

    using HandlerList = std::list<std::unique_ptr<IEventCallback>>;

    class EventBus
    {
    private:
        std::unordered_map<std::type_index, std::unique_ptr<HandlerList>> m_subscribers;

    public:
        EventBus();
        template <typename TOwner, typename TEvent>
        void subscribeToEvent(TOwner* ownerInstance,
                              const std::function<void(const TOwner&, TEvent&)>& callbackFunction);
        template <typename TEvent, typename... TArgs> void dispatchEvent(TArgs&&... args);
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

    class CollisionEvent : public Event
    {
    public:
        Entity m_entity{};
        Entity m_otherEntity{};

        CollisionEvent(Entity entity, Entity otherEntity) : m_entity{entity}, m_otherEntity{otherEntity} {}
    };
} // namespace Engine

#endif
