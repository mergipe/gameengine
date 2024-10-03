#include <ECS.h>
#include <Logger.h>
#include <vector>

namespace Engine {

Entity Registry::createEntity() {
    size_t entityId{};
    if (m_freeEntityIds.empty()) {
        entityId = m_entitiesCount++;
        if (entityId >= m_entityComponentSignatures.size()) {
            m_entityComponentSignatures.resize(entityId + 1);
        }
    } else {
        entityId = m_freeEntityIds.front();
        m_freeEntityIds.pop_front();
    }
    const Entity entity{entityId};
    m_entitiesToBeAdded.insert(entity);
    Logger::trace("Entity created with id = {}", entityId);
    return entity;
}

void Registry::killEntity(Entity entity) {
    m_entitiesToBeKilled.insert(entity);
    Logger::trace("Entity {} killed", entity.getId());
}

void Registry::addEntityToSystems(Entity entity) {
    const auto entityId{entity.getId()};
    const auto &entityComponentSignature{m_entityComponentSignatures[entityId]};
    for (auto &system : m_systems) {
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
    for (auto system : m_systems) {
        system.second->removeEntity(entity);
    }
    Logger::trace("Entity {} removed from systems", entity.getId());
}

void Registry::update() {
    for (auto entity : m_entitiesToBeAdded) {
        addEntityToSystems(entity);
    }
    m_entitiesToBeAdded.clear();
    for (auto entity : m_entitiesToBeKilled) {
        removeEntityFromSystems(entity);
        m_entityComponentSignatures[entity.getId()].reset();
        m_freeEntityIds.push_back(entity.getId());
    }
    m_entitiesToBeKilled.clear();
}

} // namespace Engine
