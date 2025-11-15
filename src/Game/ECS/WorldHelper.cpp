#include "WorldHelper.hpp"
#include "World.hpp"
#include "Components.hpp"
#include <algorithm>

namespace sw::ecs
{
    bool WorldHelper::IsAlive(const Entity& entity)
    {
        auto health = entity.GetComponent<HealthComponent>();
        return health && health->health > 0;
    }

    Entity* WorldHelper::GetMapEntity(const World& world)
    {
        // Простой кэш - поскольку карта должна быть одна, проверяем только если кэш пустой или ID изменился
        if (!_cachedMapEntity || (_cachedMapEntity && _cachedMapEntity->GetId() != _cachedMapEntityId))
        {
            _cachedMapEntity = nullptr;
            _cachedMapEntityId = 0;

            // Используем публичный API мира для получения сущностей с MapComponent
            std::vector<Entity*> mapEntities = world.GetEntitiesWith<MapComponent>();
            if (!mapEntities.empty())
            {
                // Берем первую найденную карту (должна быть только одна)
                _cachedMapEntity = mapEntities[0];
                _cachedMapEntityId = _cachedMapEntity->GetId();
            }
        }

        return _cachedMapEntity;
    }

    void WorldHelper::SortEntitiesById(std::vector<Entity*>& entities)
    {
        std::sort(entities.begin(), entities.end(),
            [](Entity* a, Entity* b) {
                return a->GetId() < b->GetId();
            });
    }
}
