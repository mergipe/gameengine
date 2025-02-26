#ifndef EVENTBUS_H
#define EVENTBUS_H

#include "Events.h"
#include <functional>
#include <list>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <utility>

namespace Engine
{
    class IEventCallback
    {
    public:
        IEventCallback(const IEventCallback&) = delete;
        IEventCallback(IEventCallback&&) noexcept = default;
        IEventCallback& operator=(const IEventCallback&) = delete;
        IEventCallback& operator=(IEventCallback&&) noexcept = default;
        virtual ~IEventCallback() = default;
        virtual void execute(Event& e) = 0;

    protected:
        IEventCallback() = default;
    };

    template <typename TOwner, typename TEvent>
    using CallbackFunction = std::function<void(TOwner&, TEvent&)>;

    template <typename TOwner, typename TEvent>
    class EventCallback final : public IEventCallback
    {
    public:
        EventCallback(TOwner* ownerInstance, const CallbackFunction<TOwner, TEvent>& callbackFunction)
            : m_callbackFunction{callbackFunction}, m_ownerInstance{ownerInstance}
        {
        }
        EventCallback(const EventCallback&) = delete;
        EventCallback(EventCallback&&) noexcept = default;
        EventCallback& operator=(const EventCallback&) = delete;
        EventCallback& operator=(EventCallback&&) noexcept = default;
        ~EventCallback() override = default;
        void execute(Event& e) override;

    private:
        CallbackFunction<TOwner, TEvent> m_callbackFunction{};
        TOwner* m_ownerInstance{};
    };

    template <typename TOwner, typename TEvent>
    void EventCallback<TOwner, TEvent>::execute(Event& e)
    {
        m_callbackFunction(*m_ownerInstance, static_cast<TEvent&>(e));
    }

    class EventBus final
    {
    public:
        template <typename TEvent, typename TOwner>
        void addSubscriber(TOwner* ownerInstance, const CallbackFunction<TOwner, TEvent>& callbackFunction);
        template <typename TEvent, typename... TArgs> void dispatchEvent(TArgs&&... args);
        void reset() { m_subscribers.clear(); }

    private:
        using HandlerList = std::list<std::unique_ptr<IEventCallback>>;
        std::unordered_map<std::type_index, std::unique_ptr<HandlerList>> m_subscribers{};
    };

    template <typename TEvent, typename TOwner>
    void EventBus::addSubscriber(TOwner* ownerInstance,
                                 const CallbackFunction<TOwner, TEvent>& callbackFunction)
    {
        if (!m_subscribers[typeid(TEvent)].get()) {
            m_subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
        }
        auto subscriber{std::make_unique<EventCallback<TOwner, TEvent>>(ownerInstance, callbackFunction)};
        m_subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
    }

    template <typename TEvent, typename... TArgs>
    void EventBus::dispatchEvent(TArgs&&... args)
    {
        const auto handlers{m_subscribers[typeid(TEvent)].get()};
        TEvent event{std::forward<TArgs>(args)...};
        if (handlers) {
            for (auto it{handlers->begin()}; it != handlers->end(); ++it) {
                const auto handler{it->get()};
                handler->execute(event);
            }
        }
    }

} // namespace Engine

#endif
