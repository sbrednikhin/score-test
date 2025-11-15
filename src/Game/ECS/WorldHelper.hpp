#pragma once

#include "Entity.hpp"
#include <memory>

namespace sw::ecs
{
    class World;

    class WorldHelper
    {
    public:
        // Проверка, жива ли сущность (health > 0)
        static bool IsAlive(const Entity& entity);

        // Сортировка вектора сущностей по ID
        static void SortEntitiesById(std::vector<Entity*>& entities);

        // Перемещение сущности на новые координаты (освобождает старую клетку, занимает новую)
        static bool MoveEntityTo(const World& world, Entity* entity, int32_t newX, int32_t newY);

        // Проверка, есть ли в мире активные сущности (с активными поведениями)
        static bool HasActiveEntities(const World& world);
    };
}
