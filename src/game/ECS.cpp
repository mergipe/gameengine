#include "ECS.h"

namespace Engine
{
    Entity Registry::createEntity()
    {
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
        const Entity entity{entityId, this};
        m_entitiesToBeAdded.insert(entity);
        Logger::trace("Entity created with id = {}", entityId);
        return entity;
    }

    void Registry::killEntity(Entity entity)
    {
        m_entitiesToBeKilled.insert(entity);
        Logger::trace("Entity {} killed", entity.getId());
    }

    void Registry::addEntityToSystems(Entity entity)
    {
        const auto entityId{entity.getId()};
        const auto& entityComponentSignature{m_entityComponentSignatures[entityId]};
        for (const auto& system : m_systems) {
            const auto& systemComponentSignature{system.second->getComponentSignature()};
            if ((entityComponentSignature & systemComponentSignature) == systemComponentSignature) {
                system.second->addEntity(entity);
            }
        }
        Logger::trace("Entity {} added to systems", entityId);
    }

    void Registry::removeEntityFromSystems(Entity entity)
    {
        for (const auto& system : m_systems) {
            system.second->removeEntity(entity);
        }
        Logger::trace("Entity {} removed from systems", entity.getId());
    }

    void Registry::update()
    {
        for (const auto& entity : m_entitiesToBeAdded) {
            addEntityToSystems(entity);
        }
        m_entitiesToBeAdded.clear();
        for (const auto& entity : m_entitiesToBeKilled) {
            removeEntityFromSystems(entity);
            m_entityComponentSignatures[entity.getId()].reset();
            m_freeEntityIds.push_back(entity.getId());
            removeTag(entity);
            removeGroup(entity);
        }
        m_entitiesToBeKilled.clear();
    }

    void Registry::tagEntity(Entity entity, const std::string& tag)
    {
        m_entityPerTag.emplace(tag, entity);
        m_tagPerEntity.emplace(entity.getId(), tag);
    }

    bool Registry::hasTag(Entity entity, const std::string& tag) const
    {
        if (m_tagPerEntity.find(entity.getId()) == m_tagPerEntity.end()) {
            return false;
        }
        return m_entityPerTag.find(tag)->second == entity;
    }

    Entity Registry::getEntityByTag(const std::string& tag) const { return m_entityPerTag.at(tag); }

    void Registry::removeTag(Entity entity)
    {
        const auto taggedEntity{m_tagPerEntity.find(entity.getId())};
        if (taggedEntity != m_tagPerEntity.end()) {
            const auto& tag{taggedEntity->second};
            m_entityPerTag.erase(tag);
            m_tagPerEntity.erase(taggedEntity);
        }
    }

    void Registry::groupEntity(Entity entity, const std::string& group)
    {
        m_entitiesPerGroup.emplace(group, std::set<Entity>());
        m_entitiesPerGroup[group].emplace(entity);
        m_groupPerEntity.emplace(entity.getId(), group);
    }

    bool Registry::belongsToGroup(Entity entity, const std::string& group) const
    {
        if (m_entitiesPerGroup.find(group) == m_entitiesPerGroup.end()) {
            return false;
        }
        const auto& groupEntities{m_entitiesPerGroup.at(group)};
        return groupEntities.find(entity) != groupEntities.end();
    }

    std::vector<Entity> Registry::getEntitiesByGroup(const std::string& group) const
    {
        const auto& entities{m_entitiesPerGroup.at(group)};
        return std::vector<Entity>(entities.begin(), entities.end());
    }

    void Registry::removeGroup(Entity entity)
    {
        const auto groupedEntity{m_groupPerEntity.find(entity.getId())};
        if (groupedEntity != m_groupPerEntity.end()) {
            const auto entitiesInGroup{m_entitiesPerGroup.find(groupedEntity->second)};
            if (entitiesInGroup != m_entitiesPerGroup.end()) {
                const auto entityInGroup{entitiesInGroup->second.find(entity)};
                if (entityInGroup != entitiesInGroup->second.end()) {
                    entitiesInGroup->second.erase(entityInGroup);
                }
            }
            m_groupPerEntity.erase(groupedEntity);
        }
    }

    void Entity::kill() { m_registry->killEntity(*this); }

    void Entity::tag(const std::string& tag) { m_registry->tagEntity(*this, tag); }

    bool Entity::hasTag(const std::string& tag) const { return m_registry->hasTag(*this, tag); }

    void Entity::group(const std::string& group) { m_registry->groupEntity(*this, group); }

    bool Entity::belongsToGroup(const std::string& group) const
    {
        return m_registry->belongsToGroup(*this, group);
    }
} // namespace Engine
