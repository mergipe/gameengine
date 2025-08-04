#ifndef LOCATOR_H
#define LOCATOR_H

#include "core/Logger.h"
#include "events/EventBus.h"
#include "resources/ResourceManager.h"

namespace Engine
{
    class Locator
    {
    public:
        Locator() = delete;
        static Logger* getLogger() { return s_logger; }
        static EventBus* getEventBus() { return s_eventBus; }
        static ResourceManager* getResourceManager() { return s_resourceManager; }
        static void provide(Logger* logger) { s_logger = logger; }
        static void provide(EventBus* eventBus) { s_eventBus = eventBus; }
        static void provide(ResourceManager* resourceManager) { s_resourceManager = resourceManager; }

    private:
        static inline Logger* s_logger;
        static inline EventBus* s_eventBus;
        static inline ResourceManager* s_resourceManager;
    };
} // namespace Engine

#endif
