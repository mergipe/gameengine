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
        static Logger* GetLogger() { return s_logger; }
        static EventBus* GetEventBus() { return s_eventBus; }
        static ResourceManager* GetResourceManager() { return s_resourceManager; }
        static void Provide(Logger* logger) { s_logger = logger; }
        static void Provide(EventBus* eventBus) { s_eventBus = eventBus; }
        static void Provide(ResourceManager* resourceManager) { s_resourceManager = resourceManager; }

    private:
        static inline Logger* s_logger;
        static inline EventBus* s_eventBus;
        static inline ResourceManager* s_resourceManager;
    };
} // namespace Engine

#endif
