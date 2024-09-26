#include <ECS.h>
#include <Logger.h>
#include <vector>

bool Entity::operator==(const Entity &e) { return getId() == e.getId(); }

void System::addEntity(Entity entity) { entities.push_back(entity); }

void System::removeEntity(Entity entity) { std::erase(entities, entity); }

Entity Registry::createEntity() {
    const int entityId{entitiesCount++};
    const Entity entity{entityId};
    entitiesToBeAdded.insert(entity);
    Logger::debug("Entity created with id = {}", entityId);
    return entity;
}

void Registry::update() {}
