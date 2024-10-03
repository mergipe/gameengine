#include <ECS.h>
#include <Logger.h>
#include <vector>

namespace Engine {

Entity Registry::createEntity() {
    size_t entityId{};
    if (freeEntityIds.empty()) {
        entityId = entitiesCount++;
        if (entityId >= entityComponentSignatures.size()) {
            entityComponentSignatures.resize(entityId + 1);
        }
    } else {
        entityId = freeEntityIds.front();
        freeEntityIds.pop_front();
    }
    const Entity entity{entityId};
    entitiesToBeAdded.insert(entity);
    Logger::trace("Entity created with id = {}", entityId);
    return entity;
}

void Registry::killEntity(Entity entity) {
    entitiesToBeKilled.insert(entity);
    Logger::trace("Entity {} killed", entity.getId());
}

void Registry::addEntityToSystems(Entity entity) {
    const auto entityId{entity.getId()};
    const auto &entityComponentSignature{entityComponentSignatures[entityId]};
    for (auto &system : systems) {
        const auto &systemComponentSignature{system.second->getComponentSignature()};
        const bool signaturesMatch{(entityComponentSignature & systemComponentSignature) ==
                                   systemComponentSignature};
        if (signaturesMatch) {
            system.second->addEntity(entity);
        }
    }
    Logger::trace("Entity {} added to systems", entityId);
}

void Registry::removeEntityFromSystems(Entity entity) {
    for (auto system : systems) {
        system.second->removeEntity(entity);
    }
    Logger::trace("Entity {} removed from systems", entity.getId());
}

void Registry::update() {
    for (auto entity : entitiesToBeAdded) {
        addEntityToSystems(entity);
    }
    entitiesToBeAdded.clear();
    for (auto entity : entitiesToBeKilled) {
        removeEntityFromSystems(entity);
        entityComponentSignatures[entity.getId()].reset();
        freeEntityIds.push_back(entity.getId());
    }
    entitiesToBeKilled.clear();
}

} // namespace Engine
