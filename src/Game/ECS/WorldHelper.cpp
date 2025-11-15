#include "WorldHelper.hpp"
#include "World.hpp"
#include "Components.hpp"
#include "MapService.hpp"
#include "Debug.hpp"
#include <algorithm>

namespace sw::ecs
{
    bool WorldHelper::IsAlive(const Entity& entity)
    {
        auto health = entity.GetComponent<HealthComponent>();
        return health && health->health > 0;
    }


    void WorldHelper::SortEntitiesById(std::vector<Entity*>& entities)
    {
        std::sort(entities.begin(), entities.end(),
            [](Entity* a, Entity* b) {
                return a->GetId() < b->GetId();
            });
    }

    bool WorldHelper::MoveEntityTo(const World& world, Entity* entity, int32_t newX, int32_t newY)
    {
        if (!entity)
        {
            return false;
        }

        // Получаем текущую позицию
        auto* position = entity->GetComponent<PositionComponent>();
        if (!position)
        {
            return false; // Сущность не имеет позиции
        }

        // Если позиция не изменилась, ничего не делаем
        if (position->x == newX && position->y == newY)
        {
            return true;
        }

        // Получаем MapService
        auto* mapService = world.GetService<sw::ecs::MapService>();
        if (!mapService)
        {
            return false;
        }

        // Освобождаем старую клетку
        bool freed = mapService->FreeCell(position->x, position->y);
        if (!freed)
        {
            DEBUG_LOG("Warning: Failed to free cell (" << position->x << "," << position->y << ")");
            // Продолжаем, так как это может быть нормально в некоторых случаях
        }

        // Занимаем новую клетку
        bool occupied = mapService->OccupyCell(newX, newY, entity);
        if (!occupied)
        {
            DEBUG_LOG("Warning: Failed to occupy cell (" << newX << "," << newY << ")");
            // Пытаемся вернуть старую клетку
            mapService->OccupyCell(position->x, position->y, entity);
            return false;
        }

        // Обновляем позицию сущности
        position->x = newX;
        position->y = newY;

        DEBUG_LOG("Entity " << entity->GetId() << " moved to (" << newX << "," << newY << ")");

        return true;
    }

    bool WorldHelper::HasActiveEntities(const World& world)
    {
        // Получаем все сущности с BehaviourComponent
        std::vector<Entity*> entities = world.GetEntitiesWith<BehaviourComponent>();

        // Проверяем, есть ли среди них активные
        for (auto* entity : entities)
        {
            auto* behaviour = entity->GetComponent<BehaviourComponent>();
            if (behaviour && behaviour->is_active)
            {
                return true;
            }
        }

        return false;
    }
}
