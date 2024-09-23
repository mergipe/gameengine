#include <ECS.h>
#include <vector>

bool Entity::operator==(const Entity &e) { return getId() == e.getId(); }

void System::addEntity(Entity entity) { entities.push_back(entity); }

void System::removeEntity(Entity entity) { std::erase(entities, entity); }
