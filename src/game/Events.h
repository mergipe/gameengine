#ifndef EVENTS_H
#define EVENTS_H

#include <ECS.h>
#include <functional>
#include <list>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <utility>

class Event {
public:
    Event() = default;
};

class IEventCallback {
private:
    virtual void call(Event &e) = 0;

public:
    virtual ~IEventCallback() = default;
    void execute(Event &e) { call(e); }
};

template <typename TOwner, typename TEvent> class EventCallback : public IEventCallback {
private:
    using CallbackFunction = std::function<void(const TOwner &, TEvent &)>;
    TOwner *ownerInstance{};
    CallbackFunction callbackFunction{};
    virtual void call(Event &e) override {
        std::invoke(callbackFunction, ownerInstance, static_cast<TEvent &>(e));
    }

public:
    EventCallback(TOwner *ownerInstance, CallbackFunction callbackFunction)
        : ownerInstance{ownerInstance}, callbackFunction{callbackFunction} {}
    virtual ~EventCallback() override = default;
};

using HandlerList = std::list<std::unique_ptr<IEventCallback>>;

class EventBus {
private:
    std::unordered_map<std::type_index, std::unique_ptr<HandlerList>> subscribers;

public:
    EventBus();
    template <typename TOwner, typename TEvent>
    void subscribeToEvent(TOwner *ownerInstance,
                          std::function<void(const TOwner &, TEvent &)> callbackFunction);
    template <typename T, typename... Args> void dispatchEvent(Args &&...args);
};

template <typename TOwner, typename TEvent>
void EventBus::subscribeToEvent(TOwner *ownerInstance,
                                std::function<void(const TOwner &, TEvent &)> callbackFunction) {
    if (!subscribers[typeid(TEvent)].get()) {
        subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
    }
    auto subscriber{std::make_unique<EventCallback<TOwner, TEvent>>(ownerInstance, callbackFunction)};
    subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
}

template <typename T, typename... Args> void EventBus::dispatchEvent(Args &&...args) {
    const auto handlers{subscribers[typeid(T)].get()};
    T event{std::forward<Args>(args)...};
    if (handlers) {
        for (auto it{handlers->begin()}; it != handlers->end(); ++it) {
            const auto handler{it->get()};
            handler->execute(event);
        }
    }
}

class CollisionEvent : public Event {
public:
    Entity entity{};
    Entity otherEntity{};
    CollisionEvent(Entity entity, Entity otherEntity) : entity{entity}, otherEntity{otherEntity} {}
};

#endif
