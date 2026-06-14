#include "ECSUtils.h"

namespace Engine::ECSUtils
{
    void CopyEntity(entt::handle source, entt::handle target)
    {
        for (auto [id, sourceStorage] : source.storage()) {
            if (sourceStorage.contains(source.entity())) {
                if (auto* destinationStorage{target.registry()->storage(id)}) {
                    destinationStorage->push(target.entity(), sourceStorage.value(source.entity()));
                }
            }
        }
    }
} // namespace Engine::ECSUtils
