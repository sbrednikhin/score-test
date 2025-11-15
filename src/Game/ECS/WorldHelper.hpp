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

        // Получение сущности с картой (кэшированная)
        static Entity* GetMapEntity(const World& world);

        // Сортировка вектора сущностей по ID
        static void SortEntitiesById(std::vector<Entity*>& entities);

    private:
        // Кэш для сущности с картой (простой, поскольку карта должна быть одна)
        static inline Entity* _cachedMapEntity = nullptr;
        static inline uint32_t _cachedMapEntityId = 0;
    };
}
