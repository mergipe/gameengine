#ifndef LOCATOR_H
#define LOCATOR_H

#include "core/Logger.h"
#include "events/EventBus.h"

namespace Engine
{
    class Locator
    {
    public:
        static Logger* getLogger() { return s_logger; }
        static EventBus* getEventBus() { return s_eventBus; }
        static void provide(Logger* logger) { s_logger = logger; }
        static void provide(EventBus* eventBus) { s_eventBus = eventBus; }

    private:
        static inline Logger* s_logger;
        static inline EventBus* s_eventBus;
    };
} // namespace Engine

#endif
