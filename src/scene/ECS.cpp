#include <ECS.h>
#include <Logger.h>
#include <vector>

Entity Registry::createEntity() {
    const size_t entityId{entitiesCount++};
    const Entity entity{entityId};
    entitiesToBeAdded.insert(entity);
    if (entityId >= entityComponentSignatures.size()) {
        entityComponentSignatures.resize(entityId + 1);
    }
    Logger::trace("Entity created with id = {}", entityId);
    return entity;
}

void Registry::addEntityToSystems(Entity entity) {
    const auto entityId{entity.getId()};
    const auto &entityComponentSignature{entityComponentSignatures[entityId]};
    for (auto &system : systems) {
        const auto &systemComponentSignature{system.second->getComponentSignature()};
        bool signaturesMatch{(entityComponentSignature & systemComponentSignature) ==
                             systemComponentSignature};
        if (signaturesMatch) {
            system.second->addEntity(entity);
        }
    }
    Logger::trace("Entity {} added to systems", entityId);
}

void Registry::update() {
    for (auto entity : entitiesToBeAdded) {
        addEntityToSystems(entity);
    }
    entitiesToBeAdded.clear();
}
